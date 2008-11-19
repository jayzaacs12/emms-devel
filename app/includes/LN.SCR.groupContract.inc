<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/receipt.php';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';
require_once 'Date.php';

$master 	= new LOAN_MASTER($_REQUEST['id']);

$master->loadloans();
$master->loadextrainfo();

// PATCH: 2008.07.21
if ($_REQUEST['tpl'] == '-'){
    printf("<h1 style=\"color:red;\">%s</h1>",$_LABELS['contract_error']);
exit;
}

// PATCH: 2008.07.16
if (file_exists(sprintf("templates/%s.tpl",$_REQUEST['tpl']))) {

    $tpl->loadTemplateFile(sprintf("%s.tpl",$_REQUEST['tpl']));

}

$c = 1;
foreach($master->data['extrainfo'] as $loan_id => $data) {

  $loan = new LOAN($loan_id);
  $loan->load_xp_pmt_plan();  //echo '<pre>'; print_r($loan); exit;

  $number_of_pmt = count($loan->data['xp_pmt_plan']);
  $total_amount += $loan->data['xp_pmt_plan_totals']['pmt'];
  $pmt_last		+= $loan->data['xp_pmt_plan'][$number_of_pmt - 1]['pmt'];

  $tpl->setVariable(sprintf('client_%s',$c),   $data['client_name']);
  $tpl->setVariable(sprintf('code_%s',$c),     $data['client_code']);
  $tpl->setVariable(sprintf('address_%s',$c),  $data['client_address']);

  $pmt_amount += $data['pmt'];
  $payment_frequency = $data['payment_frequency'];

  $c++;
  }

$master->loadBorrowerData();

$tpl->setVariable('master_id',		$master->data['id']);
$tpl->setVariable('borrower',		$master->data['borrower']->data['name']);
$tpl->setVariable('kp',		        number_format($master->data['amount'], 2, '.', ','));
$tpl->setVariable('pmt_amount',		number_format($pmt_amount, 2, '.', ','));
$tpl->setVariable('payment_frequency',	$_LABELS[sprintf('tblLoanTypes.payment_frequency.%s',$payment_frequency)]);

$tpl->setVariable('num_payments',	$number_of_pmt);
$tpl->setVariable('pmt_last',		number_format($pmt_last, 2, '.', ','));
$tpl->setVariable('total_amount',	number_format($total_amount, 2, '.', ','));

// PATCH: 2008.07.16
$date_to_print = $loan->check_xp_delivered_date() ? $master->data['xp_delivered_date'] : date('Y-m-d');

// PATCH: 2008.07.16
//$tpl->setVariable('date',			WEBPAGE::verbose_date_format(date('Y-m-d')));
$tpl->setVariable('date',			WEBPAGE::verbose_date_format($date_to_print));

$tpl->setVariable('fdate',			WEBPAGE::verbose_date_format($master->data['xp_first_payment_date']));


?>
