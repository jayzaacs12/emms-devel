<?php
$date_from 	= $_REQUEST['date_from'] 	? $_REQUEST['date_from'] 	: date('Y-m-d');
$date_to 	= $_REQUEST['date_to'] 		? $_REQUEST['date_to'] 		: date('Y-m-d');
$tpl->setVariable('dateRangeSelector', 	$_LABELS['BS.SCR.dateRangeSelector']);
$tpl->setVariable('oldrange', 			sprintf('%s - %s',$date_from,$date_to));

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select l.zone_id, z.zone as zone, lt.borrower_type as type,
		 		sum(p.pmt) as pmt, sum(p.principal) as principal, sum(p.insurances) as insurances, sum(p.fees) as fees, sum(p.interest) as interest, sum(p.penalties) as penalties
		 		from tblPayments as p, tblLoans as l, tblZones as z, tblLoanTypes as lt where l.id = p.loan_id and z.id = l.zone_id and lt.id = l.loan_type_id and p.date >= '%s' and p.date <= '%s' group by l.zone_id, type", $date_from, $date_to));
$total = array();
foreach($data as $key=>$val) {
	
  $ldata[$val['zone_id']]['zone'] 	 	= str_replace(' ','&nbsp;',$val['zone']);
  
  $ldata[$val['zone_id']]['pmt'] 			+= $val['pmt'];
  $ldata[$val['zone_id']]['principal'] 		+= $val['principal'];
  $ldata[$val['zone_id']]['insurances'] 	+= $val['insurances'];
  $ldata[$val['zone_id']]['fees'] 			+= $val['fees'];
  $ldata[$val['zone_id']]['interest'] 		+= $val['interest'];
  $ldata[$val['zone_id']]['penalties'] 		+= $val['penalties'];
    
  $ldata[$val['zone_id']]['pmt_I'] 			+= ($val['type'] == 'I') ? $val['pmt'] 			: 0;
  $ldata[$val['zone_id']]['principal_I'] 	+= ($val['type'] == 'I') ? $val['principal'] 	: 0;
  $ldata[$val['zone_id']]['insurances_I'] 	+= ($val['type'] == 'I') ? $val['insurances'] 	: 0;
  $ldata[$val['zone_id']]['fees_I'] 		+= ($val['type'] == 'I') ? $val['fees'] 		: 0;
  $ldata[$val['zone_id']]['interest_I'] 	+= ($val['type'] == 'I') ? $val['interest'] 		: 0;
  $ldata[$val['zone_id']]['penalties_I'] 	+= ($val['type'] == 'I') ? $val['penalties'] 	: 0;
  
  $ldata[$val['zone_id']]['pmt_G'] 			+= ($val['type'] == 'G') ? $val['pmt'] 			: 0;
  $ldata[$val['zone_id']]['principal_G'] 	+= ($val['type'] == 'G') ? $val['principal'] 	: 0;
  $ldata[$val['zone_id']]['insurances_G'] 	+= ($val['type'] == 'G') ? $val['insurances'] 	: 0;
  $ldata[$val['zone_id']]['fees_G'] 		+= ($val['type'] == 'G') ? $val['fees'] 		: 0;
  $ldata[$val['zone_id']]['interest_G'] 	+= ($val['type'] == 'G') ? $val['interest'] 		: 0;
  $ldata[$val['zone_id']]['penalties_G'] 	+= ($val['type'] == 'G') ? $val['penalties'] 	: 0;
  
  $ldata[$val['zone_id']]['pmt_B'] 			+= ($val['type'] == 'B') ? $val['pmt'] 			: 0;
  $ldata[$val['zone_id']]['principal_B	'] 	+= ($val['type'] == 'B') ? $val['principal'] 	: 0;
  $ldata[$val['zone_id']]['insurances_B'] 	+= ($val['type'] == 'B') ? $val['insurances'] 	: 0;
  $ldata[$val['zone_id']]['fees_B'] 		+= ($val['type'] == 'B') ? $val['fees'] 		: 0;
  $ldata[$val['zone_id']]['interest_B'] 	+= ($val['type'] == 'B') ? $val['interest'] 	: 0;
  $ldata[$val['zone_id']]['penalties_B'] 	+= ($val['type'] == 'B') ? $val['penalties'] 	: 0;
  
  $total['zone'] 	 						=  $_LABELS['total'];

  $total['pmt'] 							+= $val['pmt'];
  $total['principal'] 						+= $val['principal'];
  $total['insurances'] 						+= $val['insurances'];
  $total['fees'] 							+= $val['fees'];
  $total['interest'] 						+= $val['interest'];
  $total['penalties'] 						+= $val['penalties'];
    
  $total['pmt_I'] 							+= ($val['type'] == 'I') ? $val['pmt'] 			: 0;
  $total['principal_I'] 					+= ($val['type'] == 'I') ? $val['principal'] 	: 0;
  $total['insurances_I'] 					+= ($val['type'] == 'I') ? $val['insurances'] 	: 0;
  $total['fees_I'] 							+= ($val['type'] == 'I') ? $val['fees'] 		: 0;
  $total['interest_I'] 						+= ($val['type'] == 'I') ? $val['interest'] 	: 0;
  $total['penalties_I'] 					+= ($val['type'] == 'I') ? $val['penalties'] 	: 0;
  
  $total['pmt_G'] 							+= ($val['type'] == 'G') ? $val['pmt'] 			: 0;
  $total['principal_G'] 					+= ($val['type'] == 'G') ? $val['principal'] 	: 0;
  $total['insurances_G'] 					+= ($val['type'] == 'G') ? $val['insurances'] 	: 0;
  $total['fees_G'] 							+= ($val['type'] == 'G') ? $val['fees'] 		: 0;
  $total['interest_G'] 						+= ($val['type'] == 'G') ? $val['interest'] 	: 0;
  $total['penalties_G'] 					+= ($val['type'] == 'G') ? $val['penalties'] 	: 0;
  
  $total['pmt_B'] 							+= ($val['type'] == 'B') ? $val['pmt'] 			: 0;
  $total['principal_B	'] 					+= ($val['type'] == 'B') ? $val['principal'] 	: 0;
  $total['insurances_B'] 					+= ($val['type'] == 'B') ? $val['insurances'] 	: 0;
  $total['fees_B'] 							+= ($val['type'] == 'B') ? $val['fees'] 		: 0;
  $total['interest_B'] 						+= ($val['type'] == 'B') ? $val['interest'] 	: 0;
  $total['penalties_B'] 					+= ($val['type'] == 'B') ? $val['penalties'] 	: 0;
  }

if (count($total)) { $ldata['total'] = $total; }

// applies currency format to array data

foreach($ldata as $key => $value) {
    foreach ($value as $key_inner=>$value_inner) {
    if (is_float($value_inner)) {
        $ldata[$key][$key_inner] = number_format($ldata[$key][$key_inner],2,'.',',');
    }
}
}

$head = array('zone'=>$_LABELS['tblZones.zone'],'pmt'=>sprintf('PMT-T*'),'principal'=>sprintf('KP-T*'),'insurances'=>sprintf('INS-T*'),'fees'=>sprintf('FEES-T*'),'interest'=>sprintf('RT-T*'),'penalties'=>sprintf('PN-T*'),'pmt_I'=>sprintf('PMT-I*'),'principal_I'=>sprintf('KP-I*'),'insurances_I'=>sprintf('INS-I*'),'fees_I'=>sprintf('FEES-I*'),'interest_I'=>sprintf('RT-I*'),'penalties_I'=>sprintf('PN-I*'),'pmt_G'=>sprintf('PMT-G*'),'principal_G'=>sprintf('KP-G*'),'insurances_G'=>sprintf('INS-G*'),'fees_G'=>sprintf('FEES-G*'),'interest_G'=>sprintf('RT-G*'),'penalties_G'=>sprintf('PN-G*'),'pmt_B'=>sprintf('PMT-B*'),'principal_B'=>sprintf('KP-B*'),'insurances_B'=>sprintf('INS-B*'),'fees_B'=>sprintf('FEES-B*'),'interest_B'=>sprintf('RT-B*'),'penalties_B'=>sprintf('PN-B*'));
$tpl->setVariable('rpt_label', 			$_LABELS['recovery']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.recoveryByBranch']);
$tpl->setVariable('date_range_label', 	$_LABELS['dateRange']);
$tpl->setVariable('date_range', 		sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($ldata) ? WEBPAGE::printchart($ldata,$head) : $_LABELS['noData']);

$tpl->setVariable('recoveryInPeriod', 	$_LABELS['recoveryInPeriod']);
$tpl->setVariable('legend', 			sprintf('<b>%s:</b> <b>PMT</b> - %s, <b>KP</b> - %s, <b>INS</b> - %s, <b>FEES</b> - %s, <b>RT</b> - %s, <b>PN</b> - %s, <b>T</b> - %s, <b>I</b> - %s, <b>G</b> - %s, <b>B</b> - %s',$_LABELS['legend'],$_LABELS['PMT'],$_LABELS['KP'],$_LABELS['INS'],$_LABELS['FEES'],$_LABELS['RT'],$_LABELS['PN'],$_LABELS['R-T'],$_LABELS['R-I'],$_LABELS['R-G'],$_LABELS['R-B']));

?>

