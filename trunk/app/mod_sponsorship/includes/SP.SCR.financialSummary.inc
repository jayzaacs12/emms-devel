<?php
$show_gohome = true;

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
            lm.zone_id", WEBPAGE::$userID));

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
            lm.zone_id", WEBPAGE::$userID));

$data['total']['zone'] = $_LABELS['total'];

foreach($data1 as $key=>$val) {

  $data['total']['loans'] 	 	 	+= $val['loans'];
  $data['total']['kp'] 	 	      += $val['kp'];
  $data['total']['avg'] 	 	     = round($data['total']['kp']/$data['total']['loans'],2);
  $data['total']['repayment'] 	+= $val['repayment'];
  $data['total']['balance'] 	 	+= $val['balance'];
  $data['total']['write_off']   += $val['write_off'];
  $data['total']['rendimiento'] += $data2[$val['zone']]['rendimiento'];

  $data[$key]['zone'] 	 	    = $val['zone'];
  $data[$key]['loans'] 	 	 	  = $val['loans'];
  $data[$key]['kp'] 	 	      = $val['kp'];
  $data[$key]['avg'] 	 	      = round($val['kp']/$val['loans'],2);
  $data[$key]['repayment'] 	 	= $val['repayment'];
  $data[$key]['balance'] 	 	  = $val['balance'];
  $data[$key]['write_off']    = $val['write_off'];
  $data[$key]['rendimiento']  = $data2[$val['zone']]['rendimiento'];

}

$data = array_reverse($data);

$extra['zone']['header']	       = 'tblZones.zone';
$extra['zone']['pack']	         = '%s';
$extra['zone']['align']	         = 'right';

$extra['loans']['header']		     = 'loans';
$extra['loans']['pack']		       = '%s';
$extra['loans']['align']		     = 'right';

$extra['kp']['header']	         = 'amount_disbursed';
$extra['kp']['pack']	           = '%s';
$extra['kp']['align']	           = 'right';

$extra['avg']['header']	         = 'loan_amount_average';
$extra['avg']['pack']	           = '%s';
$extra['avg']['align']	         = 'right';

$extra['repayment']['header']	   = 'principalRepayment';
$extra['repayment']['pack']		   = '%s';
$extra['repayment']['align']	   = 'right';
  
$extra['balance']['header']	     = 'portfolio_balance';
$extra['balance']['pack']		     = '%s';
$extra['balance']['align']		   = 'right';

$extra['write_off']['header']	   = 'writeOff_amount';
$extra['write_off']['pack']	     = '%s';
$extra['write_off']['align']	   = 'right';

$extra['rendimiento']['header']	 = 'operationalIncome';
$extra['rendimiento']['pack']		 = '%s';
$extra['rendimiento']['align']	 = 'right';

$extra['param']['ref']		       = WEBPAGE::$scr_name;

array_unshift($data,$extra);

WEBPAGE::makecachefile($data,WEBPAGE::$queryCache);
require_once '../includes/BS.SCR.pager.inc';

?>