<?php

$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');
$tpl->setVariable('dateSelector', 		$_LABELS['BS.SCR.dateSelector']);
$tpl->setVariable('olddate', 			$date);

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select
  xl.loan_code, c.code, concat(c.last,', ', c.first) client, if((lm.borrower_type != 'I'),s.name,'-') bde, xl.kp, u.username, f.fund, p.program, z.zone, xl.dateG, xl.dateC, xl.status
from
  (
    select
      lsh.id, lsh.loan_id, lsh.date dateG, lshc.date dateC, l.client_id, l.loan_code, l.program_id, l.zone_id, lmd.master_id, l.advisor_id, l.kp, l.status
    from
      tblLoanStatusHistory lsh
    left join
      (select lshi.id,lshi.loan_id,lshi.date from tblLoanStatusHistory lshi where lshi.p_status = 'G' and lshi.date <= '%s') lshp
      on lshp.loan_id = lsh.loan_id
    left join
      tblLoanStatusHistory lshc on (lshc.status = 'C' and lshc.loan_id = lsh.loan_id)
    left join
      tblLoans l on l.id = lsh.loan_id
    left join
      tblLoansMasterDetails lmd on lmd.loan_id = lsh.loan_id
    where
      lsh.status = 'G' and
      lsh.date <= '%s' and
      lshp.id is null
   ) xl, tblClients c, tblPrograms p, tblZones z, tblUsers u, tblFundsLoansMasterPct flmp, tblFunds f, tblLoansMaster lm
  left join
    tblSocieties s on s.id = lm.borrower_id
  where
    c.id = xl.client_id and
    lm.id = xl.master_id and
    z.id = xl.zone_id and
    p.id = xl.program_id and
    u.id = xl.advisor_id and
    flmp.master_id = xl.master_id and
    f.id = flmp.fund_id", $date, $date));

/*
$total = current(WEBPAGE::$dbh->getAll(sprintf("
		 select '%s' as advisor,
		 		sum(cp.clients) as clients,sum(cp.female) as female,sum(cp.male) as male,sum(cp.client_i) as client_i,sum(cp.client_g) as client_g,sum(cp.client_b) as client_b,sum(cp.group_g) as group_g,sum(cp.group_b) as group_b,sum(cp.group_bg) as group_bg,sum(cp.client_al) as client_al
		 		from tblClientPortfolio as cp, tblUsers as u where u.id = cp.advisor_id and cp.date = '%s'", $_LABELS['total'],$date)));

if (count($data)) { $data['total'] = $total; }
*/


$head = array('loan_code' => $_LABELS['tblLoans.loan_code'], 'code' => $_LABELS['tblClients.code'], 'client' => $_LABELS['tblClients.id'], 'bde' => $_LABELS['tblClients.society_id'], 'kp' => $_LABELS['tblLoans.kp'],'username' => $_LABELS['tblClients.advisor_id'], 'fund' => $_LABELS['tblFunds.fund'], 'program' => $_LABELS['tblPrograms.program'], 'zone' => $_LABELS['tblZones.zone'], 'dateG' => $_LABELS['tblLoans.delivered_date'], 'dateC' => $_LABELS['cancel_date'], 'status' => $_LABELS['tblLoans.status']);
$pack = array('status' => 'tblLoans.status.%s');

$tpl->setVariable('rpt_label', 			$_LABELS['activeLoanPortfolio']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.activeLoanPortfolioByLoan']);
$tpl->setVariable('data_date_label', 	$_LABELS['date']);
$tpl->setVariable('data_date', 			$date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head,$pack) : $_LABELS['noData']);
?>


