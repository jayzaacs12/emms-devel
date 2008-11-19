<?php
$date_from 	= $_REQUEST['date_from'] 	? $_REQUEST['date_from'] 	: date('Y-m-d');
$date_to 	= $_REQUEST['date_to'] 		? $_REQUEST['date_to'] 		: date('Y-m-d');
$tpl->setVariable('dateRangeSelector', 	$_LABELS['BS.SCR.dateRangeSelector']);
$tpl->setVariable('oldrange', 			sprintf('%s - %s',$date_from,$date_to));

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select l.program_id, p.program as program,
		 		count(l.id) as loans, sum(l.kp) as kp, lt.borrower_type as type
		 		from tblLoans as l, tblPrograms as p, tblLoanTypes as lt where p.id = l.program_id and lt.id = l.loan_type_id and l.delivered_date >= '%s' and l.delivered_date <= '%s' group by l.program_id, type", $date_from, $date_to));

foreach($data as $key=>$val) {
  $ldata[$val['program_id']]['program'] 	 = $val['program'];
  $ldata[$val['program_id']]['loans'] 		+= $val['loans'];
  $ldata[$val['program_id']]['kp'] 			+= $val['kp'];
  $ldata[$val['program_id']]['loans_I'] 	+= ($val['type'] == 'I') ? $val['loans'] 	: 0;
  $ldata[$val['program_id']]['kp_I'] 		+= ($val['type'] == 'I') ? $val['kp'] 		: 0;
  $ldata[$val['program_id']]['loans_G'] 	+= ($val['type'] == 'G') ? $val['loans'] 	: 0;
  $ldata[$val['program_id']]['kp_G'] 		+= ($val['type'] == 'G') ? $val['kp'] 		: 0;
  $ldata[$val['program_id']]['loans_B'] 	+= ($val['type'] == 'B') ? $val['loans'] 	: 0;
  $ldata[$val['program_id']]['kp_B'] 		+= ($val['type'] == 'B') ? $val['kp'] 		: 0;

  $total['program'] 	 					 = $_LABELS['total'];
  $total['loans'] 							+= $val['loans'];
  $total['kp'] 								+= $val['kp'];
  $total['loans_I'] 						+= ($val['type'] == 'I') ? $val['loans'] 	: 0;
  $total['kp_I'] 							+= ($val['type'] == 'I') ? $val['kp'] 		: 0;
  $total['loans_G'] 						+= ($val['type'] == 'G') ? $val['loans'] 	: 0;
  $total['kp_G'] 							+= ($val['type'] == 'G') ? $val['kp'] 		: 0;
  $total['loans_B'] 						+= ($val['type'] == 'B') ? $val['loans'] 	: 0;
  $total['kp_B'] 							+= ($val['type'] == 'B') ? $val['kp'] 		: 0;
  }

if (count($ldata)) {$ldata['total'] = $total;}

$head = array('zone'=>$_LABELS['tblPrograms.program'],'loans'=>sprintf('%s *',$_LABELS['loans']),'kp'=>sprintf('%s *',$_LABELS['tblLoans.kp']),'loans_I'=>sprintf('%s *',$_LABELS['loans_I']),'kp_I'=>sprintf('%s *',$_LABELS['kp_I']),'loans_G'=>sprintf('%s *',$_LABELS['loans_G']),'kp_G'=>sprintf('%s *',$_LABELS['kp_G']),'loans_B'=>sprintf('%s *',$_LABELS['loans_B']),'kp_B'=>sprintf('%s *',$_LABELS['kp_B']));

$tpl->setVariable('rpt_label', 			$_LABELS['loanPortfolio']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.loanByProgram']);
$tpl->setVariable('date_range_label', 	$_LABELS['dateRange']);
$tpl->setVariable('date_range', 		sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($ldata) ? WEBPAGE::printchart($ldata,$head) : $_LABELS['noData']);

$tpl->setVariable('releasedInPeriod', 	$_LABELS['releasedInPeriod']);

?>

