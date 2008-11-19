<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/receipt.php';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';

$master 	= new LOAN_MASTER($_REQUEST['id']);
$loan 		= new LOAN(0,$master->data['loan_type_id']);

$master->loadloans();
if (!$master->data['cancelled']) { exit; }

$master->loadpayments();
$master->load_pmt_receipt_flag_a();
$master->load_pmt_receipt_flag_b();
$master->loadBorrowerData();



//print sub-payments chart
$head 	= array($_LABELS['tblClients.id'],$_LABELS['total'],$_LABELS['KP'],$_LABELS['FEES'],$_LABELS['INS'],$_LABELS['RT'],$_LABELS['PN']);
if ($_CONF['pmt_receipt_flag_a'])  { $head[] = $_CONF['pmt_receipt_flag_a']; }
if ($_CONF['pmt_receipt_flag_b'])  { $head[] = $_CONF['pmt_receipt_flag_b']; }

$tpl->addBlockfile('chart','chart','ST.chart.tpl');
foreach($head as $key=>$val) {
  $tpl->setCurrentBlock("header") ;
  $tpl->setVariable('column_name', $val);
  $tpl->parseCurrentBlock("header") ;
  }

foreach($master->data['payments'] AS $key=>$val) {

  $data[$key]['client'] 	= $val['client'];
  $data[$key]['pmt'] 	    = number_format($val['pmt'], 2, '.', ',');
  $data[$key]['principal'] 	= number_format($val['principal'], 2, '.', ',');
  $data[$key]['fees'] 	    = number_format($val['fees'], 2, '.', ',');
  $data[$key]['insurances'] = number_format($val['insurances'], 2, '.', ',');
  $data[$key]['interest'] 	= number_format($val['interest'], 2, '.', ',');
  $data[$key]['penalties'] 	= number_format($val['penalties'], 2, '.', ',');

  if ($_CONF['pmt_receipt_flag_a'])  {
  	$data[$key]['pmt_receipt_flag_a']   = $master->data['pmt_receipt_flag_a'][$key] ? $master->data['pmt_receipt_flag_a'][$key] : 0;
  	$pmt_receipt_flag_a     += $master->data['pmt_receipt_flag_a'][$key];
    }

  if ($_CONF['pmt_receipt_flag_b'])  {
  	$data[$key]['pmt_receipt_flag_b']   = $master->data['pmt_receipt_flag_b'][$key] ? $master->data['pmt_receipt_flag_b'][$key] : 0;
  	$pmt_receipt_flag_b     += $master->data['pmt_receipt_flag_b'][$key];
    }

  $total 		+= $val['pmt'];
  $principal 	+= $val['principal'];
  $fees 		+= $val['fees'];
  $insurances 	+= $val['insurances'];
  $interest 	+= $val['interest'];
  $penalties 	+= $val['penalties'];

  }

$data['total']['client'] 	    = '<b>'.$_LABELS['total'].'</b>';
$data['total']['pmt'] 	        = '<b>'.number_format($total, 2, '.', ',').'</b>';
$data['total']['principal'] 	= '<b>'.number_format($principal, 2, '.', ',').'</b>';
$data['total']['fees'] 	        = '<b>'.number_format($fees, 2, '.', ',').'</b>';
$data['total']['insurances']    = '<b>'.number_format($insurances, 2, '.', ',').'</b>';
$data['total']['interest'] 	    = '<b>'.number_format($interest, 2, '.', ',').'</b>';
$data['total']['penalties'] 	= '<b>'.number_format($penalties, 2, '.', ',').'</b>';

if ($_CONF['pmt_receipt_flag_a'])  {
  $data['total']['pmt_receipt_flag_a']    = '<b>'.$pmt_receipt_flag_a.'</b>';
  }

if ($_CONF['pmt_receipt_flag_b'])  {
  $data['total']['pmt_receipt_flag_b']    = '<b>'.$pmt_receipt_flag_b.'</b>';
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

$tpl->setVariable('organization',		$_CONF['client_name']);
$tpl->setVariable('address',			$_CONF['client_address']);

$tpl->setVariable('page_title',			$_LABELS['LN.SCR.viewCancelLetter']);
$tpl->setVariable('cancelLetter',		sprintf($_LABELS['cancelLetter'],$master->data['borrower']->data['name'],number_format($total, 2, '.', ',')));

$tpl->setVariable('loanmaster_id',		        $master->data['id']);
$tpl->setVariable('loanmaster_id_label',		$_LABELS['tblLoans.id']);

$tpl->setVariable('program',		    $master->data['borrower']->data['zone']);
$tpl->setVariable('program_label',		$_LABELS['tblPrograms.program']);
$tpl->setVariable('advisor',		    $master->data['borrower']->data['advisor']);
$tpl->setVariable('advisor_label',		$_LABELS['tblClients.advisor_id']);
$tpl->setVariable('details_label', 		$_LABELS['details']);

$tpl->setVariable('print', 				sprintf("<a href='javascript:print()'>%s</a>",$_LABELS['printSheet']));



?>
