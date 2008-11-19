<?php
$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');
$tpl->setVariable('dateSelector', 		$_LABELS['BS.SCR.dateSelector']);
$tpl->setVariable('olddate', 			$date);
// PATCH: 2008.07.18 $data sql string modify
$data = WEBPAGE::$dbh->getAll(sprintf("
		 select concat(u.first,' ',u.last) as advisor,
		 		sum(cp.clients) as clients,sum(cp.female) as female,sum(cp.male) as male,sum(cp.client_i) as client_i,sum(cp.client_g) as client_g,sum(cp.client_b) as client_b,sum(cp.group_g) as group_g,sum(cp.group_b) as group_b,sum(cp.group_bg) as group_bg,sum(cp.client_al) as client_al
		 		from tblClientPortfolio as cp, tblUsers as u where u.id = cp.advisor_id and cp.date = '%s' group by cp.advisor_id", $date));
// PATCH: 2008.07.18 $total sql string modify
$total = current(WEBPAGE::$dbh->getAll(sprintf("
		 select '%s' as advisor,
		 		sum(cp.clients) as clients,sum(cp.female) as female,sum(cp.male) as male,sum(cp.client_i) as client_i,sum(cp.client_g) as client_g,sum(cp.client_b) as client_b,sum(cp.group_g) as group_g,sum(cp.group_b) as group_b,sum(cp.group_bg) as group_bg,sum(cp.client_al) as client_al
		 		from tblClientPortfolio as cp, tblUsers as u where u.id = cp.advisor_id and cp.date = '%s'", $_LABELS['total'],$date)));

if (count($data)) { $data['total'] = $total; }
// PATCH: 2008.07.18 
$head = array('advisor'=>$_LABELS['tblClients.advisor_id'],'clients'=>$_LABELS['clients'],'female'=>$_LABELS['female'],'male'=>$_LABELS['male'],'client_i'=>$_LABELS['client_i'],'client_g'=>$_LABELS['client_g'],'client_b'=>$_LABELS['client_b'],'group_g'=>$_LABELS['group_g'],'group_b'=>$_LABELS['group_b'],'group_bg'=>$_LABELS['group_bg'],'client_al' =>$_LABELS['activeWithLoan']);

$tpl->setVariable('rpt_label', 			$_LABELS['clientPortfolio']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.clientByAdvisor']);
$tpl->setVariable('data_date_label', 	$_LABELS['date']);
$tpl->setVariable('data_date', 			$date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : $_LABELS['noData']);
?>
