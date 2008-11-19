<?php
$date_from 	= $_REQUEST['date_from'] 	? $_REQUEST['date_from'] 	: date('Y-m-d');
$date_to 	= $_REQUEST['date_to'] 		? $_REQUEST['date_to'] 		: date('Y-m-d');
$tpl->setVariable('dateRangeSelector', 	$_LABELS['BS.SCR.dateRangeSelector']);
$tpl->setVariable('oldrange', 			sprintf('%s - %s',$date_from,$date_to));

$data = WEBPAGE::$dbh->getAll(sprintf("
		SELECT  d.date   ,
        z.zone   ,
        p.program,
        f.fund   ,
        d.amount ,
        d.code
FROM    tblTDebits  AS d           ,
        tblZones    AS z           ,
        tblPrograms AS p           ,
        tblFundsLoansMasterPct flmp,
        tblFunds f
WHERE   z.id           = d.branch_id
    AND p.id           = d.program_id
    AND flmp.master_id = d.id
    AND f.id           = flmp.fund_id
    AND d.date        >= '%s'
    AND d.date        <= '%s'
ORDER BY d.code,d.branch_id,d.date,
        d.program_id", $date_from, $date_to));


$head = array('date'=>$_LABELS['date'],'zone'=>$_LABELS['tblZones.zone'],'program'=>$_LABELS['tblPrograms.program'],'fund'=>$_LABELS['tblFunds.fund'],'amount'=>$_LABELS['tblLoans.kp'],'code'=>$_LABELS['code']);
$tpl->setVariable('rpt_label', 			$_LABELS['transactionsDebits']);
//$tpl->setVariable('rpt_subtitle_label', $_LABELS['transactionsDebits']);
$tpl->setVariable('date_range_label', 	$_LABELS['dateRange']);
$tpl->setVariable('date_range', 		sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : $_LABELS['noData']);

?>
