<?php

  $sponsor = WEBPAGE::$dbh->getAll(sprintf("select sp.sponsor from tblSponsors sp where sp.id =%s",$_REQUEST['id']));

  $data1 = WEBPAGE::$dbh->getAll(sprintf("
         select
            z.zone,
            count(lmd.loan_id) loans,
            sum(l.kp) kp,
            if(sum(lcd.balance_kp),sum(lcd.balance_kp),0) balance,
            if(sum(lwo.amount),sum(lwo.amount),0) write_off,
            sum(l.kp) - if(sum(lcd.balance_kp),sum(lcd.balance_kp),0) - if(sum(lwo.amount),sum(lwo.amount),0) repayment
       from
            (tblZones z, tblLoansMaster lm, tblLoansMasterDetails lmd, tblLoans l)
       left join
            tblLoansCurrentData lcd on lcd.loan_id = lmd.loan_id
       left join
            tblLoanWriteOff lwo on lwo.loan_id = lmd.loan_id
       where
            lm.check_status = 'R' and
            lm.sponsor_id = %s and
            lmd.master_id = lm.id and
            z.id = lm.zone_id and
            l.id = lmd.loan_id
       group by
            lm.zone_id",$_REQUEST['id']));

$data2 = WEBPAGE::$dbh->getAssoc(sprintf("
         select
            z.zone,
            if(sum(p.principal),sum(p.principal),0) principal,
            if(sum(p.interest)+sum(p.fees)+sum(insurances)+sum(penalties),sum(p.interest)+sum(p.fees)+sum(insurances)+sum(penalties),0) rendimiento
       from
            (tblZones z,
            tblLoansMaster lm,
            tblLoansMasterDetails lmd)
       left join
            tblPayments p on p.loan_id = lmd.loan_id
       where
            lm.check_status = 'R'
            and lm.sponsor_id = %s and
            lmd.master_id = lm.id and
            z.id = lm.zone_id
       group by
            lm.zone_id", $_REQUEST['id']));

  $mrow['total']['zone'] = $_LABELS['total'];

foreach($data1 as $key=>$val) {

  $mrow['total']['loans']           += $val['loans'];
  $mrow['total']['kp']            += $val['kp'];
  $mrow['total']['avg']              = round($mrow['total']['kp']/$mrow['total']['loans'],2);
  $mrow['total']['repayment']   += $val['repayment'];
  $mrow['total']['balance']         += $val['balance'];
  $mrow['total']['write_off']   += $val['write_off'];
  $mrow['total']['rendimiento'] += $data2[$val['zone']]['rendimiento'];

  $mrow[$key]['zone']           = $val['zone'];
  $mrow[$key]['loans']            = $val['loans'];
  $mrow[$key]['kp']               = $val['kp'];
  $mrow[$key]['avg']              = round($val['kp']/$val['loans'],2);
  $mrow[$key]['repayment']      = $val['repayment'];
  $mrow[$key]['balance']          = $val['balance'];
  $mrow[$key]['write_off']    = $val['write_off'];
  $mrow[$key]['rendimiento']  = $data2[$val['zone']]['rendimiento'];

}

$head = array(
    'zone'  => $_LABELS['tblZones.zone'],
    'loans' => $_LABELS['loans'],
    'kp' => $_LABELS['amount_disbursed'],
    'avg' => $_LABELS['loan_amount_average'],
    'repayment' => $_LABELS['principalRepayment'],
    'balance' => $_LABELS['portfolio_balance'],
    'write_off' => $_LABELS['writeOff_amount'],
    'rendimiento' => $_LABELS['operationalIncome']
);

$mrow = array_reverse($mrow);

  $tpl->setVariable('sponsor_name_label',$_LABELS['tblSponsors.sponsor']);
  $tpl->setVariable('sponsor_name_value',$sponsor[0]['sponsor']);
  $tpl->setVariable('chart', count($mrow)>1 ? WEBPAGE::printchart($mrow,$head) : $_LABELS['noData']);
?>
