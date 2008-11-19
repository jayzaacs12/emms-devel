<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';

$master 	= new LOAN_MASTER($_REQUEST['master_id']);  
$master->loadBorrowerData(); 

$tpl->setVariable('zone_label',				$_LABELS['tblZones.zone']);
$tpl->setVariable('zone',					$master->data['borrower']->data['zone']);
$tpl->setVariable('borrower_label',			$_LABELS['tblClients.id']);
$tpl->setVariable('borrower',				$master->data['borrower']->data['name']);
$tpl->setVariable('loan_type_label',		$_LABELS['tblLoansMaster.loan_type_id']);
$tpl->setVariable('loan_type',				$loan->data['modality']);
$tpl->setVariable('check_number_label',		$_LABELS['tblLoansMaster.check_number']);
$tpl->setVariable('check_number',			$master->data['check_number']);

$pdata = WEBPAGE::$dbh->getAll(sprintf("select max(p.id) as code, p.date, sum(p.pmt) as pmt, sum(p.penalties) as penalties, sum(p.interest) as interest, 
		sum(p.insurances) as insurances, sum(p.fees) as fees, sum(p.principal) as principal from tblPayments as p, tblLoansMasterDetails as lmd where lmd.master_id = %s and p.loan_id = lmd.loan_id and p.id > %s group by lmd.master_id", $_REQUEST['master_id'], $_REQUEST['last_pID']));
$bdata = WEBPAGE::$dbh->getAll(sprintf("select min(p.balance_kp) as balance_kp, min(p.balance_kaf) as balance_kaf, min(p.balance_kat) as balance_kat 
		from tblPayments as p, tblLoansMasterDetails as lmd where lmd.master_id = %s and p.loan_id = lmd.loan_id and p.id <= %s group by p.loan_id", $_REQUEST['master_id'], $pdata[0]['code']));
$ndata = WEBPAGE::$dbh->getAll(sprintf("select sum(l.kp) as kp, sum(l.kaf) as kaf, sum(l.kat) as kat from (tblLoans as l, tblLoansMasterDetails as lmd) LEFT JOIN tblPayments as p on p.loan_id = l.id where p.loan_id is null and l.id = lmd.loan_id and lmd.master_id = %s",$_REQUEST['master_id']));

$ldata = WEBPAGE::$dbh->getAssoc(sprintf("select p.loan_id,l.loan_code from tblLoansMasterDetails as lmd, tblLoans as l, tblPayments as p where p.id > %s and p.loan_id = l.id and lmd.master_id = %s and l.id = lmd.loan_id", $_REQUEST['last_pID'], $_REQUEST['master_id']));

$total_amount = $pdata[0]['principal'] + $pdata[0]['interest'] + $pdata[0]['insurances'] + $pdata[0]['fees'] + $pdata[0]['penalties'];

$tpl->setVariable('organization',		$_CONF['client_name']);
$tpl->setVariable('address',			$_CONF['client_address']);

$tpl->setVariable('date_label',			$_LABELS['date']);
$tpl->setVariable('date',				$pdata[0]['date']);
$tpl->setVariable('code_label',			$_LABELS['code']);
$tpl->setVariable('code',				sprintf('%s.%s',$master->data['check_number'],$pdata[0]['code']));
$tpl->setVariable('loan_label',			$_LABELS['tblLoans.id']);
$tpl->setVariable('loan',				implode('<br>',$ldata));


$tpl->setVariable('page_title',			$_LABELS['LN.SCR.paymentReceipt']);
$tpl->setVariable('paymentReceipt',		sprintf($_LABELS['paymentReceipt'],$master->data['borrower']->data['name'],$total_amount));

$data[0] = array('total' => $total_amount, 'principal'=>$pdata[0]['principal'],'interest' => $pdata[0]['interest'],'insurances' => $pdata[0]['insurances'], 'fees' => $pdata[0]['fees'], 'penalties' => $pdata[0]['penalties']);
$head = array('total' => $_LABELS['total'], 'principal'=>$_LABELS['tblPayments.principal'],'interest' => $_LABELS['tblPayments.interest'],'insurances' => $_LABELS['tblPayments.insurances'], 'fees' => $_LABELS['tblPayments.fees'], 'penalties' => $_LABELS['tblPayments.penalties'], 'balance_kp'=>$_LABELS['tblPayments.balance_kp'], 'balance_kaf'=>$_LABELS['tblPayments.balance_kaf'], 'balance_kat'=>$_LABELS['tblPayments.balance_kat']);

foreach ($bdata as $key=>$val) {
  $data[0]['balance_kp'] += $val['balance_kp'];
  $data[0]['balance_kaf'] += $val['balance_kaf'];
  $data[0]['balance_kat'] += $val['balance_kat'];
  }
$data[0]['balance_kp']  += $ndata[0]['kp'];
$data[0]['balance_kaf'] += $ndata[0]['kaf'];
$data[0]['balance_kat'] += $ndata[0]['kat'];

$tpl->setVariable('details_label', 		$_LABELS['details']);
$tpl->setVariable('details', 			WEBPAGE::printchart($data,$head));
$tpl->setVariable('print', 				sprintf("<a href='javascript:print()'>%s</a>",$_LABELS['printSheet']));

?>