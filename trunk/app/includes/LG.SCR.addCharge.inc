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
//$cbalance = $loan->data['legal']['current_balance'];

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

$form = new HTML_QuickForm('lchargeForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('lchargeForm',$_LABELS['submit']));
$form->addElement('hidden',	  'scr_name',		  $_REQUEST['scr_name']);
$form->addElement('hidden',	  'id',			      $_REQUEST['id']);
$form->addElement('text',		  'amount',			  $_LABELS['charge_amount'],   'class=large');
$form->addElement('textarea', 'description',  $_LABELS['description'],     'class=large');

$form->addRule('amount',	    $_LABELS['required'], 	'required');
$form->addRule('amount',	    $_LABELS['numeric'],    'numeric');
$form->addRule('description',	$_LABELS['required'], 	'required');

if ($form->validate()) {
  // do charge and redirect to legal profile

  $form->freeze();
  $values = $form->_submitValues;

  WEBPAGE::$dbh->query(sprintf("insert into tblLoanWriteOffCharges (id,loan_id,amount,description,user_id,date) values ('','%s','%s','%s','%s','%s')",$values['id'],$values['amount'],$values['description'],WEBPAGE::$userID,date('Y-m-d')));
  WEBPAGE::redirect(sprintf("index.popup.php?scr_name=LG.SCR.legalProfile&id=%s&msg=scs&ico=scs",$values['id']));

  }

$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
$tpl->setVariable('applyLCharge',			          $_LABELS['applyLCharge']);

?>