<?php

$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');
$tpl->setVariable('dateSelector', 		$_LABELS['BS.SCR.dateSelector']);
$tpl->setVariable('olddate', 			$date);

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select
            xl.loan_code, c.code, concat(c.last,', ', c.first) client, if((lm.borrower_type != 'I'),s.name,'-') bde, xl.kp, u.username, f.fund, p.program, z.zone, xl.date_g, xl.date_c, xl.status
         from
            (
            select
                lsh1.loan_id,lsh1.date date_g,
                lsh2.date date_c,
                l.loan_code,l.client_id,l.program_id,l.zone_id, l.advisor_id, l.kp, l.status,
                lmd.master_id
            from
                tblLoans l, tblLoansMasterDetails lmd, tblLoanStatusHistory lsh1
            left join
                tblLoanStatusHistory lsh2 on lsh2.loan_id = lsh1.loan_id and lsh2.p_status = 'G'
            where
                lsh1.status = 'G' and
                lsh1.date <= '%s' and
                ((lsh2.date >= '%s')or(lsh2.date is null)) and
                l.id = lsh1.loan_id and
                lmd.loan_id = lsh1.loan_id
            )
            xl, tblClients c, tblPrograms p, tblZones z, tblUsers u, tblFundsLoansMasterPct flmp, tblFunds f, tblLoansMaster lm
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

$head = array('loan_code' => $_LABELS['tblLoans.loan_code'], 'code' => $_LABELS['tblClients.code'], 'client' => $_LABELS['tblClients.id'], 'bde' => $_LABELS['tblClients.society_id'], 'kp' => $_LABELS['tblLoans.kp'],'username' => $_LABELS['tblClients.advisor_id'], 'fund' => $_LABELS['tblFunds.fund'], 'program' => $_LABELS['tblPrograms.program'], 'zone' => $_LABELS['tblZones.zone'], 'dateG' => $_LABELS['tblLoans.delivered_date'], 'dateC' => $_LABELS['cancel_date'], 'status' => $_LABELS['tblLoans.status']);
$pack = array('status' => 'tblLoans.status.%s');

$tpl->setVariable('rpt_label', 			$_LABELS['activeLoanPortfolio']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.activeLoanPortfolioByLoan']);
$tpl->setVariable('data_date_label', 	$_LABELS['date']);
$tpl->setVariable('data_date', 			$date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head,$pack) : $_LABELS['noData']);
?>


