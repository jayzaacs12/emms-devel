<?php
$date_from 	= $_REQUEST['date_from'] 	? $_REQUEST['date_from'] 	: date('Y-m-d');
$date_to 	= $_REQUEST['date_to'] 		? $_REQUEST['date_to'] 		: date('Y-m-d');
$tpl->setVariable('dateRangeSelector', 	$_LABELS['BS.SCR.dateRangeSelector']);
$tpl->setVariable('oldrange', 			sprintf('%s - %s',$date_from,$date_to));

// $data = WEBPAGE::$dbh->getAll(sprintf("SELECT l.loan_code,concat(c.last,', ',c.first) client, c.code ccode,lt.description,bt.type,l.kp,l.delivered_date,z.zone,p.program,u.username FROM (tblLoans l, tblClients c, tblLoanTypes lt, tblUsers u, tblZones z, tblPrograms p) left join tblBusiness b on b.id = l.business_id left join tblBusinessTypes bt on bt.id = b.type_id where l.delivered_date >= '%s' and l.delivered_date <= '%s' and c.id = l.client_id and lt.id = l.loan_type_id and u.id = l.advisor_id and z.id = l.zone_id and p.id = l.program_id", $date_from, $date_to));
$data = WEBPAGE::$dbh->getAll(sprintf("
								SELECT
    								l.loan_code,
    								concat(c.last,', ',c.first) client,
    								c.code ccode,
    								if(lm.borrower_type != 'I',s.name,'') society,
    								lt.description,
    								bt.type,
    								l.kp,
    								l.delivered_date,
    								z.zone,
    								p.program,
    								f.fund,
    								lm.check_number,
    								u.username
  								FROM
    								(
    									tblLoans l,
       									tblClients c,
       									tblLoanTypes lt,
       									tblUsers u,
       									tblZones z,
       									tblPrograms p,
       									tblLoansMasterDetails lmd,
       									tblFundsLoansMasterPct flmp,
       									tblLoansMaster lm,
       									tblFunds f
    								)
    								left join tblBusiness b on b.id = l.business_id
    								left join tblBusinessTypes bt on bt.id = b.type_id
    								left join tblSocieties s on s.id = lm.borrower_id
  								where
    								l.delivered_date >= '%s' and
    								l.delivered_date <= '%s' and
    								c.id = l.client_id and
    								lt.id = l.loan_type_id and
    								u.id = l.advisor_id and
    								z.id = l.zone_id and
    								p.id = l.program_id and
    								lmd.loan_id = l.id and
    								flmp.master_id = lmd.master_id and
    								f.id = flmp.fund_id and
    								lm.id = lmd.master_id", $date_from, $date_to));

$head = array('loan_code'=>$_LABELS['tblLoans.loan_code'],'client'=>$_LABELS['name'],'ccode'=>$_LABELS['tblClients.code'],'society'=>$_LABELS['tblClients.society_id'],'description'=>$_LABELS['tblLoanTypes.description'],'type'=>$_LABELS['tblBusinessTypes.type'],'kp'=>$_LABELS['tblLoans.kp'],'delivered_date'=>$_LABELS['tblLoans.delivered_date'],'zone'=>$_LABELS['tblZones.zone'],'program'=>$_LABELS['tblPrograms.program'],'fund'=>$_LABELS['tblFunds.fund'],'check_number'=>$_LABELS['tblLoansMaster.check_number'],'username'=>$_LABELS['tblClients.advisor_id']);

$tpl->setVariable('rpt_label', 			$_LABELS['loanPortfolio']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.loanByClient']);
$tpl->setVariable('date_range_label', 	$_LABELS['dateRange']);
$tpl->setVariable('date_range', 		sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : $_LABELS['noData']);

$tpl->setVariable('releasedInPeriod', 	$_LABELS['releasedInPeriod']);

?>
