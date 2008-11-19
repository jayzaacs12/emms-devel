<?php
$date_from 	= $_REQUEST['date_from'] 	? $_REQUEST['date_from'] 	: date('Y-m-d');
$date_to 	= $_REQUEST['date_to'] 		? $_REQUEST['date_to'] 		: date('Y-m-d');
$tpl->setVariable('dateRangeSelector', 	$_LABELS['BS.SCR.dateRangeSelector']);
$tpl->setVariable('oldrange', 			sprintf('%s - %s',$date_from,$date_to));

$data = WEBPAGE::$dbh->getAssoc(sprintf("select lsh.loan_id,lsh.date as dateF,l.delivered_date as dateT,(l.kp+l.kat+l.kaf) as balance, l.rates_r as rate, l.zone_id, l.program_id, l.advisor_id, lt.borrower_type, concat(u.first,'&nbsp;',u.last) as advisor from tblLoanStatusHistory as lsh, tblLoans as l, tblLoanTypes as lt, tblUsers as u
                                                where
                                                     u.id = l.advisor_id and
                                                     lt.id = l.loan_type_id and
                                                     l.id = lsh.loan_id and
                                                     lsh.status = 'D' and
                                                     lsh.date <= '%s' and
                                                     (l.delivered_date >= '%s' or l.delivered_date = '0000-00-00')",$date_to,$date_from));

foreach($data as $key=>$val) {

  $val['dateT'] = ($val['dateT'] == '0000-00-00') ? '9999-01-01' : $val['dateT'];
  if ($val['dateF'] >= $val['dateT']) { continue; }
  $dateF = max($date_from,$val['dateF']);
  $dateT = min($date_to,$val['dateT']);
  $adF = explode('-',$dateF);
  $adT = explode('-',$dateT);
  
//  $ldata[$key]['days']             = ((mktime(0,0,0,$adT[1],$adT[2],$adT[0]) - mktime(0,0,0,$adF[1],$adF[2],$adF[0]))/(60*60*24))+1;
//  $ldata[$key]['loss']             = $ldata[$key]['days']*(($val['balance']*$val['rate'])/36500);
//  $ldata[$key]['advisor_id']       = $val['advisor_id'];
//  $ldata[$key]['zone_id']          = $val['zone_id'];
//  $ldata[$key]['program_id']       = $val['program_id'];
//  $ldata[$key]['borrower_type']    = $val['borrower_type'];

  $days             = ((mktime(0,0,0,$adT[1],$adT[2],$adT[0]) - mktime(0,0,0,$adF[1],$adF[2],$adF[0]))/(60*60*24))+1;
  $loss = round($days*(($val['balance']*$val['rate'])/36500),2);

  $ldata[$val['advisor_id']]['advisor']        = $val['advisor'];
  $total['advisor']                            = 'Total';
  
  $ldata[$val['advisor_id']]['B']             += ($val['borrower_type']=='B') ? $loss : 0;
  $ldata[$val['advisor_id']]['I']             += ($val['borrower_type']=='I') ? $loss : 0;
  $ldata[$val['advisor_id']]['T']             += $loss;

  $total['B']                                 += ($val['borrower_type']=='B') ? $loss : 0;
  $total['I']                                 += ($val['borrower_type']=='I') ? $loss : 0;
  $total['T']                                 += $loss;

  }

if (count($ldata)) { $ldata['total'] = $total; }

$head = array('advisor'=>$_LABELS['tblClients.advisor_id'],'B'=>$_LABELS['tblLoanTypes.borrower_type.B'],'I'=>$_LABELS['tblLoanTypes.borrower_type.I'],'T'=>$_LABELS['total']);
$tpl->setVariable('rpt_label', 			$_LABELS['operationalLoss']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.operationalLossByAdvisor']);
$tpl->setVariable('date_range_label', 	$_LABELS['dateRange']);
$tpl->setVariable('date_range', 		sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($ldata) ? WEBPAGE::printchart($ldata,$head) : $_LABELS['noData']);

?>