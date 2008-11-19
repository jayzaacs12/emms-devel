<?php
$date_from 	= $_REQUEST['date_from'] 	? $_REQUEST['date_from'] 	: date('Y-m-d');
$date_to 	= $_REQUEST['date_to'] 		? $_REQUEST['date_to'] 		: date('Y-m-d');
$tpl->setVariable('dateRangeSelector', 	$_LABELS['BS.SCR.dateRangeSelector']);
$tpl->setVariable('oldrange', 			sprintf('%s - %s',$date_from,$date_to));

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select cr.date,z.zone,p.program,f.fund,cr.principal,cr.fees,cr.insurances,cr.interest,cr.penalties,cr.amount,cr.code
           from tblTCredits as cr, tblZones as z, tblPrograms as p, tblFunds f
           where z.id = cr.branch_id and p.id = cr.program_id and f.id = cr.fund_id and cr.date >= '%s' and cr.date <= '%s'
           order by cr.branch_id, cr.program_id", $date_from, $date_to));

$head = array('date'=>$_LABELS['date'],'zone'=>$_LABELS['tblZones.zone'],'program'=>$_LABELS['tblPrograms.program'],'fund'=>$_LABELS['tblFunds.fund'],'principal'=>$_LABELS['principal'],'fees'=>$_LABELS['FEES'],'insurances'=>$_LABELS['INS'],'interest'=>$_LABELS['interest'],'penalties'=>$_LABELS['PN'],'amount'=>$_LABELS['total'],'code'=>$_LABELS['code']);

foreach($data as $key=>$val) {
  $data[$key]['code'] = sprintf('<a href=\'javascript:openWin("index.popup.php?scr_name=LN.SCR.viewTransactionDetails&transaction_id=%s","%s","menubar=no,scrollbars=yes,resizable=yes,width=700,height=320")\' onClick="">%s</a>',$val['code'],$val['code'],$val['code']);
  }
$tpl->setVariable('rpt_label', 			$_LABELS['transactionsCredits']);
//$tpl->setVariable('rpt_subtitle_label', $_LABELS['transactionsCredits']);
$tpl->setVariable('date_range_label', 	$_LABELS['dateRange']);
$tpl->setVariable('date_range', 		sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : $_LABELS['noData']);

?>
