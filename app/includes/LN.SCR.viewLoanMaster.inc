<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';
// PATCH: 2008.07.21
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';


$master 	= new LOAN_MASTER($_REQUEST['id']);
$loan 		= new LOAN(0,$master->data['loan_type_id']);


$master->loadloans();
$master->loadBorrowerData();

//( count($master->data['loans']) == 1 ) ? WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.viewILoan&id=%s',key($master->data['loans']))) : '';

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
// PATCH: 2008.07.21
$tpl->setVariable('master_id',			$master->data['id']);
$tpl->setVariable('xp_dates',			$_LABELS['xp_dates']);
if ($master->data['cancelled']) { $tpl->setVariable('lnk_cancellation_letter', sprintf('<a href="index.popup.php?scr_name=LN.SCR.viewCancelLetter&id=%s" target="_blank"><img alt="%s" src="button.php?txt=%s"></a>',$_REQUEST['id'],$_LABELS['cancellation_letter'],$_LABELS['cancellation_letter'])); }

if (($master->data['check_status'] == 'D')&&($_CONF['contract_promissory_note'] == 1)) {
  $tpl->setVariable('lnk_promissory_note', sprintf('<a href="index.popup.php?scr_name=LN.SCR.promissoryNote&id=%s" target="_blank"><img alt="%s" src="button.php?txt=%s"></a>',$_REQUEST['id'],$_LABELS['promissory_note'],$_LABELS['promissory_note']));
  switch($master->data['borrower_type']) {
    case 'B':
	  $tpl->setVariable('lnk_contract', sprintf('<a href=javascript:openWin("index.popup.php?scr_name=LN.SCR.groupContract&id=%s","groupContract","menubar=no,scrollbars=yes,resizable=yes,width=800,height=320")>%s</a>',$_REQUEST['id'],$_LABELS['loan_contract']));
	  break;
    default:
	  $tpl->setVariable('lnk_contract', sprintf('<a href=javascript:openWin("index.popup.php?scr_name=LN.SCR.individualContract&id=%s","individualContract","menubar=no,scrollbars=yes,resizable=yes,width=800,height=320")>%s</a>',$_REQUEST['id'],$_LABELS['loan_contract']));
    }
  }



// PATCH: 2008.07.21

if ($_CONF['contract_select'] == 1 && $master->data['check_status'] != 'R'){

    $contracts = WEBPAGE::$dbh->getAssoc(sprintf("select cn.tpl,cn.name from tblContracts cn where 1;"));

    $form = new HTML_QuickForm('showContract','GET','index.popup.php','_blank');

    $btn  = new TTFButton(WEBPAGE::$theme);

    $form->addElement('static', 	'submit', 	'', $btn->imgsubmit('showContract',$_LABELS['printSheet']));
    $form->addElement('select',		'tpl',	'',	(array('-'=>'--- '.$_LABELS['contract_select']) + $contracts),		'style="width:300px"');
    $form->addElement('hidden',		'id',			$_REQUEST['id']);
    $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
    $form->accept($renderer);

}

// PATCH: 2008.07.20
if ($_CONF['original_pmt_plan'] == 1) {
    //$tpl->setVariable('lnk_original_payment_plan', sprintf('<a href=javascript:openWin("index.popup.php?scr_name=LN.SCR.originalPMT_plan&id=%s","LN.SCR.originalPMT_plan","menubar=no,scrollbars=yes,resizable=yes,width=800,height=320")>%s</a>',$_REQUEST['id'],$_LABELS['LN.SCR.originalPMT_plan']));
    $tpl->setVariable('lnk_original_payment_plan', sprintf('<a href="index.popup.php?scr_name=LN.SCR.originalPMT_plan&id=%s" target="_blank"><img alt="%s" src="button.php?txt=%s"></a>',$_REQUEST['id'],$_LABELS['LN.SCR.originalPMT_plan'],$_LABELS['LN.SCR.originalPMT_plan']));
    }

$tpl->setVariable('chart_title',			$_LABELS['details']);

//print sub-loans chart
$head 	= array($_LABELS['tblLoans.loan_code'],$_LABELS['tblLoans.kp'],$_LABELS['tblClients.id'],$_LABELS['status'],$_LABELS['view'],$_LABELS['edit'], $_LABELS['delete']);
$align 	= array('loan_code'=>'left','kp'=>'right','borrower'=>'left','status'=>'left','view'=>'center','edit'=>'center','delete'=>'center');
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
  $data[$key]['view'] 		= sprintf("<a href='%s?scr_name=LN.SCR.viewILoan&id=%s'>%s</a>", $PHP_SELF,$val['loan_id'],$_LABELS['view']);
  $data[$key]['edit'] 		= sprintf("<a href='%s?scr_name=LN.SCR.addILoan&id=%s'>%s</a>", $PHP_SELF,$val['loan_id'],$_LABELS['edit']);
  $data[$key]['delete'] 	= sprintf("<a href='%s?scr_name=LN.SCR.deleteILoan&id=%s'>%s</a>", $PHP_SELF,$val['loan_id'],$_LABELS['delete']);
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

?>
