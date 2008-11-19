<?php
$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');
$tpl->setVariable('dateSelector', 		$_LABELS['BS.SCR.dateSelector']);
$tpl->setVariable('olddate', 			$date);
/*
$data = WEBPAGE::$dbh->getAll(sprintf("
		 select concat(u.first,' ',u.last) as advisor,
		 		sum(rp.balance) as balance,
		 		sum(rp.riskB) as risk, concat(round(100*(sum(rp.riskB)/sum(rp.balance)),2),'%%') as riskPct
		 		from tblRiskPortfolio as rp, tblUsers as u where u.id = rp.advisor_id and rp.date = '%s' group by rp.advisor_id", $date));

$total = current(WEBPAGE::$dbh->getAll(sprintf("
		 select '%s' as advisor,
		 		sum(rp.balance) as balance,
		 		sum(rp.riskB) as risk, concat(round(100*(sum(rp.riskB)/sum(rp.balance)),2),'%%') as riskPct
		 		from tblRiskPortfolio as rp, tblUsers as u where u.id = rp.advisor_id and rp.date = '%s'", $_LABELS['total'],$date)));
$head = array('advisor'=>$_LABELS['tblClients.advisor_id'],'balance'=>$_LABELS['portfolio_balance'],'risk'=>$_LABELS['riskPortfolio'],'riskPct'=>sprintf('%s (%s)',$_LABELS['riskPortfolio'],'%'));
*/
$data = WEBPAGE::$dbh->getAll(sprintf("
		 select concat(u.first,' ',u.last) as advisor,
		 		sum(rp.balance) as balance,
		 		sum(rp.riskA-rp.riskB) as riskA, concat(round(100*(sum(rp.riskA-rp.riskB)/sum(rp.balance)),2),'%%') as riskAPct,
		 		sum(rp.riskB-rp.riskC) as riskB, concat(round(100*(sum(rp.riskB-rp.riskC)/sum(rp.balance)),2),'%%') as riskBPct,
		 		sum(rp.riskC-rp.riskD) as riskC, concat(round(100*(sum(rp.riskC-rp.riskD)/sum(rp.balance)),2),'%%') as riskCPct,
		 		sum(rp.riskD) as riskD, concat(round(100*(sum(rp.riskD)/sum(rp.balance)),2),'%%') as riskDPct
		 		from tblRiskPortfolio as rp, tblUsers as u where u.id = rp.advisor_id and rp.date = '%s' group by rp.advisor_id", $date));

$total = current(WEBPAGE::$dbh->getAll(sprintf("
		 select '%s' as advisor,
		 		sum(rp.balance) as balance,
		 		sum(rp.riskA-rp.riskB) as riskA, concat(round(100*(sum(rp.riskA-rp.riskB)/sum(rp.balance)),2),'%%') as riskAPct,
		 		sum(rp.riskB-rp.riskC) as riskB, concat(round(100*(sum(rp.riskB-rp.riskC)/sum(rp.balance)),2),'%%') as riskBPct,
		 		sum(rp.riskC-rp.riskD) as riskC, concat(round(100*(sum(rp.riskC-rp.riskD)/sum(rp.balance)),2),'%%') as riskCPct,
		 		sum(rp.riskD) as riskD, concat(round(100*(sum(rp.riskD)/sum(rp.balance)),2),'%%') as riskDPct
		 		from tblRiskPortfolio as rp, tblUsers as u where u.id = rp.advisor_id and rp.date = '%s'", $_LABELS['total'],$date)));
$head = array('advisor'=>$_LABELS['tblClients.advisor_id'],'balance'=>$_LABELS['portfolio_balance'],'riskA'=>sprintf($_LABELS['par.col.header'],'$',$_CONF['risk.days.A'],$_CONF['risk.days.B']),'riskAPct'=>sprintf($_LABELS['par.col.header'],'%',$_CONF['risk.days.A'],$_CONF['risk.days.B']),'riskB'=>sprintf($_LABELS['par.col.header'],'$',$_CONF['risk.days.B'],$_CONF['risk.days.C']),'riskBPct'=>sprintf($_LABELS['par.col.header'],'%',$_CONF['risk.days.B'],$_CONF['risk.days.C']),'riskC'=>sprintf($_LABELS['par.col.header'],'$',$_CONF['risk.days.C'],$_CONF['risk.days.D']),'riskCPct'=>sprintf($_LABELS['par.col.header'],'%',$_CONF['risk.days.C'],$_CONF['risk.days.D']),'riskD'=>sprintf($_LABELS['par.col.header'],'$',$_CONF['risk.days.D'],'...'),'riskDPct'=>sprintf($_LABELS['par.col.header'],'%',$_CONF['risk.days.D'],'...'));

if (count($data)) { $data['total'] = $total; }

$tpl->setVariable('rpt_label', 			$_LABELS['riskPortfolio']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.riskByAdvisor']);
$tpl->setVariable('data_date_label', 	$_LABELS['date']);
$tpl->setVariable('data_date', 			$date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : $_LABELS['noData']);
?>

