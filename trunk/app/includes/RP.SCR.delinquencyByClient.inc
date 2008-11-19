<?php
//require_once 'class/roles.php';
//require_once 'class/persons.php';
//require_once 'class/client.php';

//$client = new CLIENT();


$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');
$tpl->setVariable('dateSelector', 		$_LABELS['BS.SCR.dateSelector']);
$tpl->setVariable('olddate', 			$date);
/*
$data = WEBPAGE::$dbh->getAll(sprintf("
		 select l.client_id as client_id, concat(c.last,', ',c.first) as client, c.code as ccode,(if(isnull(c.phone) or c.phone=' ',c.mobile,c.phone)) as phone,s.name as name,count(lod.id) as loans, sum(lod.hits) as hits, sum(lod.delay) as delay,sum(lod.pmt) as pmt, sum(lod.penalties) as penalties, sum(lod.interest) as interest, sum(lod.fees) as fees, sum(lod.insurances) as insurances,  sum(lod.principal) as principal,u.username as advisor, z.zone as zone, p.program as program
		 		from (tblLoansOnDelinquency as lod, tblClients as c, tblLoans as l, tblUsers as u, tblZones as z, tblPrograms as p, tblLoanTypes as lt) LEFT JOIN tblSocieties as s ON c.society_id = s.id  where lt.id = l.loan_type_id and lt.borrower_type != 'G' and l.id = lod.loan_id and c.id = l.client_id and u.id = l.advisor_id and z.id = l.zone_id and p.id = l.program_id and lod.date = '%s' group by l.client_id order by s.name,u.username", $date));
*/
$data = WEBPAGE::$dbh->getAll(sprintf("
						select
    						l.client_id as client_id,
    						concat(c.last,', ',c.first) as client,
    						c.code as ccode,
    						(if(isnull(c.phone) or c.phone=' ',c.mobile,c.phone)) as phone,
    						s.name as name,
    						count(lod.id) as loans,
    						sum(lod.hits) as hits,
    						sum(lod.delay) as delay,
    						sum(lod.pmt) as pmt,
    						sum(lod.penalties) as penalties,
    						sum(lod.interest) as interest,
    						sum(lod.fees) as fees,
    						sum(lod.insurances) as insurances,
    						sum(lod.principal) as principal,
    						u.username as advisor,
    						z.zone as zone,
    						p.program as program,
    						f.fund as fund
  						from
    						(
    						tblLoansOnDelinquency as lod,
    						tblClients as c,
    						tblLoans as l,
    						tblUsers as u,
    						tblZones as z,
    						tblPrograms as p,
    						tblLoanTypes as lt,
    						tblLoansMasterDetails as lmd,
    						tblFundsLoansMasterPct as flmp,
    						tblFunds as f
    						)
    						LEFT JOIN tblSocieties as s ON c.society_id = s.id
  						where
    						lt.id = l.loan_type_id and
    						lt.borrower_type != 'G' and
    						l.id = lod.loan_id and
    						c.id = l.client_id and
    						u.id = l.advisor_id and
    						z.id = l.zone_id and
    						p.id = l.program_id and
    						lmd.loan_id = lod.loan_id and
    						flmp.master_id = lmd.master_id and
    						f.id = flmp.fund_id and
    						lod.date = '%s'
  						group by
    						l.client_id
  						order by
    						s.name,u.username", $date));


$total = current(WEBPAGE::$dbh->getAll(sprintf("
		 select '%s' as client, '-' as ccode,'-' as phone, '-' as name,
		 		count(lod.id) as loans, sum(lod.hits) as hits, sum(lod.delay) as delay,
		 		sum(lod.pmt) as pmt, sum(lod.penalties) as penalties, sum(lod.interest) as interest, sum(lod.fees) as fees, sum(lod.insurances) as insurances,  sum(lod.principal) as principal,
		 		'-' as advisor, '-' as zone, '-' as program, '-' as fund
		 		from tblLoansOnDelinquency as lod, tblLoans as l, tblLoanTypes as lt where lt.id = l.loan_type_id and lt.borrower_type != 'G' and l.id = lod.loan_id and lod.date = '%s'", $_LABELS['total'],$date)));



$head = array('client'=>$_LABELS['tblClients.id'],'ccode'=>$_LABELS['tblClients.code'],'phone'=>$_LABELS['tblClients.phone'],'name'=>$_LABELS['tblSocieties.name'],'loans'=>$_LABELS['loans'],'hits'=>$_LABELS['payments'],'delay'=>$_LABELS['delay'],'pmt'=>$_LABELS['kp'],'penalties'=>$_LABELS['PN'],'interest'=>$_LABELS['RT'],'fees'=>$_LABELS['FEES'],'insurances'=>$_LABELS['INS'],'principal'=>$_LABELS['KP'],'advisor'=>$_LABELS['tblClients.advisor_id'],'zone'=>$_LABELS['tblZones.zone'],'program'=>$_LABELS['tblPrograms.program'],'fund'=>$_LABELS['tblFunds.fund']);




foreach($data as $key=>$val) {
  unset($data[$key]['client_id']);
//  $client->data['id'] = $val['client_id'];
//  $client->checkConfidentiality();
//  if ($client->data['confidential']) {
//    $data[$key]['client'] = $client->data['name'];
//    $data[$key]['ccode']  = $client->data['code'];
//    }
  $data[$key]['client'] 	 	= str_replace(' ','&nbsp;',$data[$key]['client']);
  $data[$key]['zone'] 	 	 	= str_replace(' ','&nbsp;',$data[$key]['zone']);
  $data[$key]['program'] 	 	= str_replace(' ','&nbsp;',$data[$key]['program']);
  $data[$key]['fund'] 	 	    = str_replace(' ','&nbsp;',$data[$key]['fund']);
  }



if(count($data)) { $data['total'] = $total; }

$tpl->setVariable('rpt_label', 		$_LABELS['delinquencyPortfolio']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.delinquencyByClient']);
$tpl->setVariable('data_date_label', 	$_LABELS['date']);
$tpl->setVariable('data_date', 		$date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : $_LABELS['noData']);
?>