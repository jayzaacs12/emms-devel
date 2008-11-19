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

if ($loan->data['xp_pmt_date'] <= $loan->data['cn_date']) { 
  WEBPAGE::redirect(sprintf("index.popup.php?scr_name=LN.SCR.addPaymentDue&id=%s", $loan->data['id'])); 
  }

$tpl->setVariable('title', $_LABELS['LN.SCR.addPayment']);

$tpl->setVariable('loan_code_label',		$_LABELS['tblLoans.loan_code']);
$tpl->setVariable('loan_code',				$loan->data['loan_code']);
$tpl->setVariable('client_label',			$_LABELS['tblClients.id']);
$tpl->setVariable('borrower_name',			$loan->data['borrower_name']);
$tpl->setVariable('borrower_code',			$loan->data['borrower_code']);

$tpl->setVariable('loan_id', 				$loan->data['id']);

$tpl->setVariable('pickOptionToContinue',	$_LABELS['pickOptionToContinue']);
$tpl->setVariable('addPaymentEarly', 		$_LABELS['LN.SCR.addPaymentEarly']);
$tpl->setVariable('addPaymentXT', 			$_LABELS['LN.SCR.addPaymentXT']);
$tpl->setVariable('addPaymentFull', 		$_LABELS['LN.SCR.addPaymentFull']);

$tpl->setVariable('message', 				WEBPAGE::printmessage('err',sprintf('%s<br>%s',$_LABELS['noPendingPayments'],$_LABELS['payEarly.inf'])));

?>