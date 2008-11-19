<?php
/*
Que hace este script?
Muestra el historial de pagos reales de un prestamo y la proyecccion de los pagos
a partir del final del ultimo pago real.
Si al prestamo no se le ha abonado ningun pago real, entonces muestra
solamente la proyecccion de los pagos.
Si el prestamo aun no ha sido entregado, entonces muestra la proyecccion
de los pagos a partir de la fecha en curso.
*/
require_once '../includes/LN.LIB.functions.inc';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';
require_once 'class/user.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'Date.php';

$loan = new LOAN($_GET['id']);
$pmt = WEBPAGE::$dbh->getAll(sprintf("select p.id as inx,'paid' as status,p.transaction_id,p.date,p.fees,p.insurances,p.principal,p.interest,p.delay,p.penalties,p.pmt,p.balance_kp,u.username from tblPayments as p, tblUsers as u where p.loan_id = %s and u.id = p.user_id order by p.id", $loan->data['id']));

if ((!count($pmt))&&($loan->data['status']!='G')) {
  //Esto es para que prestamos de cheques aun no entregados puedan mostrar un plan de pagos tentativo
  $loan->data['xp_pmt_date']	= $loan->next_payment_date(date('Y-m-d'));
  $loan->data['xp_pmt']			  = $loan->data['pmt'];
  $loan->data['balance_kp']		= $loan->data['kp'];
  $loan->data['balance_kat']	= $loan->data['kat'];
  $loan->data['balance_kaf']	= $loan->data['kaf'];
  $loan->data['r_from_date']	= date('Y-m-d');
  }

$totals = array('inx'=>$_LABELS['total'],'status'=>' ','transaction_id'=>' ','date'=>' ','fees'=>0,'insurances'=>0,'principal'=>0,'interest'=>0,'delay'=>0,'penalties'=>0,'pmt'=>' ','balance_kp'=>' ','user_id'=>' ');

$c = 0;
foreach($pmt as $key => $val) {
  $pmt[$key]['inx']			 		= $c + 1;
  $pmt[$key]['status']			 	= $_LABELS['paid'];
  $totals['fees']					+= $val['fees'];
  $totals['insurances']				+= $val['insurances'];
  $totals['principal']				+= $val['principal'];
  $totals['interest']				+= $val['interest'];
  $totals['delay']					+= $val['delay'];
  $totals['penalties']				+= $val['penalties'];
  $totals['pmt']					+= $val['pmt'];
  $c++;
  }

$balance_kp							= $loan->data['balance_kp'];
$c = count($pmt);

while($balance_kp > 0) {

  $data[$c]							= $loan->getNextPaymentData();

  $loan->data['xp_pmt_date']		= $data[$c]['xp_pmt_date'];
  $loan->data['xp_pmt']				= $data[$c]['xp_pmt'];
  $loan->data['balance_kp']			= $data[$c]['balance_kp'];
  $loan->data['balance_kaf']		= $data[$c]['balance_kaf'];
  $loan->data['balance_kat']		= $data[$c]['balance_kat'];
  $loan->data['r_from_date']		= $data[$c]['r_from_date'];

  $pmt[$c]['inx']					= $c + 1;
  $pmt[$c]['status']				= 'Pending';
  $pmt[$c]['transaction_id']		= '-';
  $pmt[$c]['date']					= $data[$c]['pmt_date'];
  $pmt[$c]['fees']					= $data[$c]['fees'];
  $pmt[$c]['insurances']			= $data[$c]['insurances'];
  $pmt[$c]['principal']				= $data[$c]['principal'];
  $pmt[$c]['interest']				= $data[$c]['interest'];
  $pmt[$c]['delay']					= $data[$c]['delay'];
  $pmt[$c]['penalties']				= $data[$c]['penalties'];
  $pmt[$c]['pmt']					= $data[$c]['pmt'];
  $pmt[$c]['balance_kp']			= $data[$c]['balance_kp'];
  $pmt[$c]['user_id']				= '-';

  $totals['fees']					+= $data[$c]['fees'];
  $totals['insurances']				+= $data[$c]['insurances'];
  $totals['principal']				+= $data[$c]['principal'];
  $totals['interest']				+= $data[$c]['interest'];
  $totals['delay']					+= $data[$c]['delay'];
  $totals['penalties']				+= $data[$c]['penalties'];
  $totals['pmt']					+= $data[$c]['pmt'];

  $balance_kp 						=  $data[$c]['balance_kp'];
  $c++;

  }

$pmt[$c] = $totals;

$loan->load_borrower($loan->data['client_id']);
$ltype = new LOAN_TYPE($loan->data['loan_type_id']);

$tpl->setVariable('cn_date',				$loan->data['cn_date']);
$tpl->setVariable('pmtPlan_label',			$_LABELS['LN.SCR.pmtPlan']);
$tpl->setVariable('borrower_name',			$loan->data['borrower_name']);
$tpl->setVariable('borrower_code',			$loan->data['borrower_code']);
$tpl->setVariable('borrower_label',			$_LABELS['tblClients.id']);
$tpl->setVariable('loan_code',				$loan->data['loan_code']);
$tpl->setVariable('loan_code_label',		$_LABELS['tblLoans.loan_code']);
$tpl->setVariable('modality',				$ltype->data['description']);
$tpl->setVariable('modality_label',			$_LABELS['tblLoanTypes.description']);

$head = array($_LABELS['tblPayments.id'], $_LABELS['status'], $_LABELS['tblPayments.transaction_id'],$_LABELS['tblPayments.date'],$_LABELS['tblPayments.fees'],$_LABELS['tblPayments.insurances'],$_LABELS['tblPayments.principal'],$_LABELS['tblPayments.interest'],$_LABELS['tblPayments.delay'],$_LABELS['tblPayments.penalties'],$_LABELS['tblPayments.pmt'],$_LABELS['tblPayments.balance_kp'],$_LABELS['tblUsers.id']);

//$tpl->setVariable('chart', WEBPAGE::printchart($pmt,$head));

?>