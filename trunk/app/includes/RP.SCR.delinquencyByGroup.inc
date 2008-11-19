<?php
$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');
//$tpl->setVariable('dateSelector', 		$_LABELS['BS.SCR.dateSelector']);
//$tpl->setVariable('olddate', 			$date);

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select s.name as society,
		 		count(lod.id) as loans, sum(lod.hits) as hits, sum(lod.delay) as delay,
		 		sum(lod.pmt) as pmt, sum(lod.penalties) as penalties, sum(lod.interest) as interest, sum(lod.fees) as fees, sum(lod.insurances) as insurances,  sum(lod.principal) as principal,
		 		u.username as advisor, z.zone as zone, p.program as program
		 		from tblLoansOnDelinquency as lod, tblClients as c, tblLoans as l, tblUsers as u, tblZones as z, tblPrograms as p, tblLoanTypes as lt, tblSocieties as s
                                where lt.id = l.loan_type_id and lt.borrower_type = 'B' and l.id = lod.loan_id and c.id = l.client_id and u.id = l.advisor_id and z.id = l.zone_id and p.id = l.program_id and s.id = c.society_id and lod.date = '%s' group by s.id", $date));

$head = array('society'=>$_LABELS['tblSocieties.name'],'loans'=>$_LABELS['loans'],'hits'=>$_LABELS['payments'],'delay'=>$_LABELS['delay'],'pmt'=>$_LABELS['kp'],'penalties'=>$_LABELS['PN'],'interest'=>$_LABELS['RT'],'fees'=>$_LABELS['FEES'],'insurances'=>$_LABELS['INS'],'principal'=>$_LABELS['KP'],'advisor'=>$_LABELS['tblClients.advisor_id'],'zone'=>$_LABELS['tblZones.zone'],'program'=>$_LABELS['tblPrograms.program']);

$total = current(WEBPAGE::$dbh->getAll(sprintf("
		 select '%s' as society,
		 		count(lod.id) as loans, sum(lod.hits) as hits, sum(lod.delay) as delay,
		 		sum(lod.pmt) as pmt, sum(lod.penalties) as penalties, sum(lod.interest) as interest, sum(lod.fees) as fees, sum(lod.insurances) as insurances,  sum(lod.principal) as principal,
		 		'-' as advisor, '-' as zone, '-' as program
		 		from tblLoansOnDelinquency as lod, tblLoans as l, tblLoanTypes as lt where lt.id = l.loan_type_id and lt.borrower_type = 'B' and l.id = lod.loan_id and lod.date = '%s'", $_LABELS['total'],$date)));

foreach($data as $key=>$val) {
  $data[$key]['society'] 	 	= str_replace(' ','&nbsp;',$data[$key]['society']);
  $data[$key]['zone'] 	 	 	= str_replace(' ','&nbsp;',$data[$key]['zone']);
  $data[$key]['program'] 	 	= str_replace(' ','&nbsp;',$data[$key]['program']);
  }

if(count($data)) { $data['total'] = $total; }
$tpl->setVariable('rpt_label', 		$_LABELS['delinquencyPortfolio']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.delinquencyByGroup']);
$tpl->setVariable('data_date_label', 	$_LABELS['date']);
$tpl->setVariable('data_date', 		$date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : $_LABELS['noData']);
?>