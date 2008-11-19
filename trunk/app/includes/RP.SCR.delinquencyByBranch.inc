<?php
$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');
$tpl->setVariable('dateSelector', 		$_LABELS['BS.SCR.dateSelector']);
$tpl->setVariable('olddate', 			$date);
$data = WEBPAGE::$dbh->getAll(sprintf("
		 select z.zone as zone,
		 		count(lod.id) as loans, sum(lod.hits) as hits, sum(lod.delay) as delay,
		 		sum(lod.pmt) as pmt, sum(lod.penalties) as penalties, sum(lod.interest) as interest, sum(lod.fees) as fees, sum(lod.insurances) as insurances,  sum(lod.principal) as principal
		 		from tblLoansOnDelinquency as lod, tblZones as z, tblLoans as l where l.id = lod.loan_id and z.id = l.zone_id and lod.date = '%s' group by l.zone_id", $date));
$total = current(WEBPAGE::$dbh->getAll(sprintf("
		 select '%s' as total,
		 		count(lod.id) as loans, sum(lod.hits) as hits, sum(lod.delay) as delay,
		 		sum(lod.pmt) as pmt, sum(lod.penalties) as penalties, sum(lod.interest) as interest, sum(lod.fees) as fees, sum(lod.insurances) as insurances,  sum(lod.principal) as principal
		 		from tblLoansOnDelinquency as lod where lod.date = '%s'", $_LABELS['total'],$date)));
$head = array('zone'=>$_LABELS['tblZones.zone'],'loans'=>$_LABELS['loans'],'hits'=>$_LABELS['payments'],'delay'=>$_LABELS['delay'],'pmt'=>$_LABELS['kp'],'penalties'=>$_LABELS['PN'],'interest'=>$_LABELS['RT'],'fees'=>$_LABELS['FEES'],'insurances'=>$_LABELS['INS'],'principal'=>$_LABELS['KP']);

if(count($data)) { $data['total'] = $total; }

$tpl->setVariable('rpt_label', 			$_LABELS['delinquencyPortfolio']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.delinquencyByBranch']);
$tpl->setVariable('data_date_label', 	$_LABELS['date']);
$tpl->setVariable('data_date', 			$date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : $_LABELS['noData']);
?>
