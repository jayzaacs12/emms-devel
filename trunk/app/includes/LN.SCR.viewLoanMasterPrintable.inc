<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';

$master 	= new LOAN_MASTER($_REQUEST['id']);   
$loan 		= new LOAN(0,$master->data['loan_type_id']);

$master->loadloans();
$master->loadBorrowerData();

//( count($master->data['loans']) == 1 ) ? WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.viewILoan&id=%s',key($master->data['loans']))) : ''; 

$tpl->setVariable('page_title',				$_LABELS['LN.SCR.viewLoanMaster']);
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

$tpl->setVariable('date',					date('Y-m-d'));
$tpl->setVariable('print',					sprintf('<br><a href=javascript:print()>%s</a>',$_LABELS['printSheet']));
  
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
  $data[$key]['details']	= sprintf('<a href=\'javascript:openWin("index.popup.php?scr_name=LN.RPT.loanStatusHistory&ref=LN.SCR.viewILoan&id=%s","LoanIOM","menubar=no,scrollbars=yes,resizable=yes,width=700,height=320")\'>%s</a>' ,$val['loan_id'],$_LABELS['details']);
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