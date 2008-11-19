<?php

$sponsor_donations = WEBPAGE::$dbh->getAll(sprintf("
SELECT  sp.id                        AS sponsor_id   ,
        sp.sponsor                   AS display_name ,
        concat(sp.first,' ',sp.last) AS contact_name ,
        sp.email                     AS contact_email,
        SUM(spd.src_amount)          AS donations    ,
        spd.src_currency_id          AS currency_id  ,
        spd.symbol                   AS currency_symbol
FROM (tblSponsors AS sp)
        LEFT JOIN
                (SELECT spd.id              ,
                        spd.sponsor_id      ,
                        spd.src_amount      ,
                        spd.src_currency_id ,
                        cu.symbol
                FROM    tblSponsorsDonations spd,
                        tblCurrencys cu
                WHERE   cu.id = spd.src_currency_id
                ) AS spd
        ON (spd.sponsor_id = sp.id)
WHERE   1
GROUP BY sp.id,
        spd.src_currency_id;
"));



// SQL original del e-MMS --con subselect--
$portfolio = WEBPAGE::$dbh->getAssoc("
SELECT  lm.sponsor_id,
        COUNT(lmd.loan_id) loans                             ,
        SUM(l.kp) kp                                         ,
        IF(SUM(lcd.balance_kp),SUM(lcd.balance_kp),0) balance,
        IF(SUM(lwo.amount),SUM(lwo.amount),0) write_off      ,
        SUM(l.kp) - IF(SUM(lcd.balance_kp),SUM(lcd.balance_kp),0) - IF(SUM(lwo.amount),SUM(lwo.amount),0) repayment
FROM (tblLoansMaster lm, tblLoansMasterDetails lmd, tblLoans l)
        LEFT JOIN tblLoansCurrentData lcd
        ON      lcd.loan_id = lmd.loan_id
        LEFT JOIN tblLoanWriteOff lwo
        ON      lwo.loan_id = lmd.loan_id
WHERE   lm.check_status     = 'R'
    AND lmd.master_id       = lm.id
    AND l.id                = lmd.loan_id
    AND lm.sponsor_id    IN
        (SELECT tblSponsors.id
        FROM    tblSponsors
        WHERE   tblSponsors.active=1
        )
   GROUP BY lm.sponsor_id;
");



// SQL original del e-MMS --con subselect--
$operational_income = WEBPAGE::$dbh->getAssoc(sprintf("SELECT  lm.sponsor_id,
        IF(SUM(p.principal),SUM(p.principal),0) principal,
        IF(SUM(p.interest)+SUM(p.fees)+SUM(insurances)+SUM(penalties),SUM(p.interest)+SUM(p.fees)+SUM(insurances)+SUM(penalties),0) operational_income
FROM (tblLoansMaster lm, tblLoansMasterDetails lmd)
        LEFT JOIN tblPayments p
        ON      p.loan_id = lmd.loan_id
WHERE   lm.check_status   = 'R'
    AND lm.sponsor_id    IN
        (SELECT tblSponsors.id
        FROM    tblSponsors
        WHERE   tblSponsors.active=1
        )
    AND lmd.master_id = lm.id
GROUP BY lm.sponsor_id;"));



// Getting all registered currencies

$tblCurrency = WEBPAGE::$dbh->getAssoc(sprintf("
select * from tblCurrencys where 1;
    "));

// Setting up donation* headers dynamically
$head['sponsor_id'] ='#';
$head['display_name'] = $_LABELS['tblSponsors.sponsor'];
$head['contact_name'] = $_LABELS['tblSponsors.contact'];
$head['contact_email'] = $_LABELS['tblClients.email'];

foreach ($tblCurrency AS $currency_id => $value) {
    $head[sprintf("donations_sorted_%s",$currency_id)] = sprintf("%s %s",$_LABELS['donations'],$value['symbol']);
}

$head['loans_count'] = $_LABELS['loans'];
$head['operational_income'] = $_LABELS['RP.GRP.Performance.operationalIncome'];
$head['writeoff'] = $_LABELS['loanWriteOff'];

// Pre-populate $total

$total['sponsor_id']=$_LABELS['total'];
$total['display_name']='-';
$total['contact_name']='-';
$total['contact_email']='-';            

foreach ($tblCurrency as $currency_id => $currency) {                
        $total[sprintf("donations_sorted_%s",$currency_id)] = 0; 
}

$total['loans_count'] = 0;
$total['operational_income'] = 0;
$total['writeoff'] = 0;    

// Pre-populate $mrow as per donations
foreach ($sponsor_donations AS $key => $record) {
    $mrow[$record['sponsor_id']]['sponsor_id'] = $record['sponsor_id'];
    $mrow[$record['sponsor_id']]['display_name'] = $record['display_name'];
    $mrow[$record['sponsor_id']]['contact_name'] = $record['contact_name'];
    $mrow[$record['sponsor_id']]['contact_email'] = $record['contact_email'];
    
    if ($record['currency_id']!=null) {
        $mrow[$record['sponsor_id']][sprintf("donations_%s",$record['currency_id'])] = max(0,$record['donations']);
    }
    
}

//  Sorting donations_* keys in $mrow
foreach ($mrow as $sponsor_id => $record) {
    
    foreach ($tblCurrency AS $currency_id => $entry) {
        $mrow[$sponsor_id][sprintf("donations_sorted_%s",$currency_id)]=max(0,$mrow[$sponsor_id][sprintf("donations_%s",$currency_id)]);
        unset($mrow[$sponsor_id][sprintf("donations_%s",$currency_id)]);
    }
    
}

 

// Start populating $mrow with portfolio data
foreach ($mrow AS $key => $value) {
            
            $mrow[$key]['loans_count']=max(0,$portfolio[$key]['loans']);
            $mrow[$key]['operational_income']=max(0,$operational_income[$key]['operational_income']);
            $mrow[$key]['writeoff']=max(0,$portfolio[$key]['write_off']);
        
             // Calculating totals
            $total['sponsor_id']=$_LABELS['total'];
            $total['display_name']='-';
            $total['contact_name']='-';
            $total['contact_email']='-';
            
            
            // Calculating Donations Totals
            
      foreach ($tblCurrency as $currency_id => $currency) {
                        
            $total[sprintf("donations_sorted_%s",$currency_id)] += max(0,$mrow[$key][sprintf("donations_sorted_%s",$currency_id)]);
             
      }
                    
            $total['loans_count']+=max(0,$portfolio[$key]['loans']);
            $total['operational_income']+=max(0,$operational_income[$key]['operational_income']);
            $total['writeoff']+=max(0,$portfolio[$key]['write_off']);

}


$mrow['total']=$total;





  $tpl->setVariable('script_name_label',$_LABELS['RP.SCR.performance']);
  //$tpl->setVariable('sponsor_name_value',$sponsor[0]['sponsor']);
  $tpl->setVariable('chart', count($mrow)>1 ? WEBPAGE::printchart($mrow,$head) : $_LABELS['noData']);


?>

