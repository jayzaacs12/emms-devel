<?php
$date_from 	= $_REQUEST['date_from'] 	? $_REQUEST['date_from'] 	: date('Y-m-d');
$date_to 	= $_REQUEST['date_to'] 		? $_REQUEST['date_to'] 		: date('Y-m-d');
$tpl->setVariable('dateRangeSelector', 	$_LABELS['BS.SCR.dateRangeSelector']);
$tpl->setVariable('oldrange', 			sprintf('%s - %s',$date_from,$date_to));

$data = WEBPAGE::$dbh->getAll(sprintf("
								select
                                    xl.loan_code, c.code, concat(c.last,', ', c.first) client, if((lm.borrower_type != 'I'),s.name,'-') bde, xl.kp, u.username, f.fund, p.program, z.zone, xl.date_g, xl.date_c
                                from
                                    (
                                    select
                                        lsh.loan_id, l.delivered_date date_g, lsh.date date_c, l.client_id, l.loan_code, l.program_id, l.zone_id, lmd.master_id, l.advisor_id, l.kp
                                    from
                                        tblLoans l, tblLoanStatusHistory lsh, tblLoansMasterDetails lmd
                                    where
                                        lsh.status = 'C' and
                                        lsh.date >= '%s' and
                                        lsh.date <= '%s' and
                                        l.id = lsh.loan_id and
                                        lmd.loan_id = lsh.loan_id
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
                                    f.id = flmp.fund_id", $date_from, $date_to));

$head = array('loan_code' => $_LABELS['tblLoans.loan_code'], 'code' => $_LABELS['tblClients.code'], 'client' => $_LABELS['tblClients.id'], 'bde' => $_LABELS['tblClients.society_id'], 'kp' => $_LABELS['tblLoans.kp'],'username' => $_LABELS['tblClients.advisor_id'], 'fund' => $_LABELS['tblFunds.fund'], 'program' => $_LABELS['tblPrograms.program'], 'zone' => $_LABELS['tblZones.zone'], 'date_g' => $_LABELS['tblLoans.delivered_date'], 'date_c' => $_LABELS['cancel_date']);

$tpl->setVariable('rpt_label', 			$_LABELS['cancelledLoanPortfolio']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.cancelledLoanPortfolioByLoan']);
$tpl->setVariable('date_range_label', 	$_LABELS['dateRange']);
$tpl->setVariable('date_range', 		sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : $_LABELS['noData']);

$tpl->setVariable('cancelledInPeriod', 	$_LABELS['cancelledInPeriod']);

?>
