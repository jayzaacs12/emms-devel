<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'Date.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/user.php';
require_once 'class/client.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';

$loan = new LOAN($_REQUEST['id']);
$loan->load_borrower($loan->data['client_id']);
if (!count(WEBPAGE::$dbh->getAll(sprintf("select loan_id from tblLoansCurrentData where loan_id = %s", $loan->data['id'])))) {
  echo $_LABELS['applyPayment.err']; exit;
  }
if ($loan->data['xp_pmt_date'] < $loan->data['cn_date']) {
  echo $_LABELS['applyPayment.err']; exit;
  }

//# cuotas ya pagadas
$c = current(WEBPAGE::$dbh->getAssoc(sprintf("select loan_id,count(id) from tblPayments where loan_id = %s group by loan_id",$loan->data['id'])));

$totals = array('inx'=>$_LABELS['total'],'date'=>'-','fees'=>0,'insurances'=>0,'principal'=>0,'interest'=>0,'delay'=>0,'penalties'=>0,'pmt'=>0,'balance_kp'=>'-');

$balance						= $loan->data['balance_kp'] + $loan->data['balance_kaf'] + $loan->data['balance_kat'];
$date 							= new Date($loan->data['r_from_date']);
$n = 0;
while ($date->format('%Y-%m-%d') != $loan->data['cn_date']) {
  $date->addSeconds(24*60*60);
  $n++;
  }

$pmt[$c]['inx']					= $c + 1;
$pmt[$c]['date']				= $loan->data['cn_date'];
$pmt[$c]['fees']				= 0;
$pmt[$c]['insurances']			= 0;
$pmt[$c]['principal']			= 0;
$pmt[$c]['interest']			= round(($balance*(0.01*$loan->data['rates_r']/$loan->data['calendar_type'])*$n),2);
$pmt[$c]['delay']				= 0;
$pmt[$c]['penalties']			= 0;
$pmt[$c]['pmt']					= $pmt[$c]['interest'];
$pmt[$c]['balance_kp']			= $loan->data['balance_kp'];

$totals['fees']					= 0;
$totals['insurances']			= 0;
$totals['principal']			= 0;
$totals['interest']				= $pmt[$c]['interest'];
$totals['delay']				= 0;
$totals['penalties']			= 0;
$totals['pmt']					= $pmt[$c]['interest'];

$pmt[$c+1] = $totals;

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('applyForm','POST');

$defaults = array('pmt_min'=>$totals['pmt'], 'pmt_max'=>$totals['pmt']+$balance);

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'ref',			$_REQUEST['ref']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);
$form->addElement('static',		'pmt_min',		$_LABELS['pmtMin']);
$form->addElement('static',		'pmt_max',		$_LABELS['pmtMax']);
$form->addElement('text',		'amount',		$_LABELS['amount'],$_REQUEST['amount']);
$form->addElement('text',		'notes',		$_LABELS['memo'], 'class=large');
$form->addElement('static', 	'submit', 		'', $btn->imgsubmit('applyForm',$_LABELS['submit']));

$form->addRule('amount',	$_LABELS['required'], 	'required');
$form->addRule('amount',	sprintf($_LABELS['numericRange'],$defaults['pmt_min'],$defaults['pmt_max']), 'numeric');
$form->addRule('amount',	sprintf($_LABELS['numericRange'],$defaults['pmt_min'],$defaults['pmt_max']), 'callback', 'checkamount');
function checkamount() {
  global $defaults;
  if ($_REQUEST['amount'] < $defaults['pmt_min']) 	{ return false; }
  if ($_REQUEST['amount'] > $defaults['pmt_max']) 	{ return false; }
  if ($_REQUEST['amount'] <= 0) 					{ return false; }
  return true;
  }


if ($form->validate()) {

  $receipt_data = WEBPAGE::$dbh->getAll(sprintf("select max(p.id) as last_pID from tblPayments as p, tblLoansMasterDetails as lmd, tblLoans as l where lmd.master_id = %s and p.loan_id = lmd.loan_id", $loan->data['master_id']));

  $data['pmt'] 			= $_REQUEST['amount'];
  $data['penalties'] 	= 0;
  $data['delay'] 		= 0;
  $data['interest'] 	= $pmt[$c]['interest'];
  $data['insurances']	= round($loan->data['P_KAF']*($_REQUEST['amount'] - $data['interest']),2);
  $data['fees']			= round($loan->data['P_KAT']*($_REQUEST['amount'] - $data['interest']),2);
  $data['principal']	= $_REQUEST['amount'] - $data['interest'] - $data['insurances'] - $data['fees'];
  $data['balance_kp']	= $loan->data['balance_kp'] 	- $data['principal'];
  $data['balance_kaf']	= $loan->data['balance_kaf'] 	- $data['insurances'];
  $data['balance_kat']	= $loan->data['balance_kat'] 	- $data['fees'];

  $loan->data['balance_kp']			= $data['balance_kp'];
  $loan->data['balance_kaf']		= $data['balance_kaf'];
  $loan->data['balance_kat']		= $data['balance_kat'];
  $loan->data['r_from_date']		= $loan->data['cn_date'];

  $ndata = $loan->getNextPaymentData();

  WEBPAGE::$dbh->query(sprintf("insert into tblPayments (loan_id,date,pmt,penalties,delay,interest,insurances,fees,principal,balance_kp,balance_kaf,balance_kat,special,user_id) values ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','1','%s')",$loan->data['id'],$loan->data['cn_date'],$data['pmt'],$data['penalties'],$data['delay'],$data['interest'],$data['insurances'],$data['fees'],$data['principal'],$data['balance_kp'],$data['balance_kaf'],$data['balance_kat'],WEBPAGE::$userID));
  $pmtIDs[] = mysql_insert_id(WEBPAGE::$dbh->connection);
  WEBPAGE::$dbh->query(sprintf("update tblLoansCurrentData set balance_kp = '%s', balance_kaf = '%s', balance_kat = '%s', r_from_date = '%s', xp_pmt = '%s', cn_delay = 0, cn_penalties	= 0 where loan_id = '%s'", $data['balance_kp'],$data['balance_kaf'],$data['balance_kat'],$loan->data['cn_date'],$ndata['pmt'],$loan->data['id']));

  $loan->data['balance_kp'] ? '' : $loan->cancelled();

// add receipt stuff
  if(( $n = count($pmtIDs)) > 0) {
  	$balance = WEBPAGE::$dbh->getAll(sprintf("select lmd.master_id, sum(lcd.balance_kp) balance_kp, sum(lcd.balance_kat) balance_kat, sum(balance_kaf) balance_kaf from tblLoansCurrentData lcd, tblLoansMasterDetails lmd where lmd.master_id = %s and lcd.loan_id = lmd.loan_id group by lmd.master_id",$loan->data['master_id']));
    if (count($balance) != 1) {
      $balance_kp = 0; $balance_kaf = 0; $balance_kat = 0;
      } else {
      $balance_kp = $balance[0]['balance_kp']; $balance_kaf = $balance[0]['balance_kaf']; $balance_kat = $balance[0]['balance_kat'];
      }
    $receipt_id = SQL::insert('tblReceipts',array('loanmaster_id'=>$loan->data['master_id'],'balance_kp'=>$balance_kp,'balance_kaf'=>$balance_kaf,'balance_kat'=>$balance_kat,'notes'=>$_REQUEST['notes'],'flag_a'=>$_REQUEST['pmt_receipt_flag_a'] ? 1 : 0,'flag_b'=>$_REQUEST['pmt_receipt_flag_b'] ? 1 : 0,'user_id'=>WEBPAGE::$userID,'date'=>date('Y-m-d')));
    $lnkData = array();
    foreach($pmtIDs as $key=>$val) {
      $lnkData[] = sprintf("(%s,%s)",$receipt_id,$val);
      }
    SQL::insert_mult('tblLinkReceiptsPayments','receipt_id,payment_id',implode(',',$lnkData));
    WEBPAGE::redirect(sprintf("index.popup.php?scr_name=LN.SCR.viewReceipt&id=%s",$receipt_id));
    }

//  WEBPAGE::redirect(sprintf("index.popup.php?scr_name=LN.SCR.paymentReceipt&master_id=%s&last_pID=%s",  $loan->data['master_id'],max(0,$receipt_data[0]['last_pID'])));

  } else {

  $tpl->setVariable('form_name',			$_LABELS['LN.SCR.addPaymentXT']);
  $tpl->setVariable('loan_code_label',		$_LABELS['tblLoans.loan_code']);
  $tpl->setVariable('loan_code',			$loan->data['loan_code']);
  $tpl->setVariable('client_label',			$_LABELS['tblClients.id']);
  $tpl->setVariable('borrower_name',		$loan->data['borrower_name']);
  $tpl->setVariable('borrower_code',		$loan->data['borrower_code']);


  $head = array($_LABELS['tblPayments.id'],$_LABELS['tblPayments.date'],$_LABELS['tblPayments.fees'],$_LABELS['tblPayments.insurances'],$_LABELS['tblPayments.principal'],$_LABELS['tblPayments.interest'],$_LABELS['tblPayments.delay'],$_LABELS['tblPayments.penalties'],$_LABELS['tblPayments.pmt'],$_LABELS['tblPayments.balance_kp']);
  $tpl->setVariable('due_chart', 		WEBPAGE::printchart($pmt,$head));

  $tpl->setVariable('total_pmt', 		$totals['pmt']);
  $tpl->setVariable('total_pmt_label',	$_LABELS['total']);

  $form->setDefaults($defaults);
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $form->accept($renderer);

  }

?>
