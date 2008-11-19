<?php
 // PATCH: update 2008.07.16
require_once 'Date.php';

class LOAN extends WEBPAGE
{
  /* LOAN parameters */
  var $frostdata 	= array('payment_frequency','calendar_type','margin_k','margin_r','margin_k');
  var $data  		= array('creator_id'=> 0);
  var $loanfields 	= array();

  function LOAN($id = '',$loan_type_id = '')  //class constructor
  {
  global $_CONF;

  if ($id) {
  // PATCH: update 2008.07.16 SQL change
    $this->data = current(WEBPAGE::$dbh->getAll(sprintf(
             "select
                l.*,
	           	(l.kp/(l.kp+l.kaf+l.kat))  AS P_KP,
	           	(l.kaf/(l.kp+l.kaf+l.kat)) AS P_KAF,
	           	(l.kat/(l.kp+l.kaf+l.kat)) AS P_KAT,
	           	lcd.*,
			   	    lmd.master_id,
			   	    lt.borrower_type,
				      lt.calendar_type,
				      lt.payment_frequency,
				      lt.margin_k,
				      lt.margin_r,
				      lt.margin_c,
				      lt.description as modality,
			   	    c.currency,
			   	    c.symbol as currency_symbol,
    	       	CONCAT(uc.first,' ',uc.last) AS creator,
			   	    DATE_FORMAT(l.creator_date,'%s') AS f_creator_date,
		       	  CONCAT(ue.first,' ',ue.last) AS editor,
		       	  DATE_FORMAT(l.editor_date,'%s') AS f_editor_date,lm.xp_delivered_date,lm.xp_first_payment_date,lm.check_status
		      from
              (tblLoans AS l, tblLoansMasterDetails AS lmd, tblLoansMaster AS lm,tblLoanTypes AS lt, tblCurrencys AS c, tblUsers AS uc, tblUsers AS ue)
          left join
              tblLoansCurrentData AS lcd on lcd.loan_id = l.id
          where
              lmd.master_id = lm.id AND lmd.loan_id = l.id AND
              l.id=%s AND lmd.loan_id = l.id AND l.loan_type_id = lt.id AND c.id = lt.currency_id AND l.creator_id = uc.id AND l.editor_id = ue.id",
              $_CONF['date_format_mysql'], $_CONF['date_format_mysql'],$id)));

	  $this->data['collateral_value'] 	= $this->data['pg_value'] + $this->data['re_value'] + $this->data['fgd_value'] + $this->data['fgt_value'];
	  $this->data['collateral_min_value'] = $this->data['kp'] * (1 + 0.01 * $this->data['margin_c'] );
	  $this->data['rates_e'] 				= CalEffRate($this->data['rates_r'],$this->data['fees_at'],$this->data['fees_af'],$this->data['installment'],$this->data['calendar_type'],$this->data['payment_frequency']);

      // PATCH: update 2008.07.16
      $this->data['delivered_date'] = ($this->check_xp_delivered_date() && $this->data['delivered_date'] = '0000-00-00') ? $this->data['xp_delivered_date'] : $this->data['delivered_date'];
      $this->data['first_payment_date'] = ($this->check_xp_first_payment_date() && $this->data['first_payment_date'] = '0000-00-00') ? $this->data['xp_first_payment_date'] : $this->data['first_payment_date'];


  } else {

    $this->data['status'] = 'N';
    $this->data['creator_date'] = date('Y-m-d');
    $this->data['editor_date']  = date('Y-m-d');
    $this->data['creator_id']   = WEBPAGE::$userID;
    $this->data['editor_id']    = WEBPAGE::$userID;
    $this->data['loan_type_id'] = $loan_type_id;


    $this->load_defaults($loan_type_id);

    }

  $this->load_frostdata();

  }

  function getTemplateData($id)
  {
  global $_LABELS;
  global $_CONF;

  $loan = new LOAN($id);
  $loan->load_borrower($loan->data['client_id']);
  $data = $loan->data;
  $data['status_controls'] = $loan->status_controls();

  if ($loan->survey_check()) {
  	$data['survey'] 		= $_LABELS['results'];
  	$data['href_survey'] 	= sprintf('index.php?scr_name=SV.SCR.viewGraph&client_id=%s&survey_id=%s',$loan->data['client_id'],$_CONF['survey_id']);
    } else {
 	$data['survey'] 		= $_LABELS['applySurvey'];
 	$data['href_survey'] 	= sprintf('index.php?scr_name=SV.SCR.applySurveyStepB&loan_id=%s&client_id=%s&id=%s',$loan->data['id'],$loan->data['client_id'],$_CONF['survey_id']);
   }

  $c=0;
  if (!$loan->isFrost()) {
    $data['buttondata'][$c]['id']	    = "LN.SCR.addILoan";
    $data['buttondata'][$c]['href']		= "index.php?scr_name=LN.SCR.addILoan&ref=LN.SCR.requestILoan&id=".$id;
    $data['buttondata'][$c]['alt']		= $_LABELS['edit'];
    $data['buttondata'][$c]['onClick']	= "";
    $data['buttondata'][$c]['ico']		= "edit";
    $c++;
    }

  $data['buttondata'][$c]['id']			="LN.SCR.viewLoanMaster";
  $data['buttondata'][$c]['href']		=sprintf('index.php?scr_name=LN.SCR.viewLoanMaster&id=%s',$data['master_id']);
  $data['buttondata'][$c]['alt']		= $_LABELS['LN.SCR.viewLoanMaster'];
  $data['buttondata'][$c]['onClick']	="";
  $data['buttondata'][$c]['ico']		="view";
  $c++;

  if ($loan->data['status']=='G') {
    $data['buttondata'][$c]['id']		="LN.SCR.addPayment";
    $data['buttondata'][$c]['href']		='javascript:openWin("index.popup.php?scr_name=LN.SCR.addPayment&ref=LN.SCR.viewILoan&id='.$id.'","addPayment","menubar=no,scrollbars=yes,resizable=yes,width=700,height=320")';
    $data['buttondata'][$c]['alt']		= $_LABELS['LN.SCR.addPayment'];
    $data['buttondata'][$c]['onClick']	="";
    $data['buttondata'][$c]['ico']		="money_payment";
    $c++;
	}

  if ($loan->data['status']=='G') {
    $data['buttondata'][$c]['id']		="LN.SCR.addPaymentFull";
    $data['buttondata'][$c]['href']		='javascript:openWin("index.popup.php?scr_name=LN.SCR.addPaymentFull&id='.$id.'","addPayment","menubar=no,scrollbars=yes,resizable=yes,width=700,height=320")';
    $data['buttondata'][$c]['alt']		= $_LABELS['LN.SCR.addPaymentFull'];
    $data['buttondata'][$c]['onClick']	="";
    $data['buttondata'][$c]['ico']		="money_payment_full";
    $c++;
	}

  $data['buttondata'][$c]['id']			="LN.SCR.pmtPlan";
  $data['buttondata'][$c]['href']		='javascript:openWin("index.popup.php?scr_name=LN.SCR.pmtPlan&ref=LN.SCR.viewILoan&id='.$id.'","pmtPlan","menubar=no,scrollbars=yes,resizable=yes,width=700,height=320")';
  $data['buttondata'][$c]['alt']		= $_LABELS['LN.SCR.pmtPlan'];
  $data['buttondata'][$c]['onClick']	="";
  $data['buttondata'][$c]['ico']		="money_plan";
  $c++;

  $data['buttondata'][$c]['id']			="LN.RPT.loanStatusHistory";
  $data['buttondata'][$c]['href']		='javascript:openWin("index.popup.php?scr_name=LN.RPT.loanStatusHistory&ref=LN.SCR.viewILoan&id='.$id.'","ClientIOM","menubar=no,scrollbars=yes,resizable=yes,width=700,height=320")';
  $data['buttondata'][$c]['alt']		= $_LABELS['statusHistory'];
  $data['buttondata'][$c]['onClick']	="";
  $data['buttondata'][$c]['ico']		="money_history";
  $c++;

  if (($loan->data['status']=='LI')||($loan->data['status']=='LO')) {
    $data['buttondata'][$c]['id']		="LG.SCR.legalProfile";
    $data['buttondata'][$c]['href']		='javascript:openWin("index.popup.php?scr_name=LG.SCR.legalProfile&id='.$id.'","legalProfile","menubar=no,scrollbars=yes,resizable=yes,width=700,height=320")';
//    $data['buttondata'][$c]['href']		=sprintf('index.php?scr_name=LG.SCR.legalProfile&id=%s',$id);
    $data['buttondata'][$c]['alt']		= $_LABELS['LG.SCR.legalProfile'];
    $data['buttondata'][$c]['onClick']	="";
    $data['buttondata'][$c]['ico']		="legalwork";
    $c++;
	}
  return $data;
  }

  private function load_frostdata()
  {
    $ltype = new LOAN_TYPE($this->data['loan_type_id']);
    $this->frostdata = $ltype->data['installment_lock'] ? array_merge($this->frostdata,array('installment')) : $this->frostdata;
    $this->frostdata = $ltype->data['rates_r_lock']     ? array_merge($this->frostdata,array('rates_r'))     : $this->frostdata;
    $this->frostdata = $ltype->data['rates_d_lock']     ? array_merge($this->frostdata,array('rates_d'))     : $this->frostdata;
    $this->frostdata = $ltype->data['fees_at_lock']     ? array_merge($this->frostdata,array('fees_at'))     : $this->frostdata;
    $this->frostdata = $ltype->data['fees_af_lock']     ? array_merge($this->frostdata,array('fees_af'))     : $this->frostdata;
    $this->frostdata = $ltype->data['margin_d_lock']    ? array_merge($this->frostdata,array('margin_d'))    : $this->frostdata;
    $this->frostdata = $ltype->data['margin_c_lock']    ? array_merge($this->frostdata,array('margin_c'))    : $this->frostdata;
    $this->frostdata = $ltype->data['margin_c_lock']    ? array_merge($this->frostdata,array('collateral_min_value'))    : $this->frostdata;
    $this->frostdata = $ltype->data['savings_p_lock']   ? array_merge($this->frostdata,array('savings_p'))   : $this->frostdata;
  }

  private function load_defaults($loanTypeID = '')
  {
  if ($loanTypeID) {
    $ltype = new LOAN_TYPE($loanTypeID);
    $this->data['modality'] 			    =   $ltype->data['description'];
    $this->data['installment'] 			  =   $ltype->data['installment'];
    $this->data['installment_lock']   =   $ltype->data['installment_lock'];
    $this->data['payment_frequency'] 	=   $ltype->data['payment_frequency'];
    $this->data['calendar_type'] 		  =   $ltype->data['calendar_type'];
    $this->data['margin_c'] 			    =   $ltype->data['margin_c'];
    $this->data['margin_c_lock'] 		  =   $ltype->data['margin_c_lock'];
    $this->data['margin_r'] 			    =   $ltype->data['margin_r'];
    $this->data['margin_k'] 			    =   $ltype->data['margin_k'];
    $this->data['margin_d'] 			    =   $ltype->data['margin_d'];
    $this->data['margin_d_lock'] 	    =   $ltype->data['margin_d_lock'];
    $this->data['rates_r'] 				    =   $ltype->data['rates_r'];
    $this->data['rates_r_lock']       =   $ltype->data['rates_r_lock'];
    $this->data['rates_d'] 				    =   $ltype->data['rates_d'];
    $this->data['rates_d_lock'] 		  =   $ltype->data['rates_d_lock'];
    $this->data['fees_at'] 				    =   $ltype->data['fees_at'];
    $this->data['fees_at_lock'] 		  =   $ltype->data['fees_at_lock'];
    $this->data['fees_af'] 				    =   $ltype->data['fees_af'];
    $this->data['fees_af_lock'] 		  =   $ltype->data['fees_af_lock'];
    $this->data['rates_e'] 				    = 	round(CalEffRate($this->data['rates_r'],$this->data['fees_at'],$this->data['fees_af'],$this->data['installment'],$this->data['calendar_type'],$this->data['payment_frequency']),2);
    $this->data['borrower_type'] 		  =   $ltype->data['borrower_type'];
    $this->data['savings_p_lock']     =   $ltype->data['savings_p_lock'];
	  }
  }

  function load_borrower($borrower_id)
  {
  switch ($this->data['borrower_type']) {
	CASE 'G':
		$borrower = new SOCIETY($borrower_id);
		$lnk      = sprintf('index.php?scr_name=BS.SCR.viewSociety&id=%s',$borrower_id);
		break;
	DEFAULT :
		$borrower = new CLIENT($borrower_id);
		$lnk      = sprintf('index.php?scr_name=BS.SCR.viewClient&id=%s',$borrower_id);
		break;
    }
  $this->data['borrower_name'] 		= $borrower->data['name'];
  $this->data['borrower_link'] 		= $lnk;
  $this->data['borrower_code'] 		= $borrower->data['code'];
  $this->data['borrower_img_path'] 	= $borrower->data['img_path'];
  }

  function status()
  {
  global $_LABELS;
  $opt = explode(",", $_LABELS['tblLoans.status.optList']);
  $nam = explode(",", $_LABELS['tblLoans.status.optNames']);
  foreach ($opt as $key => $val) {
    $ls[$val] = $nam[$key];
    }
  return $ls;
  }

  function status_controls()
  {
  global $_CONF;
  $status_controls = array();
  $loan_status = unpackloanstatus($_CONF['loan_status'],';','|',',');
  $c = 0;
  for ($i=0;$i<count($loan_status[$this->data['status']]['options']);$i++) {
    if ($loan_status[$loan_status[$this->data['status']]['options'][$i]]['button']) {
	  $status_controls[$c]['link'] = sprintf("index.php?scr_name=LN.SCR.updateStatus&ref=LN.SCR.browseLoans&status=%s&p_status=%s&id=%s", $loan_status[$this->data['status']]['options'][$i], $this->data['status'], $this->data['id']);
	  $status_controls[$c]['text'] = $loan_status[$loan_status[$this->data['status']]['options'][$i]]['label'];
	  $c++;
	  }
    }
  return $status_controls;
  }

  function survey_check() {
  global $_CONF;
  if ( $_CONF['survey_check'] == '0' ) { return true; }
  switch ( $this->data['status'] ) {
    case 'G':  return true;
    case 'C':  return true;
    case 'LI': return true;
    case 'LO': return true;
    }

  $tables = 'tblSurveyAnswers';
  $fields = 'client_id';
  $params = sprintf("survey_id = '%s'
				AND	( CURDATE()<DATE_ADD(date, INTERVAL %s MONTH) )
				AND	client_id = '%s'",$_CONF['survey_id'],$_CONF['survey_margin'],$this->data['client_id']);
  return count(SQL::select($tables,$fields,$params));
  }

  function business_check() {
  global $_CONF;
  if ( $_CONF['business_check'] == '0' ) { return true; }
  return current(WEBPAGE::$dbh->getAssoc(sprintf('select id,business_id from tblLoans where id = %s',$this->data['id'])));
  }

  function CalMinPMT()
  {
	$n = $this->CalNumOfPMT();
	$i = (0.01*$this->data['rates_r']*$this->data['installment'])/($this->data['calendar_type']*$n);
	return ceil((($this->data['kp']+$this->data['kat']+$this->data['kaf'])*$i)/(1-pow((1/(1+$i)),$n)));
  }

  function CalNumOfPMT()
  {
  switch($this->data['payment_frequency']) {
    case  "W": return floor($this->data['installment'] /   7);
	case "BW": return floor($this->data['installment'] /  14);
	case  "M": return floor($this->data['installment'] /  30);
	case  "Q": return floor($this->data['installment'] /  90);
	case "SA": return floor($this->data['installment'] / 180);
	case  "A": return floor($this->data['installment'] / 360);
	default  : WEBPAGE::redirect('index.php?scr_name=BS.SCR.message&ico=err&msg=err');
    }
  }

  function next_payment_date($pdate = '')
  {
  $date = new Date($pdate ? $pdate : $this->data['xp_pmt_date']);
  switch($this->data['payment_frequency']) {
    case "W":
      $date->addSeconds(7*24*60*60);
      break;
    case "BW":
      $date->addSeconds(14*24*60*60);
      break;
    case "M":
      $date->addSeconds($date->getDaysInMonth()*24*60*60);
	  break;
    case "Q":
      $date->addSeconds($date->getDaysInMonth()*24*60*60);
      $date->addSeconds($date->getDaysInMonth()*24*60*60);
      $date->addSeconds($date->getDaysInMonth()*24*60*60);
	  break;
	case "SA":
      $date->addSeconds($date->getDaysInMonth()*24*60*60);
      $date->addSeconds($date->getDaysInMonth()*24*60*60);
      $date->addSeconds($date->getDaysInMonth()*24*60*60);
      $date->addSeconds($date->getDaysInMonth()*24*60*60);
      $date->addSeconds($date->getDaysInMonth()*24*60*60);
      $date->addSeconds($date->getDaysInMonth()*24*60*60);
	  break;
	case "A":
      $date->setYear($date->getYear()+1);
	  break;
	}
  return $date->format('%Y-%m-%d');
  }

  function load_penalties()
  {
  if ($this->data['cn_date'] <= $this->data['xp_pmt_date']) {
  	$this->data['delay'] 		= 0;
  	$this->data['penalties'] 	= 0;
    } else {
    $date = new Date($this->data['xp_pmt_date']);
    $n = 0;
    while ($date->format('%Y-%m-%d') != $this->data['cn_date']) {
      $date->addSeconds(24*60*60);
      $n++;
      }
    $this->data['delay'] 		= $n;
    if ($n <= $this->data['margin_d']) {
  	  $this->data['penalties'] = 0;
      } else {
      $this->data['penalties'] 	= round(0.01*$this->data['rates_d']*$n*$this->data['xp_pmt'],2);
      }
    }
  }

  function load_xp_pmt_plan()
  {
  $this->data['balance_kp']			= $this->data['kp'];
  $this->data['balance_kaf']		= $this->data['kaf'];
  $this->data['balance_kat']		= $this->data['kat'];
  if ($this->data['delivered_date'] == '0000-00-00') { $this->data['r_from_date'] = date('Y-m-d'); } else { $this->data['r_from_date'] = $this->data['delivered_date']; }
  if ($this->data['first_payment_date'] == '0000-00-00') { $this->data['xp_pmt_date'] = $this->next_payment_date($this->data['r_from_date']); } else { $this->data['xp_pmt_date'] = $this->data['first_payment_date']; }
  $this->data['cn_date']			= $this->data['r_from_date'];
  $c = 0;
  $balance_kp						= $this->data['balance_kp'];
  while($balance_kp > 0) {

    $data[$c]						= $this->getNextPaymentData('plan');
    $this->data['balance_kp']		= $data[$c]['balance_kp'];
    $this->data['balance_kaf']		= $data[$c]['balance_kaf'];
    $this->data['balance_kat']		= $data[$c]['balance_kat'];
    $this->data['r_from_date']		= $data[$c]['r_from_date'];
    $this->data['xp_pmt_date']		= $data[$c]['xp_pmt_date'];
    $this->data['xp_pmt']			= $data[$c]['xp_pmt'];

    $totals['fees']					+= $data[$c]['fees'];
    $totals['insurances']			+= $data[$c]['insurances'];
    $totals['principal']			+= $data[$c]['principal'];
    $totals['interest']				+= $data[$c]['interest'];
    $totals['pmt']					+= $data[$c]['pmt'];

    $balance_kp 					= $data[$c]['balance_kp'];
    $c++;

    }
  $this->data['xp_pmt_plan']		= $data;
  $this->data['xp_pmt_plan_totals']	= $totals;
  }

  function getNextPaymentData($mode = '')
  {
  $mode 					        = $mode ? $mode : 'plan';
  $balance					      = $this->data['balance_kp'] + $this->data['balance_kaf'] + $this->data['balance_kat'];
  $date 					        = new Date($this->data['r_from_date']);
  $data['pmt_date'] 		  = ($mode == 'plan') ? $this->data['xp_pmt_date'] : $this->data['cn_date'];
  $n = 0;
  while ($date->format('%Y-%m-%d') != $data['pmt_date']) {
    $date->addSeconds(24*60*60);
    $n++;
    }
  $data['xp_pmt_date'] 		= $this->next_payment_date();
  $data['r_from_date']		= $date->format('%Y-%m-%d');
  $data['interest'] 		= round(($balance*(0.01*$this->data['rates_r']/$this->data['calendar_type'])*$n),2);
  $data['fees'] 			= round(($this->data['P_KAT']*($this->data['pmt']-$data['interest'])),2);
  $data['insurances'] 		= round(($this->data['P_KAF']*($this->data['pmt']-$data['interest'])),2);
  $data['principal']  		= $this->data['pmt'] - $data['interest'] - $data['fees'] - $data['insurances'];
  $data['balance_kp'] 		= $this->data['balance_kp']  - $data['principal'];
  $data['balance_kaf'] 		= $this->data['balance_kaf'] - $data['insurances'];
  $data['balance_kat'] 		= $this->data['balance_kat'] - $data['fees'];
  $this->load_penalties();
  $data['delay']			= $this->data['delay'];
  $data['penalties']		= $this->data['penalties'];
  $data['pmt']				= $this->data['pmt'] + $this->data['penalties'];
  $data['xp_pmt']			= $this->data['pmt']; // <-- esto hay que revisarlo
  if (($data['balance_kp'] < 0)||($data['balance_kaf'] < 0)||($data['balance_kat'] < 0)) {
    $data['fees'] 			= $this->data['balance_kat'];
    $data['insurances'] 	= $this->data['balance_kaf'];
    $data['principal']  	= $this->data['balance_kp'];
    $data['balance_kp'] 	= 0;
    $data['balance_kaf'] 	= 0;
    $data['balance_kat'] 	= 0;
    $data['pmt']			= $data['interest'] + $data['fees'] + $data['insurances'] + $data['principal'] + $data['penalties'];
    $data['xp_pmt']			= 0;
    }
  return $data;
  }

  function cancelled()
  {
  if ($this->data['balance_kp'] == 0) {
    WEBPAGE::$dbh->query(sprintf("delete from tblLoansCurrentData where loan_id = %s", $this->data['id']));
    WEBPAGE::$dbh->query(sprintf("insert into tblLoanStatusHistory (loan_id,p_status,status,date,user_id) values ('%s','%s','%s','%s','%s')",$this->data['id'],$this->data['status'],'C',$this->data['cn_date'],WEBPAGE::$userID));
    WEBPAGE::$dbh->query(sprintf("update tblLoans set status = 'C' where id = %s", $this->data['id']));
    return true;
    }
  return false;
  }

  function legalOut()
  {
  if ($this->data['status'] != 'LI') { return false; }
  unset($this->data['legal']);
  $this->load_legal_data();
  WEBPAGE::$dbh->query(sprintf("insert into tblLoanStatusHistory (loan_id,p_status,status,date,user_id) values ('%s','%s','%s','%s','%s')",$this->data['id'],$this->data['status'],'LO',date('Y-m-d'),WEBPAGE::$userID));
  WEBPAGE::$dbh->query(sprintf("update tblLoans set status = 'LO' where id = %s", $this->data['id']));
  return true;
  }

  function writeOff($description = '')
  {
  if ($this->data['status'] != 'LI') { return false; }
  unset($this->data['legal']);
  $this->load_legal_data();
  $cbalance = $this->data['legal']['current_balance'];
  if ($this->data['legal']['current_balance']['total'] == 0) { return false; }
  WEBPAGE::$dbh->query(sprintf("insert into tblLoanWriteOff (id,loan_id,amount,principal,insurance,fees,interest,penalties,user_id,date) values ('','%s','%s','%s','%s','%s','%s','%s','%s','%s')",$this->data['id'],$cbalance['total'],$cbalance['principal'],$cbalance['insurance'],$cbalance['fees'],$cbalance['interest'],$cbalance['penalties'],WEBPAGE::$userID,date('Y-m-d')));
  return true;
  }

  function isFrost()
  {
  switch ($this->data[status]) {
    case "N":  return false;
    case "O":  return false;
    case "R":  return false;
    case "RO": return false;
	case "S":  return false;
	case "A":  return false;
	case "D":  return true;
    case "G":  return true;
	case "LI": return true;
	case "LO": return true;
    case "RT": return true;
    }
  return true;
  }

  function load_legal_data()
  {
  global $_LABELS;

  // payment details
  $this->data['legal']['payments']['regular_payments']                = WEBPAGE::$dbh->getAll(sprintf("select p.date,p.pmt total,p.principal,p.insurances insurance,p.fees,p.interest,p.penalties,0.00 others,u.username from tblPayments p, tblUsers u where p.loan_id = '%s' and u.id = p.user_id order by p.date,p.id",$this->data['id']));
  $this->data['legal']['payments']['parked_payments']                 = WEBPAGE::$dbh->getAll(sprintf("select p.date,p.total,p.principal,p.insurance,p.fees,p.interest,p.penalties,0.00 others,u.username from tblLoansParkedPayments p, tblUsers u where p.loan_id = '%s' and u.id = p.user_id order by p.date,p.id",$this->data['id']));
  $this->data['legal']['payments']['write_off_payments']              = WEBPAGE::$dbh->getAll(sprintf("select p.date,p.amount total,0.00 principal,0.00 insurance,0.00 fees,0.00 interest,0.00 penalties,p.amount others,u.username from tblLoanWriteOffPayments p, tblUsers u where p.loan_id = '%s' and u.id = p.user_id order by p.date,p.id",$this->data['id']));

  // consolidated debits and credits with final balance
  $this->data['legal']['transactions_resume']['regular_debits']       = WEBPAGE::$dbh->getAll(sprintf("select (l.kp+l.kaf+l.kat+(IFNULL(sum(p.interest),0.00)+IFNULL(lp.interest,IFNULL(lwo.interest,0.00)))+(IFNULL(sum(p.penalties),0.00)+IFNULL(lp.penalties,IFNULL(lwo.penalties,0.00)))) total,l.kp principal,l.kaf insurance,l.kat fees,(IFNULL(sum(p.interest),0.00)+IFNULL(lp.interest,IFNULL(lwo.interest,0.00))) interest,(IFNULL(sum(p.penalties),0.00)+IFNULL(lp.penalties,IFNULL(lwo.penalties,0.00))) penalties,0.00 others,'%s' type,'%s' cycle from tblLoans l left join tblPayments p on p.loan_id = l.id left join tblLoansParked lp on lp.loan_id = l.id left join tblLoanWriteOff lwo on lwo.loan_id = l.id where l.id = '%s' group by l.id",$_LABELS['debits'],$_LABELS['regular'],$this->data['id']));

  $this->data['legal']['transactions_resume']['legal_debits']         = WEBPAGE::$dbh->getAll(sprintf("select IFNULL(sum(c.amount),0.00) total,0.00 principal,0.00 insurance,0.00 fees,0.00 interest,0.00 penalties,IFNULL(sum(c.amount),0.00) others,'%s' type,'%s' cycle from tblLoans l left join tblLoanWriteOffCharges c on c.loan_id = l.id where l.id = '%s' group by l.id",$_LABELS['debits'],$_LABELS['legal'],$this->data['id']));
  $this->data['legal']['transactions_resume']['regular_credits']      = WEBPAGE::$dbh->getAll(sprintf("select ((-1)*(IFNULL(sum(p.principal),0.00)+IFNULL(sum(p.insurances),0.00)+IFNULL(sum(p.fees),0.00)+IFNULL(sum(p.interest),0.00)+IFNULL(sum(p.penalties),0.00))) total,((-1)*(IFNULL(sum(p.principal),0.00))) principal,((-1)*(IFNULL(sum(p.insurances),0.00))) insurance,((-1)*(IFNULL(sum(p.fees),0.00))) fees,((-1)*(IFNULL(sum(p.interest),0.00))) interest,((-1)*(IFNULL(sum(p.penalties),0.00))) penalties,0.00 others,'%s' type,'%s' cycle from tblLoans l left join tblPayments p on p.loan_id = l.id where l.id = '%s' group by l.id",$_LABELS['credits'],$_LABELS['regular'],$this->data['id']));
//  $this->data['legal']['transactions_resume']['legal_credits']        = WEBPAGE::$dbh->getAll(sprintf("select ((-1)*(IFNULL(sum(lpp.total),0.00)+IFNULL(sum(lwop.amount),0.00))) total,((-1)*(IFNULL(sum(lpp.principal),0.00))) principal,((-1)*(IFNULL(sum(lpp.insurance),0.00))) insurance,((-1)*(IFNULL(sum(lpp.fees),0.00))) fees,((-1)*(IFNULL(sum(lpp.interest),0.00))) interest,((-1)*(IFNULL(sum(lpp.penalties),0.00))) penalties,((-1)*(IFNULL(sum(lwop.amount),0.00))) others,'%s' type,'%s' cycle from tblLoans l left join tblLoansParkedPayments lpp on lpp.loan_id = l.id left join tblLoanWriteOffPayments lwop on lwop.loan_id = l.id where l.id = '%s' group by l.id",$_LABELS['credits'],$_LABELS['legal'],$this->data['id']));
  $this->data['legal']['transactions_resume']['legal_credits']        = WEBPAGE::$dbh->getAll(sprintf("select (-1)*sum(total) total, (-1)*sum(principal) principal, (-1)*sum(insurance) insurance, (-1)*sum(fees) fees, (-1)*sum(interest) interest, (-1)*sum(penalties) penalties, (-1)*sum(others) others, '%s' type,'%s' cycle from (
                                                                                                                   select IFNULL(sum(lpp.total),0.00) total, IFNULL(sum(lpp.principal),0.00) principal, IFNULL(sum(lpp.insurance),0.00) insurance, IFNULL(sum(lpp.fees),0.00) fees, IFNULL(sum(lpp.interest),0.00) interest, IFNULL(sum(lpp.penalties),0.00) penalties, 0.00 others from tblLoansParkedPayments lpp where lpp.loan_id = '%s'
                                                                                                                   UNION
                                                                                                                   select IFNULL(sum(lwop.amount),0.00) total, 0.00 principal, 0.00 insurance, 0.00 fees, 0.00 interest, 0.00 penalties, IFNULL(sum(lwop.amount),0.00) others from tblLoanWriteOffPayments lwop where lwop.loan_id = '%s' ) bridge",$_LABELS['credits'],$_LABELS['legal'],$this->data['id'],$this->data['id']));

  // legal charges details
  $this->data['legal']['dcharges']['legal_charges']                   = WEBPAGE::$dbh->getAll(sprintf("select c.date,c.amount,c.description,u.username from tblLoanWriteOffCharges c, tblUsers u where c.loan_id = '%s' and u.id = c.user_id order by c.id",$this->data['id']));

  // balance at the beginning of each cycle
  $this->data['legal']['ini_balances']['regular_ini_balance']          = WEBPAGE::$dbh->getAll(sprintf("select (kp+kaf+kat) total,kp principal,kaf insurance,kat fees,0.00 interest,0.00 penalties,delivered_date date,'%s' cycle from tblLoans  where id = '%s'",$_LABELS['regular'],$this->data['id']));
  $this->data['legal']['ini_balances']['parked_ini_balance']           = WEBPAGE::$dbh->getAll(sprintf("select (principal+insurance+fees+interest+penalties) total,principal,insurance,fees,interest,penalties,date,'%s' cycle from tblLoansParked where loan_id = '%s'",$_LABELS['legal'],$this->data['id']));
  $this->data['legal']['ini_balances']['write_off_ini_balance']        = WEBPAGE::$dbh->getAll(sprintf("select amount total,principal,insurance,fees,interest,penalties,date,'%s' cycle from tblLoanWriteOff where loan_id = '%s'",$_LABELS['writeOff'],$this->data['id']));

  //Current balance
  foreach($this->data['legal']['transactions_resume'] as $inx => $data) {
    $data = is_array(current($data)) ? current($data) : array();
    foreach($data as $key=>$val) {
      $this->data['legal']['current_balance'][$key] += $val;
      }
    }

  $cause = current(WEBPAGE::$dbh->getAll(sprintf("select if(lp.category,concat('tblLoansParked.category.',lp.category),if(lwo.id,'delinquency','')) category from tblLoans l left join tblLoansParked lp on lp.loan_id = l.id left join tblLoanWriteOff lwo on lwo.loan_id = l.id where l.id = '%s'",$this->data['id'])));
  $this->data['legal']['cause'] = $_LABELS[$cause['category']];

  $this->data['legal']['writeOff']                                     = current(WEBPAGE::$dbh->getAll(sprintf("select id from tblLoanWriteOff where loan_id = '%s'",$this->data['id'])));
  }
  // PATCH: update 2008.07.16
  function check_xp_delivered_date() {
  	$mdate = new Date();
    $mdate->subtractSeconds(24*60*60);
    if ($this->data['xp_delivered_date'] >= $mdate->format('%Y-%m-%d') && $this->data['check_status'] != 'R' && $this->data['status'] != 'G') {return true;} else {return false;}
  }
  // PATCH: update 2008.07.16
  function check_xp_first_payment_date() {
    global $_CONF;
    if ($this->data['xp_first_payment_date'] <= date('Y-m-d')) return false;
    if ($this->data['check_status'] == 'R') return false;
    if ($this->data['status'] == 'G') return false;
  	if ($_CONF['flex_1st_pmt_date'] == '1') return true;
    if ($this->data['xp_first_payment_date'] > $this->next_payment_date($this->data['xp_delivered_date'])) return false;
    return true;
    //if ($this->data['xp_first_payment_date'] > date('Y-m-d') && $this->data['xp_first_payment_date'] != '0000-00-00' && $this->data['xp_first_payment_date'] <= $this->next_payment_date($this->data['xp_delivered_date']) && $this->data['check_status'] != 'R' && $this->data['status'] != 'G') {return true;} else {return false;}
    }


}
?>