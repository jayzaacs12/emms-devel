<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/receipt.php';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';

$receipt 	= new RECEIPT($_REQUEST['id']);
$master 	= new LOAN_MASTER($receipt->data['loanmaster_id']);
$loan 		= new LOAN(0,$master->data['loan_type_id']);

$receipt->loadPayments();
$master->loadloans();
$master->loadBorrowerData();



//print sub-payments chart
$head 	= array($_LABELS['tblClients.id'],$_LABELS['total'],$_LABELS['KP'],$_LABELS['FEES'],$_LABELS['INS'],$_LABELS['RT'],$_LABELS['PN'],$_LABELS['memo']);

$tpl->addBlockfile('chart','chart','ST.chart.tpl');
foreach($head as $key=>$val) {
  $tpl->setCurrentBlock("header") ;
  $tpl->setVariable('column_name', $val);
  $tpl->parseCurrentBlock("header") ;
  }

foreach($receipt->data['payments']['rollback'] AS $key=>$val) {
  $data[$key]['client'] 	= $val['client'];
  $data[$key]['pmt'] 	    = '<strike>&nbsp;'.number_format($val['pmt'], 2, '.', ',').'&nbsp;</strike>';
  $data[$key]['principal'] 	= '<strike>&nbsp;'.number_format($val['principal'], 2, '.', ',').'&nbsp;</strike>';
  $data[$key]['fees'] 	    = '<strike>&nbsp;'.number_format($val['fees'], 2, '.', ',').'&nbsp;</strike>';
  $data[$key]['insurances'] = '<strike>&nbsp;'.number_format($val['insurances'], 2, '.', ',').'&nbsp;</strike>';
  $data[$key]['interest'] 	= '<strike>&nbsp;'.number_format($val['interest'], 2, '.', ',').'&nbsp;</strike>';
  $data[$key]['penalties'] 	= '<strike>&nbsp;'.number_format($val['penalties'], 2, '.', ',').'&nbsp;</strike>';
  $data[$key]['note'] 	    = $_LABELS['annulled'];

  $balance_kp_rollback		+= $val['principal'];
  $balance_kaf_rollback		+= $val['insurances'];
  $balance_kat_rollback		+= $val['fees'];
  }

foreach($receipt->data['payments']['normal'] AS $key=>$val) {
//  $data[$key]['loan_code'] 	= $val['loan_code'];
  $data[$key]['client'] 	= $val['client'];
  $data[$key]['pmt'] 	    = number_format($val['pmt'], 2, '.', ',');
  $data[$key]['principal'] 	= number_format($val['principal'], 2, '.', ',');
  $data[$key]['fees'] 	    = number_format($val['fees'], 2, '.', ',');
  $data[$key]['insurances'] = number_format($val['insurances'], 2, '.', ',');
  $data[$key]['interest'] 	= number_format($val['interest'], 2, '.', ',');
  $data[$key]['penalties'] 	= number_format($val['penalties'], 2, '.', ',');
//  $data[$key]['balance'] 	= $val['balance_kp'] + $val['balance_kat'] + $val['balance_kaf'];
  $data[$key]['note'] 	    = '-';

  $total 		+= $val['pmt'];
  $principal 	+= $val['principal'];
  $fees 		+= $val['fees'];
  $insurances 	+= $val['insurances'];
  $interest 	+= $val['interest'];
  $penalties 	+= $val['penalties'];
  $delay 		+= $val['delay'];
  }

//$data['total']['loan_code'] 	= '-';
$data['total']['client'] 	    = '<b>'.$_LABELS['total'].'</b>';
$data['total']['pmt'] 	        = '<b>'.number_format($total, 2, '.', ',').'</b>';
$data['total']['principal'] 	= '<b>'.number_format($principal, 2, '.', ',').'</b>';
$data['total']['fees'] 	        = '<b>'.number_format($fees, 2, '.', ',').'</b>';
$data['total']['insurances']    = '<b>'.number_format($insurances, 2, '.', ',').'</b>';
$data['total']['interest'] 	    = '<b>'.number_format($interest, 2, '.', ',').'</b>';
$data['total']['penalties'] 	= '<b>'.number_format($penalties, 2, '.', ',').'</b>';
$data['total']['note'] 	        = '<b>'.'-'.'</b>';


$delay = round($delay/max(1,count($receipt->data['payments']['normal'])),0);

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

$tpl->setVariable('page_title',			$_LABELS['LN.SCR.paymentReceipt']);
$tpl->setVariable('paymentReceipt',		sprintf($_LABELS['paymentReceipt'],$master->data['borrower']->data['name'],number_format($total, 2, '.', ',')));

$tpl->setVariable('receipt_number',		        $receipt->data['id']);
$tpl->setVariable('receipt_number_label',		$_LABELS['code']);

$tpl->setVariable('createdByOn',		sprintf($_LABELS['createdByOn'],$receipt->data['username'],$receipt->data['date']));

$tpl->setVariable('loanmaster_id',		        $receipt->data['loanmaster_id']);
$tpl->setVariable('loanmaster_id_label',		$_LABELS['tblLoans.id']);


$tpl->setVariable('delay',		        $delay);
$tpl->setVariable('delay_label',		$_LABELS['delay']);
$tpl->setVariable('balance',		    number_format($receipt->data['balance_kp'] + $receipt->data['balance_kat'] + $receipt->data['balance_kaf'] + $balance_kp_rollback + $balance_kaf_rollback + $balance_kat_rollback, 2, '.', ','));
$tpl->setVariable('balance_label',		$_LABELS['current_balance']);
$tpl->setVariable('program',		    $master->data['borrower']->data['zone']);
$tpl->setVariable('program_label',		$_LABELS['tblPrograms.program']);
$tpl->setVariable('advisor',		    $master->data['borrower']->data['advisor']);
$tpl->setVariable('advisor_label',		$_LABELS['tblClients.advisor_id']);
$tpl->setVariable('details_label', 		$_LABELS['details']);
$tpl->setVariable('memo',		        $receipt->data['notes'] ? $receipt->data['notes'] : '-');
$tpl->setVariable('memo_label', 		$_LABELS['memo']);
$tpl->setVariable('print', 				sprintf("<a href='javascript:print()'>%s</a>",$_LABELS['printSheet']));

$pmt_receipt_flag_all = array();
if ($receipt->data['flag_a']) { $pmt_receipt_flag_all['a'] = $_CONF['pmt_receipt_flag_a']; }
if ($receipt->data['flag_b']) { $pmt_receipt_flag_all['b'] = $_CONF['pmt_receipt_flag_b']; }

$tpl->setVariable('pmt_receipt_flag_all', implode('<br />',$pmt_receipt_flag_all));

?>
