<?php
//echo mktime();
//echo ': INIT<br>';
// being defensive against hacking
// only loan master with all loans in status = 'A' can be approved
if (current(current(WEBPAGE::$dbh->getAll(sprintf("select count(l.id) from tblLoans as l, tblLoansMasterDetails as lmd where l.status != 'A' AND lmd.master_id = %s AND lmd.loan_id = l.id",$_REQUEST['id']))))) {
  WEBPAGE::redirect(sprintf("index.php?scr_name=LN.SCR.viewLoanMaster&id=%s&ico=err&msg=check.update.err",$_REQUEST['id']));
  }
//echo mktime();
//echo ': Hacker prevention<br>';

require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/loan_master.php';
require_once 'class/program.php';
require_once './mod_sponsorship/class/sponsor.php';

$master 	= new LOAN_MASTER($_REQUEST['id']);   
//echo mktime();
//echo 'LOAN MASTER<br>';
$program 	= new PROGRAM($master->data['program_id']);
//echo mktime();
//echo 'PROGRAM<br>';
$sponsor 	= new SPONSOR();
//echo mktime();
//echo 'SPONSOR<br>';

$btn  		= new TTFButton(WEBPAGE::$theme);

$form 		= new HTML_QuickForm('disbursementForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('disbursementForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	  '', $btn->imgreset('disbursementForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name',		  $_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			      $_REQUEST['id']);
$form->addElement('text',		  'check_number',	$_LABELS['tblLoansMaster.check_number'], 'class=large');
$form->addElement('select',		'fund',			    $_LABELS['tblFunds.id'],		             (array('-'=>'') + $program->getFunds()), 'class=large');
$form->addElement('select',		'sponsor_id',	  $_LABELS['tblSponsors.sponsor'],		     (array(''=>'') + array_intersect_key($sponsor->getSponsorsByBalance($master->data['amount']),$sponsor->getSponsorsByZone($master->getBorrowerZone()))), 'class=large');
$form->addElement('textarea',	'memo',			$_LABELS['memo'], 'rows=5');

//add rules
$form->addRule('check_number',	sprintf($_LABELS['rangelength'],1,32),'required');
$form->addRule('fund',			$_LABELS['selectOption'],'required');
$form->addRule('fund',			$_LABELS['selectOption'],'alphanumeric');
$form->addRule('memo',			sprintf($_LABELS['maxlength'],255),'maxlength', 255);

if ($form->validate()) { 
  
  $form->freeze();  
  $values = $form->_submitValues;
  $new_loan_status 			= 'D'; //Disbursed
  $new_loan_master_status 	= 'D'; //Disbursed
	
  //insert new record at tblFundsLoansMasterPct
  SQL::insert('tblFundsLoansMasterPct',array('id'=>'Null','master_id'=>$values['id'],'fund_id'=>$values['fund'],'pct'=>'100'));
  //Insert loan status log
  WEBPAGE::$dbh->query(sprintf("insert into tblLoanStatusHistory (loan_id,p_status,status,date,user_id,memo) select l.id,l.status,'%s','%s','%s','%s' from tblLoans as l, tblLoansMasterDetails as lmd where lmd.master_id = %s AND lmd.loan_id = l.id",$new_loan_status,date('Y-m-d'),WEBPAGE::$userID,$values['memo'],$values['id']));
  //update loans status
  WEBPAGE::$dbh->query(sprintf("update tblLoans as l set l.status = '%s' where l.id in (select lmd.loan_id from tblLoansMasterDetails as lmd where lmd.master_id = %s)",$new_loan_status,$values['id']));
  //update check status
  WEBPAGE::$dbh->query(sprintf("update tblLoansMaster set check_status = '%s', check_number = '%s', sponsor_id = '%s' where id = %s",$new_loan_master_status,$values['check_number'],$values['sponsor_id'],$values['id']));
  //redirect page
  WEBPAGE::redirect(sprintf("index.php?scr_name=LN.SCR.viewLoanMaster&id=%s&ico=scs&msg=scs",$values['id']));

  } 

// print some data before rendering the form
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';

$loan 		= new LOAN(0,$master->data['loan_type_id']);
//echo mktime();
//echo 'NEW LOAN<br>';

$master->loadloans();
$master->loadBorrowerData();

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

//print the form
$form->setDefaults();
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>

