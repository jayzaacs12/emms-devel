<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'Date.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/roles.php';
require_once 'class/client.php';
require_once 'class/society.php';
require_once 'class/business_type.php';
require_once 'class/business.php';
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';

$loan  = new LOAN($_REQUEST['id']);
$loan->load_legal_data();
$cbalance = $loan->data['legal']['current_balance'];

($loan->data['status'] != 'LI') ? exit : '';

extract(LOAN::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('title',					           $_LABELS['LG.SCR.legalProfile']);
$tpl->setVariable('loan_code',					       $loan_code);
$tpl->setVariable('loan_code_label',		       $_LABELS['tblLoans.id']);
$tpl->setVariable('status',						         $_LABELS[sprintf("tblLoans.status.%s",$status)]);
$tpl->setVariable('status_label',				       $_LABELS['tblLoans.status']);
$tpl->setVariable('li_cause',						       $loan->data['legal']['cause']);
$tpl->setVariable('li_cause_label',				     $_LABELS['tblLoansParked.category']);
$tpl->setVariable('borrower_name',				     $borrower_name);
$tpl->setVariable('borrower_name_label',	     $_LABELS['tblClients.id']);
$tpl->setVariable('borrower_code',				     $borrower_code);
$tpl->setVariable('borrower_code_label',	     $_LABELS['tblClients.code']);

$form = new HTML_QuickForm('lpaymentForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('lpaymentForm',$_LABELS['submit']));
$form->addElement('hidden',	  'scr_name',		  $_REQUEST['scr_name']);
$form->addElement('hidden',	  'id',			      $_REQUEST['id']);
$form->addElement('text',		  'amount',			  $_LABELS['amount'], 'class=large');
$form->addRule('amount',	$_LABELS['required'], 	'required');
$form->addRule('amount',	sprintf($_LABELS['numericRange'],0.00,$cbalance['total']), 'numeric');
$form->addRule('amount',	sprintf($_LABELS['numericRange'],0.00,$cbalance['total']), 'callback', 'checkamount');
function checkamount() {
  global $cbalance;
  if (round($_REQUEST['amount'],2) > round($cbalance['total'],2)) 	{ return false; }
  if ($_REQUEST['amount'] <= 0) 					{ return false; }
  return true;
  }

if ($form->validate()) {
  // add payment and redirect to legal profile
  
  $form->freeze();
  $values = $form->_submitValues;
  
  // check if write off
//  if (count($loan->data['legal']['ini_balances']['write_off_ini_balance']))  {
  if ( $loan->data['legal']['writeOff'] )  {
  
    WEBPAGE::$dbh->query(sprintf("insert into tblLoanWriteOffPayments (id,loan_id,amount,user_id,date) values ('','%s','%s','%s','%s')",$values['id'],$values['amount'],WEBPAGE::$userID,date('Y-m-d')));

    } else {

    $diff = $values['amount'];
    $pmt_dist['principal'] = ( $diff - $cbalance['principal'] >= 0 ) ? $cbalance['principal'] : max(0,$diff);
    $diff = $diff - $cbalance['principal'];
    $pmt_dist['insurance'] = ( $diff - $cbalance['insurance'] >= 0 ) ? $cbalance['insurance'] : max(0,$diff);
    $diff = $diff - $cbalance['insurance'];
    $pmt_dist['fees'] = ( $diff - $cbalance['fees'] >= 0 ) ? $cbalance['fees'] : max(0,$diff);
    $diff = $diff - $cbalance['fees'];
    $pmt_dist['interest'] = ( $diff - $cbalance['interest'] >= 0 ) ? $cbalance['interest'] : max(0,$diff);
    $diff = $diff - $cbalance['interest'];
    $pmt_dist['penalties'] = ( $diff - $cbalance['penalties'] >= 0 ) ? $cbalance['penalties'] : max(0,$diff);

    WEBPAGE::$dbh->query(sprintf("insert into tblLoansParkedPayments (id,loan_id,principal,insurance,fees,interest,penalties,total,user_id,date) values ('','%s','%s','%s','%s','%s','%s','%s','%s','%s')",$values['id'],$pmt_dist['principal'],$pmt_dist['insurance'],$pmt_dist['fees'],$pmt_dist['interest'],$pmt_dist['penalties'],$values['amount'],WEBPAGE::$userID,date('Y-m-d')));

    }

  if ( round($values['amount'],2) == round($cbalance['total'],2) ) { $loan->legalOut(); }
  WEBPAGE::redirect(sprintf("index.popup.php?scr_name=LG.SCR.legalProfile&id=%s&msg=scs&ico=scs",$values['id']));

  }

$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
$tpl->setVariable('applyPayment',			          $_LABELS['applyPayment']);
$tpl->setVariable('max_payment',                $loan->data['legal']['current_balance']['total']);
$tpl->setVariable('max_payment_label',          sprintf('%s: ',$_LABELS['total']));

?>