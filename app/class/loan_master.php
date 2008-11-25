<?php
class LOAN_MASTER extends WEBPAGE
{
  /* LOAN_MASTER parameters */
  var $frostdata = array();
  var $data  = array();
  var $fields = array();

  function LOAN_MASTER($id = '')  //class constructor
  {
  if ($id) {

    $this->data = current(SQL::select('tblLoansMaster','*',sprintf('id=%s',$id)));
    $this->loadloans();

    } else {

    $this->data['check_status'] = 'P';
    $this->data['creator_date'] = date('Y-m-d');
    $this->data['editor_date']  = date('Y-m-d');
    $this->data['creator_id']   = WEBPAGE::$userID;
    $this->data['editor_id']    = WEBPAGE::$userID;

    }
  }

  function updateAmount($id)
  {
  WEBPAGE::$dbh->query(sprintf("update tblLoansMaster as lm set lm.amount = (select sum(l.kp) from tblLoans as l, tblLoansMasterDetails as lmd where lmd.master_id = %s and lmd.loan_id = l.id) where lm.id = %s",$id,$id));
  }

  function getTemplateData($id)
  {
  /*
  global $_LABELS;
  global $_CONF;

  $fields = sprintf("
  				f.*, c.currency,
		    	CONCAT(uc.first,' ',uc.last) AS creator,
		    	DATE_FORMAT(f.creator_date,'%s') AS f_creator_date,
		    	CONCAT(ue.first,' ',ue.last) AS editor,
		    	DATE_FORMAT(f.editor_date,'%s') AS f_editor_date", $_CONF['date_format_mysql'], $_CONF['date_format_mysql']);
  $tables = "tblFunds AS f, tblCurrencys AS c, tblUsers AS uc, tblUsers AS ue";
  $param  = sprintf("f.id = '%s' AND c.id = f.currency_id AND f.creator_id = uc.id AND f.editor_id = ue.id",$id);

  $data 				  = current(SQL::select($tables, $fields, $param));

  $c=0;
  $data['buttondata'][$c][id]	    = "AC.SCR.addFund";
  $data['buttondata'][$c][href]		= "index.php?scr_name=AC.SCR.addFund&id=".$id;
  $data['buttondata'][$c][alt]		= $_LABELS['edit'];
  $data['buttondata'][$c][onClick]	= "";
  $data['buttondata'][$c][ico]		= "edit";

  return $data;
  */
  }

  function check_status()
  {
  global $_LABELS;
  $opt = explode(",", $_LABELS['tblLoansMaster.check_status.optList']);
  $nam = explode(",", $_LABELS['tblLoansMaster.check_status.optNames']);
  foreach ($opt as $key => $val) {
    $ls[$val] = $nam[$key];
    }
  return $ls;
  }

  function get_duplicates()
  {
  global $_CONF;
  if ($_CONF['duplicate_chk_request_margin'] < 0) { return array(); }
  return WEBPAGE::$dbh->getAssoc(sprintf("select
                                        id,id
                                    from
                                        tblLoansMaster
                                    where
                                        id != '%s' and
                                        borrower_id = '%s' and
                                        borrower_type = '%s' and
                                        abs(datediff(creator_date,'%s')) <= '%s'",
                                        $this->data['id'],
                                        $this->data['borrower_id'],
                                        $this->data['borrower_type'],
                                        $this->data['creator_date'],
                                        $_CONF['duplicate_chk_request_margin']));
  }
  
  function loadloans()
  {
  $this->data['loans'] = SQL::getAssoc('tblLoansMasterDetails AS lmd, tblLoans AS l', 'l.id, l.status', sprintf('lmd.master_id = %s AND l.id = lmd.loan_id', $this->data['id']));
  foreach($this->data['loans'] as $status) {
    $this->data['cancelled'] = false;
    if ($status != 'C') { break; } else { $this->data['cancelled'] = true; }
    }
  }

  function loadextrainfo()
  {
  $this->data['extrainfo'] = WEBPAGE::$dbh->getAssoc(sprintf("select l.id,l.kp,l.pmt,lt.payment_frequency,concat(c.first,' ',c.last) client_name,c.code client_code,c.address client_address from tblLoans l, tblClients c, tblLoanTypes lt where l.id in (select lmd.loan_id id from tblLoansMasterDetails lmd where lmd.master_id = %s) and c.id = l.client_id and lt.id = l.loan_type_id", $this->data['id']));
  }

  function load_pmt_receipt_flag_a()
  {
  $this->data['pmt_receipt_flag_a'] = WEBPAGE::$dbh->getAssoc(sprintf("select p.loan_id, count(p.id) from tblPayments p, tblReceipts r, tblLinkReceiptsPayments rp where p.loan_id in (select lmd.loan_id from tblLoansMasterDetails lmd where lmd.master_id = %s) and  r.flag_a = '1' and r.id = rp.receipt_id and rp.payment_id = p.id group by p.loan_id", $this->data['id']));
  }

  function load_pmt_receipt_flag_b()
  {
  $this->data['pmt_receipt_flag_b'] = WEBPAGE::$dbh->getAssoc(sprintf("select p.loan_id, count(p.id) from tblPayments p, tblReceipts r, tblLinkReceiptsPayments rp where p.loan_id in (select lmd.loan_id from tblLoansMasterDetails lmd where lmd.master_id = %s) and  r.flag_b = '1' and r.id = rp.receipt_id and rp.payment_id = p.id group by p.loan_id", $this->data['id']));
  }

  function loadpayments()
  {
  $this->data['payments'] = WEBPAGE::$dbh->getAssoc(sprintf("SELECT p.loan_id, concat(c.first,' ',c.last) client, sum(p.pmt) pmt, sum(p.principal) principal, sum(p.interest) interest, sum(p.fees) fees, sum(p.insurances) insurances, sum(p.penalties) penalties FROM tblClients c, tblLoans l, tblPayments p, tblLoansMasterDetails lmd where c.id = l.client_id and l.id = lmd.loan_id and p.loan_id = lmd.loan_id and lmd.master_id = %s group by p.loan_id;", $this->data['id']));
  }

  function loadBorrowerData()
  {
  switch ($this->data['borrower_type']) {
    case 'B':
//      require_once 'class/society.php';
      $this->data['borrower'] = new SOCIETY($this->data['borrower_id']);
      break;
    case 'G':
//      require_once 'class/society.php';
      $this->data['borrower'] = new SOCIETY($this->data['borrower_id']);
      break;
    case 'I':
//      require_once 'class/client.php';
      $this->data['borrower'] = new CLIENT($this->data['borrower_id']);
      break;
    default:
      WEBPAGE::redirect('index.php?logout=1');
      exit;
    }
  }

  function getBorrowerZone()
  {
    return  current(WEBPAGE::$dbh->getAssoc(sprintf("select id,id from tblZones where parent_id = %s and program_id = %s", $this->data['zone_id'],$this->data['program_id'])));

  }
}
?>
