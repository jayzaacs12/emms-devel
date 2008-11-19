<?php
class LOAN_TYPE extends WEBPAGE
{  
  /* LOAN_TYPE parameters */
  var $frostdata   = array();
  var $data        = array( 'creator_id' => 0);
  var $ltypefields = array();

  function LOAN_TYPE($id = '')  //class constructor
  {
  global $_CONF;
  
  if ($id) {
	  $this->data = current(WEBPAGE::$dbh->getAll(sprintf("
                    select
                       lt.*, c.currency, c.symbol, CONCAT(uc.first,' ',uc.last) AS creator,
                       DATE_FORMAT(lt.creator_date,'%s') AS f_creator_date,
		                   CONCAT(ue.first,' ',ue.last) AS editor,
		                   DATE_FORMAT(lt.editor_date,'%s') AS f_editor_date
                    from
                       tblLoanTypes AS lt, tblCurrencys AS c, tblUsers AS uc, tblUsers AS ue
                    where
                       lt.id=%s AND c.id = lt.currency_id AND lt.creator_id = uc.id AND lt.editor_id = ue.id",
                       $_CONF['date_format_mysql'], $_CONF['date_format_mysql'],$id)));

	  $this->data['rates_e'] 	= CalEffRate($this->data['rates_r'],$this->data['fees_at'],$this->data['fees_af'],$this->data['installment'],$this->data['calendar_type'],$this->data['payment_frequency']);
    
    } else {

    $this->data['creator_date'] = date('Y-m-d');
    $this->data['creator_id']   = WEBPAGE::$userID;
    $this->frostdata = array();

    }
  }

  function getTemplateData($id)
  {
  global $_LABELS;

  $ltype = new LOAN_TYPE($id);
  $data = $ltype->data;
    
  $c=0;
  $data['buttondata'][$c][id]	    = "LN.SCR.addLoanType";
  $data['buttondata'][$c][href]		= "index.php?scr_name=LN.SCR.addLoanType&id=".$id;
  $data['buttondata'][$c][alt]		= $_LABELS['edit'];
  $data['buttondata'][$c][onClick]	= "";
  $data['buttondata'][$c][ico]		= "edit";

  return $data;
  }
  
  function payment_frequencys()
  {
  global $_LABELS;
  $opt = explode(",", $_LABELS['tblLoanTypes.payment_frequency.optList']);
  $nam = explode(",", $_LABELS['tblLoanTypes.payment_frequency.optNames']);
  foreach ($opt as $key => $val) {
    $pf[$val] = $nam[$key];
    }
  return $pf;
  }

  function borrower_types()
  {
  global $_LABELS;
  $opt = explode(",", $_LABELS['tblLoanTypes.borrower_type.optList']);
  $nam = explode(",", $_LABELS['tblLoanTypes.borrower_type.optNames']);
  foreach ($opt as $key => $val) {
    $bt[$val] = $nam[$key];
    }
  return $bt;
  }

  function calendar_types()
  {
  global $_LABELS;
  $opt = explode(",", $_LABELS['tblLoanTypes.calendar_type.optList']);
  $nam = explode(",", $_LABELS['tblLoanTypes.calendar_type.optNames']);
  foreach ($opt as $key => $val) {
    $ct[$val] = $nam[$key];
    }
  return $ct;
  }

  function currencys()
  {
  return SQL::getAssoc('tblCurrencys','id,currency');
  }

  function add()
  {
  WEBPAGE::$dbh->query(sprintf("insert into tblLoanTypes  set
                                                         borrower_type          = '%s',
                                                         description            = '%s',
                                                         installment            = '%s',
                                                         installment_lock       = '%s',
                                                         rates_r                = '%s',
                                                         rates_r_lock           = '%s',
                                                         rates_d                = '%s',
                                                         rates_d_lock           = '%s',
                                                         fees_at                = '%s',
                                                         fees_at_lock           = '%s',
                                                         fees_af                = '%s',
                                                         fees_af_lock           = '%s',
                                                         margin_r               = '%s',
                                                         margin_k               = '%s',
                                                         margin_d               = '%s',
                                                         margin_d_lock          = '%s',
                                                         payment_frequency      = '%s',
                                                         calendar_type          = '%s',
                                                         margin_c               = '%s',
                                                         margin_c_lock          = '%s',
                                                         savings_p              = '%s',
                                                         savings_p_lock         = '%s',
                                                         currency_id            = '%s',
                                                         creator_id             = '%s',
                                                         creator_date           = '%s',
                                                         editor_id              = '%s',
                                                         editor_date            = '%s'",
                                                         $this->data['borrower_type'],
                                                         $this->data['description'],
                                                         $this->data['installment'],
                                                         $this->data['installment_lock'],
                                                         $this->data['rates_r'],
                                                         $this->data['rates_r_lock'],
                                                         $this->data['rates_d'],
                                                         $this->data['rates_d_lock'],
                                                         $this->data['fees_at'],
                                                         $this->data['fees_at_lock'],
                                                         $this->data['fees_af'],
                                                         $this->data['fees_af_lock'],
                                                         $this->data['margin_r'],
                                                         $this->data['margin_k'],
                                                         $this->data['margin_d'],
                                                         $this->data['margin_d_lock'],
                                                         $this->data['payment_frequency'],
                                                         $this->data['calendar_type'],
                                                         $this->data['margin_c'],
                                                         $this->data['margin_c_lock'],
                                                         $this->data['savings_p'],
                                                         $this->data['savings_p_lock'],
                                                         $this->data['currency_id'],
                                                         $this->data['creator_id'],
                                                         $this->data['creator_date'],
                                                         $this->data['editor_id'],
                                                         $this->data['editor_date']));
  return mysql_insert_id(WEBPAGE::$dbh->connection);
  }

  function update()
  {
  WEBPAGE::$dbh->query(sprintf("update tblLoanTypes set
                                                         borrower_type          = '%s',
                                                         description            = '%s',
                                                         installment            = '%s',
                                                         installment_lock       = '%s',
                                                         rates_r                = '%s',
                                                         rates_r_lock           = '%s',
                                                         rates_d                = '%s',
                                                         rates_d_lock           = '%s',
                                                         fees_at                = '%s',
                                                         fees_at_lock           = '%s',
                                                         fees_af                = '%s',
                                                         fees_af_lock           = '%s',
                                                         margin_r               = '%s',
                                                         margin_k               = '%s',
                                                         margin_d               = '%s',
                                                         margin_d_lock          = '%s',
                                                         payment_frequency      = '%s',
                                                         calendar_type          = '%s',
                                                         margin_c               = '%s',
                                                         margin_c_lock          = '%s',
                                                         savings_p              = '%s',
                                                         savings_p_lock         = '%s',
                                                         currency_id            = '%s',
                                                         editor_id              = '%s',
                                                         editor_date            = '%s'
                                                   where
                                                         id                     =  %s",
                                                         $this->data['borrower_type'],
                                                         $this->data['description'],
                                                         $this->data['installment'],
                                                         $this->data['installment_lock'],
                                                         $this->data['rates_r'],
                                                         $this->data['rates_r_lock'],
                                                         $this->data['rates_d'],
                                                         $this->data['rates_d_lock'],
                                                         $this->data['fees_at'],
                                                         $this->data['fees_at_lock'],
                                                         $this->data['fees_af'],
                                                         $this->data['fees_af_lock'],
                                                         $this->data['margin_r'],
                                                         $this->data['margin_k'],
                                                         $this->data['margin_d'],
                                                         $this->data['margin_d_lock'],
                                                         $this->data['payment_frequency'],
                                                         $this->data['calendar_type'],
                                                         $this->data['margin_c'],
                                                         $this->data['margin_c_lock'],
                                                         $this->data['savings_p'],
                                                         $this->data['savings_p_lock'],
                                                         $this->data['currency_id'],
                                                         $this->data['editor_id'],
                                                         $this->data['editor_date'],
                                                         $this->data['id']));
  return $this->data['id'];
  }

}
?>