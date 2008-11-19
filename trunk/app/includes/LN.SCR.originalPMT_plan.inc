<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/receipt.php';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/webpage.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';
require_once 'Date.php';


$master 	= new LOAN_MASTER($_REQUEST['id']);

$master->loadloans();
$master->loadextrainfo();



$c = 1;
foreach($master->data['extrainfo'] as $loan_id => $data) {

$loan = new LOAN($loan_id);
  

$loan->load_xp_pmt_plan();
  
  $number_of_pmt = count($loan->data['xp_pmt_plan']);
  
  $client_data = WEBPAGE::$dbh->getRow(sprintf("select concat(c.first,' ',c.last) as cname, c.society_id from tblClients c where c.id = %s limit 1;",$loan->data['client_id']));
  
  $plan_data[$c]['clients'] = $client_data['cname'];

  $plan_data[$c]['sname'] = WEBPAGE::$dbh->getOne(sprintf("select s.name from tblSocieties s where s.id = %s limit 1;",$client_data['society_id']));
    
  $plan_data[$c]['kp'] = $loan->data['kp'];
  $plan_data[$c]['fees'] = $loan->data['xp_pmt_plan_totals']['fees'];
  $plan_data[$c]['insurances'] = $loan->data['xp_pmt_plan_totals']['insurances'];
  $plan_data[$c]['principal'] = $loan->data['xp_pmt_plan_totals']['principal'];
  $plan_data[$c]['interest'] = $loan->data['xp_pmt_plan_totals']['interest'];
  $plan_data[$c]['pmt'] = $loan->data['xp_pmt_plan_totals']['pmt'];
  $plan_data[$c]['first_pmt'] = $loan->data['pmt'];
  $plan_data[$c]['pmt_last'] = $loan->data['xp_pmt_plan'][$number_of_pmt - 1]['pmt'];
  
  $plan_total['client'] = '-';
  $plan_total['sname'] = '-';
  $plan_total['kp'] += $loan->data['kp'];
  $plan_total['fees'] += $loan->data['xp_pmt_plan_totals']['fees'];
  $plan_total['insurances'] += $loan->data['xp_pmt_plan_totals']['insurances'];
  $plan_total['principal'] += $loan->data['xp_pmt_plan_totals']['principal'];
  $plan_total['interest'] += $loan->data['xp_pmt_plan_totals']['interest'];
  $plan_total['pmt'] += $loan->data['xp_pmt_plan_totals']['pmt'];
  $plan_total['first_pmt'] += $loan->data['pmt'];
  $plan_total['pmt_last'] += $loan->data['xp_pmt_plan'][$number_of_pmt - 1]['pmt'];
  
  $c++;
  }

 
  
$society_name = $plan_data[1]['sname']; 
  

if (count($plan_data)) { $plan_data['total'] = $plan_total; }

$head = array('clients'=>$_LABELS['clients'],'sname' => $_LABELS['group_b'],'kp'=>$_LABELS['tblLoans.kp'],'fees' => $_LABELS['FEES'],'insurances' =>$_LABELS['insurance'],'principal' => $_LABELS['principal'],'interest'=>$_LABELS['interest'],'pmt'=>$_LABELS['PMT'],'first_pmt'=>$_LABELS['pmt_first'],'pmt_last'=>$_LABELS['pmt_last'] );

$tpl->setVariable('rpt_label', 			$_LABELS['LN.SCR.originalPMT_plan']);
//$tpl->setVariable('rpt_subtitle_label', $society_name);
$tpl->setVariable('society_label', $society_name);
$tpl->setVariable('data_date_label', 	$_LABELS['tblLoans.delivered_date']);
$tpl->setVariable('data_date', 			$loan->data['delivered_date']);
$tpl->setVariable('chart', count($plan_data) ? WEBPAGE::printchart($plan_data,$head) : $_LABELS['noData']);

?>
