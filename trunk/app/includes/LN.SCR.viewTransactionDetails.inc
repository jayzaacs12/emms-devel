<?php

  $transac = WEBPAGE::$dbh->getAll(sprintf("select tc.date, z.zone, p.program from tblTCredits as tc, tblZones as z, tblPrograms as p where p.id = tc.program_id and z.id = tc.branch_id and tc.code = '%s'", $_REQUEST['transaction_id']));
  $mrow = WEBPAGE::$dbh->getAll(sprintf("SELECT l.loan_code, concat(c.first, ' ', c.last) as client, p.pmt, p.principal, p.insurances, p.fees, p.interest, p.penalties, p.balance_kp, IF(p.special,'%s','%s') as special, u.username FROM tblLoans as l, tblClients as c, tblPayments as p, tblLoanTypes as lt, tblUsers as u WHERE u.id = p.user_id and p.transaction_id = '%s' and l.id = p.loan_id and c.id = l.client_id and lt.id = l.loan_type_id order by client,p.id",$_LABELS['yes'],$_LABELS['no'],$_REQUEST['transaction_id']));
  $head = array('loan_code'=>$_LABELS['tblLoans.loan_code'],'client'=>$_LABELS['tblClients.id'],'pmt'=>$_LABELS['tblPayments.pmt'],'principal'=>$_LABELS['tblPayments.principal'],'insurances'=>$_LABELS['tblPayments.insurances'],'fees'=>$_LABELS['tblPayments.fees'],'interest'=>$_LABELS['tblPayments.interest'],'penalties'=>$_LABELS['tblPayments.penalties'],'balance_kp'=>$_LABELS['tblPayments.balance_kp'],'special'=>$_LABELS['tblPayments.special'],'user_id'=>$_LABELS['tblUsers.id']);

  $total = array();
  foreach ($mrow as $key=>$val) {
    $mrow[$key]['client'] = str_replace(' ','&nbsp;',$val['client']);
    
    $total['loan_code']		 = $_LABELS['total'];
    $total['client']		 = '-';
    $total['pmt'] 			+= $val['pmt'];
    $total['principal'] 	+= $val['principal'];
    $total['insurances'] 	+= $val['insurances'];
    $total['fees'] 			+= $val['fees'];
    $total['interest'] 		+= $val['interest'];
    $total['penalties'] 	+= $val['penalties'];
    $total['balance_kp']	 = '-';
    $total['special']		 = '-';
    $total['user_id']		 = '-';
    
    }
  $mrow[] = $total;

  $tpl->setVariable('chart', count($total) ? WEBPAGE::printchart($mrow,$head) : $_LABELS['noData']);
  $tpl->setVariable('content_title',		$_LABELS[WEBPAGE::$scr_name]);
  $tpl->setVariable('transaction_label',	$_LABELS['tblPayments.transaction_id']);
  $tpl->setVariable('transaction_value',	$_REQUEST['transaction_id']);
  $tpl->setVariable('date_label',			$_LABELS['date']);
  $tpl->setVariable('date_value',			$transac[0]['date']);
  $tpl->setVariable('branch_label',			$_LABELS['tblZones.zone']);
  $tpl->setVariable('branch_value',			$transac[0]['zone']);
  $tpl->setVariable('program_label',		$_LABELS['tblPrograms.program']);
  $tpl->setVariable('program_value',		$transac[0]['program']);

?>
