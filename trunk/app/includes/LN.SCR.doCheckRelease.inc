<?php
// being defensive against hacking
// only loan master with all loans in status = 'D' can be released
if (current(current(WEBPAGE::$dbh->getAll(sprintf("select count(l.id) from tblLoans as l, tblLoansMasterDetails as lmd where l.status != 'D' AND lmd.master_id = %s AND lmd.loan_id = l.id",$_REQUEST['id']))))) {
  WEBPAGE::redirect(sprintf("index.php?scr_name=LN.SCR.viewLoanMaster&id=%s&ico=err&msg=check.update.err",$_REQUEST['id']));
  }

require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';

require_once 'Date.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';

$master 	= new LOAN_MASTER($_REQUEST['id']);
$master->loadloans();

$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('releaseForm','POST');

$form->addElement('static', 	'submit', 	'', 			$btn->imgsubmit('releaseForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', 			$btn->imgreset('releaseForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', 			$btn->imglink($PHP_SELF,$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name',					$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',						$_REQUEST['id']);
$form->addElement('hidden',		'min_delivered_date',		$_REQUEST['min_delivered_date']);
$form->addElement('hidden',		'max_first_payment_date',	$_REQUEST['max_first_payment_date']);
$form->addElement('text',		'delivered_date',			$_LABELS['tblLoans.delivered_date'], 'class=large');
$form->addElement('text',		'first_payment',			$_LABELS['tblLoans.first_payment_date'], 'class=large');
$form->addElement('textarea',	'memo',						$_LABELS['memo'], 'rows=5');

//add rules
$form->addRule('delivered_date',	'yyyy-mm-dd','required');
$form->addRule('delivered_date',	'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('delivered_date',	$_LABELS['nonValidDate'],'callback','checkdates_delivered');
$form->addRule('first_payment',		'yyyy-mm-dd','required');
$form->addRule('first_payment',		'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('first_payment',		$_LABELS['nonValidDate'],'callback','checkdates_1stpmt');
$form->addRule('memo',				sprintf($_LABELS['maxlength'],255),'maxlength', 255);

//$form->freeze('delivered_date');

if ($form->validate()) {

  $form->freeze();
  $values = $form->_submitValues;
  $new_loan_status 			= 'G'; //Released
  $new_loan_master_status 	= 'R'; //Released

  //Insert loan status log
  WEBPAGE::$dbh->query(sprintf("insert into tblLoanStatusHistory (loan_id,p_status,status,date,user_id,memo) select l.id,l.status,'%s','%s','%s','%s' from tblLoans as l, tblLoansMasterDetails as lmd where lmd.master_id = %s AND lmd.loan_id = l.id",$new_loan_status,date('Y-m-d'),WEBPAGE::$userID,$values['memo'],$values['id']));
  //update loans status
  WEBPAGE::$dbh->query(sprintf("update tblLoans as l set l.status = '%s',l.delivered_date = '%s',l.first_payment_date = '%s' where l.id in (select lmd.loan_id from tblLoansMasterDetails as lmd where lmd.master_id = %s)",$new_loan_status,$values['delivered_date'],$values['first_payment'],$values['id']));
  //update check status
  WEBPAGE::$dbh->query(sprintf("update tblLoansMaster set check_status = '%s' where id = %s",$new_loan_master_status,$values['id']));
  //insert TDebits data para fines de conciliaciones
  WEBPAGE::$dbh->query(sprintf("insert into tblTDebits (id,code,date,branch_id,program_id,amount) select lm.id,lm.check_number,CURDATE(),lm.zone_id,lm.program_id,lm.amount from tblLoansMaster as lm where lm.id = '%s'",$values['id']));
  //Insert loan current data
  WEBPAGE::$dbh->query(sprintf("insert into tblLoansCurrentData (loan_id,balance_kp,balance_kaf,balance_kat,r_from_date,xp_pmt_date,xp_pmt,cn_date,cn_delay,cn_penalties) select l.id,l.kp,l.kaf,l.kat,l.delivered_date,l.first_payment_date,l.pmt,l.delivered_date,0,0 from tblLoans as l, tblLoanTypes as lt, tblLoansMasterDetails as lmd where lmd.master_id = %s AND lmd.loan_id = l.id AND lt.id = l.loan_type_id",$values['id']));
  //update the loan expected number of payments and cancellation date
  foreach ($master->data['loans'] as $loan_id => $status) {

    $loan = new LOAN($loan_id);
    	$loan->load_xp_pmt_plan();

    $xp_num_pmt = count($loan->data['xp_pmt_plan']);
    $xp_cancel_date = $loan->data['xp_pmt_plan'][$xp_num_pmt-1]['pmt_date'];
    WEBPAGE::$dbh->query(sprintf("update tblLoans as l set l.xp_num_pmt = '%s',l.xp_cancel_date = '%s' where l.id = %s",$xp_num_pmt,$xp_cancel_date,$loan_id));
    }

  //redirect page
  WEBPAGE::redirect(sprintf("index.php?scr_name=LN.SCR.viewLoanMaster&id=%s&ico=scs&msg=scs",$values['id']));

  }

// print some data before rendering the form
$master->loadBorrowerData();
$loan = new LOAN(0,$master->data['loan_type_id']);


$tpl->setVariable('zone_label',				$_LABELS['tblZones.zone']);
$tpl->setVariable('zone',					$master->data['borrower']->data['zone']);
$tpl->setVariable('borrower_label',			$_LABELS['tblLoansMaster.borrower_id']);
$tpl->setVariable('borrower',				$master->data['borrower']->data['name']);
$tpl->setVariable('borrower_type_label',	$_LABELS['tblLoansMaster.borrower_type']);
$tpl->setVariable('borrower_type',			$_LABELS[sprintf('tblLoanTypes.borrower_type.%s',$master->data['borrower_type'])]);
$tpl->setVariable('loan_type_label',		$_LABELS['tblLoansMaster.loan_type_id']);
$tpl->setVariable('loan_type',				$loan->data['modality']);
$tpl->setVariable('amount_label',			$_LABELS['tblLoansMaster.amount']);
$tpl->setVariable('amount',					$master->data['amount']);
$tpl->setVariable('check_number_label',		$_LABELS['tblLoansMaster.check_number']);
$tpl->setVariable('check_number',			$master->data['check_number']);
$tpl->setVariable('check_status_label',		$_LABELS['tblLoansMaster.check_status']);
$tpl->setVariable('check_status',			$_LABELS[sprintf('tblLoansMaster.check_status.%s',$master->data['check_status'])]);

$tpl->setVariable('chart_title',			$_LABELS['details']);

//print sub-loans chart
$head 	= array($_LABELS['tblLoans.loan_code'],$_LABELS['tblLoans.kp'],$_LABELS['tblClients.id'],$_LABELS['status'],$_LABELS['details']);
$align 	= array('loan_code'=>'left','kp'=>'right','borrower'=>'left','status'=>'left','details'=>'center');
$mrow 	= WEBPAGE::$dbh->getAll(sprintf('select l.loan_code,l.kp,l.client_id,l.status,lmd.loan_id from tblLoans AS l, tblLoansMasterDetails AS lmd where lmd.master_id = %s AND l.id = lmd.loan_id',$_REQUEST['id']));

$tpl->addBlockfile('chart','chart','ST.chart.tpl');
foreach($head as $key=>$val) {
  $tpl->setCurrentBlock("header") ;
  $tpl->setVariable('column_name', $val);
  $tpl->parseCurrentBlock("header") ;
  }

foreach($mrow AS $key=>$val) {
  $loan->load_borrower($val['client_id']);

  $data[$key]['loan_code'] 	= $val['loan_code'];
  $data[$key]['kp'] 		= $val['kp'];
  $data[$key]['borrower'] 	= $loan->data['borrower_name'];
  $data[$key]['status'] 	= $_LABELS[sprintf('tblLoans.status.%s',$val['status'])];
  $data[$key]['details']	= sprintf('<a href=\'javascript:openWin("index.popup.php?scr_name=LN.RPT.loanStatusHistory&id=%s","LoanIOM","menubar=no,scrollbars=yes,resizable=yes,width=700,height=320")\'>%s</a>' ,$val['loan_id'],$_LABELS['details']);
//  $data[$key]['view'] 		= sprintf("<a href='%s?scr_name=LN.SCR.viewILoan&id=%s'>%s</a>", $PHP_SELF,$val['loan_id'],$_LABELS['view']);
//  $data[$key]['edit'] 		= sprintf("<a href='%s?scr_name=LN.SCR.addILoan&id=%s'>%s</a>", $PHP_SELF,$val['loan_id'],$_LABELS['edit']);
//  $data[$key]['delete'] 	= sprintf("<a href='%s?scr_name=LN.SCR.deleteILoan&id=%s'>%s</a>", $PHP_SELF,$val['loan_id'],$_LABELS['delete']);
  }

foreach($data as $key=>$row) {
  foreach($row as $col => $val) {
    $tpl->setCurrentBlock("row") ;
    $tpl->setVariable('align', $align[$col]);
    $tpl->setVariable('item', $val);
    $tpl->parseCurrentBlock("row") ;
    }
  $tpl->setCurrentBlock("results") ;
  $tpl->parseCurrentBlock("results") ;
  }
$tpl->setCurrentBlock("chart") ;
$tpl->parseCurrentBlock("chart") ;

//print_r($loan); exit;

//minimum delivered date
$mdate = new Date();
$mdate->subtractSeconds(24*60*60);

$ddate = new Date();

// PATCH: 2008.08.12
if (($master->data['xp_delivered_date'] <= $ddate->format('%Y-%m-%d')) && ($master->data['xp_delivered_date'] >= $mdate->format('%Y-%m-%d'))) {
	$def_delivered_date = $master->data['xp_delivered_date'];
    } else {
	$def_delivered_date = $ddate->format('%Y-%m-%d');
    }

// PATCH: 2008.08.12
if ($_CONF['flex_1st_pmt_date'] == '1') {
  $fdate = ($master->data['xp_first_payment_date'] > $def_delivered_date ) ? new Date($master->data['xp_first_payment_date']) : new Date($loan->next_payment_date($def_delivered_date));
  } else {
  $fdate = ($master->data['xp_first_payment_date'] <= $loan->next_payment_date($def_delivered_date)) ? new Date($master->data['xp_first_payment_date']) : new Date($loan->next_payment_date($def_delivered_date));
  }

//print the form
$form->setDefaults(array('max_first_payment_date'=>$fdate->format('%Y-%m-%d'),'min_delivered_date'=>$mdate->format('%Y-%m-%d'),'delivered_date'=>$def_delivered_date,'first_payment'=>$fdate->format('%Y-%m-%d')));
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

function checkdates_1stpmt() {
  global $_CONF;
  $date_1stpmt = explode('-',$_REQUEST['first_payment']);
  if (!checkdate($date_1stpmt[1],$date_1stpmt[2],$date_1stpmt[0])) { return false; } //verifica que es una fecha real
  if ($_REQUEST['first_payment'] <= $_REQUEST['delivered_date']) { return false; }
  if ($_CONF['flex_1st_pmt_date'] == '1') return true;
  if (date('w',date(mktime(0,0,0,$date_1stpmt[1],$date_1stpmt[2],$date_1stpmt[0]))) == 0) { return false; } // verifica que no sea domingo
  if (date('w',date(mktime(0,0,0,$date_1stpmt[1],$date_1stpmt[2],$date_1stpmt[0]))) == 6) { return false; } // verifica que no sea sabado
  if ($_REQUEST['first_payment'] > $_REQUEST['max_first_payment_date']) { return false; }
  return true;
  }

function checkdates_delivered() {
  $date_delivered = explode('-',$_REQUEST['delivered_date']);
  if (!checkdate($date_delivered[1],$date_delivered[2],$date_delivered[0])) { return false; } //verifica que es una fecha real
  if (date('w',date(mktime(0,0,0,$date_delivered[1],$date_delivered[2],$date_delivered[0]))) == 0) { return false; } // verifica que no sea domingo
  if (date('w',date(mktime(0,0,0,$date_delivered[1],$date_delivered[2],$date_delivered[0]))) == 6) { return false; } // verifica que no sea sabado
  if ($_REQUEST['delivered_date'] > date('Y-m-d')) { return false; }
  if ($_REQUEST['delivered_date'] < $_REQUEST['min_delivered_date']) { return false; }
  return true;
  }

?>
