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

//# cuotas ya pagadas
$c = max(0,current(WEBPAGE::$dbh->getAssoc(sprintf("select loan_id,count(id) from tblPayments where loan_id = %s group by loan_id",$loan->data['id']))));

$totals = array('inx'=>$_LABELS['total'],'date'=>'-','fees'=>0,'insurances'=>0,'principal'=>0,'interest'=>0,'delay'=>0,'penalties'=>0,'pmt'=>0,'balance_kp'=>'-');

$balance_kp							= $loan->data['balance_kp'];

while($loan->data['xp_pmt_date'] < $loan->data['cn_date']) {

  $data[$c]							= $loan->getNextPaymentData();

  $loan->data['xp_pmt_date']		= $data[$c]['xp_pmt_date'];
  $loan->data['xp_pmt']				= $data[$c]['xp_pmt'];
  $loan->data['balance_kp']			= $data[$c]['balance_kp'];
  $loan->data['balance_kaf']		= $data[$c]['balance_kaf'];
  $loan->data['balance_kat']		= $data[$c]['balance_kat'];
  $loan->data['r_from_date']		= $data[$c]['r_from_date'];

  $pmt[$c]['inx']					= $c + 1;
  $pmt[$c]['date']					= $data[$c]['pmt_date'];
  $pmt[$c]['fees']					= $data[$c]['fees'];
  $pmt[$c]['insurances']			= $data[$c]['insurances'];
  $pmt[$c]['principal']				= $data[$c]['principal'];
  $pmt[$c]['interest']				= $data[$c]['interest'];
  $pmt[$c]['delay']					= $data[$c]['delay'];
  $pmt[$c]['penalties']				= $data[$c]['penalties'];
  $pmt[$c]['pmt']					= $data[$c]['pmt'];
  $pmt[$c]['balance_kp']			= $data[$c]['balance_kp'];

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

$date = new Date($loan->data['r_from_date']);
$n = 0;
while ($date->format('%Y-%m-%d') != $loan->data['cn_date']) {
  $date->addSeconds(24*60*60);
  $n++;
  }

$pmt[$c]['inx']					= $c + 1;
$pmt[$c]['date']				= $loan->data['cn_date'];
$pmt[$c]['fees']				= $loan->data['balance_kat'];
$pmt[$c]['insurances']			= $loan->data['balance_kaf'];
$pmt[$c]['principal']			= $loan->data['balance_kp'];
$pmt[$c]['interest']			= round((($pmt[$c]['fees']+$pmt[$c]['insurances']+$pmt[$c]['principal'])*(0.01*$loan->data['rates_r']/$loan->data['calendar_type'])*$n),2);
$pmt[$c]['delay']				= 0;
$pmt[$c]['penalties']			= 0;
$pmt[$c]['pmt']					= $pmt[$c]['fees']+$pmt[$c]['insurances']+$pmt[$c]['principal']+$pmt[$c]['interest'];
$pmt[$c]['balance_kp']			= 0;

$totals['fees']					+= $pmt[$c]['fees'];
$totals['insurances']			+= $pmt[$c]['insurances'];
$totals['principal']			+= $pmt[$c]['principal'];
$totals['interest']				+= $pmt[$c]['interest'];
$totals['pmt']					+= $pmt[$c]['pmt'];

$idata = $pmt;

$pmt[$c+1] = $totals;

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('applyForm','POST');

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'ref',			$_REQUEST['ref']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);

$form->addElement('text',		'notes',		$_LABELS['memo'], 'class=large');

$_CONF['pmt_receipt_flag_a'] ?  $form->addElement('advcheckbox','pmt_receipt_flag_a',	'', $_CONF['pmt_receipt_flag_a'],'class=checkbox') : '';
$_CONF['pmt_receipt_flag_b'] ?  $form->addElement('advcheckbox','pmt_receipt_flag_b',	'', $_CONF['pmt_receipt_flag_b'],'class=checkbox') : '';

$form->addElement('static', 	'submit', 		'', $btn->imgsubmit('applyForm',$_LABELS['submit']));

if ($form->validate()) {

  $receipt_data = WEBPAGE::$dbh->getAll(sprintf("select max(p.id) as last_pID from tblPayments as p, tblLoansMasterDetails as lmd, tblLoans as l where lmd.master_id = %s and p.loan_id = lmd.loan_id", $loan->data['master_id']));

  foreach($idata as $key=>$val) {
  	WEBPAGE::$dbh->query(sprintf("insert into tblPayments (loan_id,date,pmt,penalties,delay,interest,insurances,fees,principal,balance_kp,balance_kaf,balance_kat,user_id) values ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",$loan->data['id'],$loan->data['cn_date'],$val['pmt'],$val['penalties'],$val['delay'],$val['interest'],$val['insurances'],$val['fees'],$val['principal'],$val['balance_kp'],$val['balance_kaf'],$val['balance_kat'],WEBPAGE::$userID));
  	$pmtIDs[] = mysql_insert_id(WEBPAGE::$dbh->connection);
    }
  $loan->data['balance_kp'] = 0;
  $loan->cancelled();

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

//  WEBPAGE::$dbh->query(sprintf("delete from tblLoansCurrentData where loan_id = %s", $loan->data['id']));
//  WEBPAGE::$dbh->query(sprintf("insert into tblLoanStatusHistory (loan_id,p_status,status,date,user_id,memo) values ('%s','%s','%s','%s','%s','%s')",$loan->data['id'],$loan->data['status'],'C',$loan->data['cn_date'],WEBPAGE::$userID,$_LABELS['LN.SCR.addPaymentFull']));
//  WEBPAGE::$dbh->query(sprintf("update tblLoans set status = 'C' where id = %s", $loan->data['id']));

//  WEBPAGE::redirect(sprintf("index.popup.php?scr_name=LN.SCR.paymentReceipt&master_id=%s&last_pID=%s",  $loan->data['master_id'],max(0,$receipt_data[0]['last_pID'])));

  } else {

  $tpl->setVariable('form_name',			$_LABELS['LN.SCR.addPaymentFull']);

  $tpl->setVariable('message', 				WEBPAGE::printmessage('wrn',$_LABELS['confirm']));

  $tpl->setVariable('loan_code_label',		$_LABELS['tblLoans.loan_code']);
  $tpl->setVariable('loan_code',			$loan->data['loan_code']);
  $tpl->setVariable('client_label',			$_LABELS['tblClients.id']);
  $tpl->setVariable('borrower_name',		$loan->data['borrower_name']);
  $tpl->setVariable('borrower_code',		$loan->data['borrower_code']);


  $head = array($_LABELS['tblPayments.id'],$_LABELS['tblPayments.date'],$_LABELS['tblPayments.fees'],$_LABELS['tblPayments.insurances'],$_LABELS['tblPayments.principal'],$_LABELS['tblPayments.interest'],$_LABELS['tblPayments.delay'],$_LABELS['tblPayments.penalties'],$_LABELS['tblPayments.pmt'],$_LABELS['tblPayments.balance_kp']);
  $tpl->setVariable('due_chart', 		WEBPAGE::printchart($pmt,$head));

  $tpl->setVariable('total_pmt', 		$totals['pmt']);
  $tpl->setVariable('total_pmt_label',	$_LABELS['total']);

  $form->setDefaults();
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $form->accept($renderer);

  }

?>