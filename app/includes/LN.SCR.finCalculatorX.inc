<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/loan_master.php';

$btn  		= new TTFButton(WEBPAGE::$theme);
$loan 		= new LOAN(0,$_REQUEST['id']);

$ltype 		= new LOAN_TYPE($_REQUEST['id']);

$tpl->setVariable('title', $_LABELS['LN.SCR.finCalculator']);
$tpl->setVariable('loanType', 				$ltype->data['description']);
$tpl->setVariable('loanType_label', 		$_LABELS['tblLoanTypes.description']);
$tpl->setVariable('rates_label', 			$_LABELS['rate']);
$tpl->setVariable('margin_label', 			$_LABELS['margin']);
$tpl->setVariable('days', 					$_LABELS['days']);

$form = new HTML_QuickForm('iLoanForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('iLoanForm',$_LABELS['submit']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);

$form->addElement('static', 	'rate', 	'', $_LABELS['rate']);
$form->addElement('static', 	'insurance','', $_LABELS['insurance']);

$form->addElement('select',		'payment_frequency',	$_LABELS['tblLoanTypes.payment_frequency'],	$ltype->payment_frequencys(), 	"onChange='calEffRate()'");
$form->addElement('select',		'calendar_type',		$_LABELS['tblLoanTypes.calendar_type'],		$ltype->calendar_types(), 		"onChange='calEffRate()'");

$form->addElement('text',		'rdate',                $_LABELS['tblLoans.delivered_date']);
$form->addElement('text',		'kp',					$_LABELS['tblLoans.kp']);
$form->addElement('text',		'rates_r',				$_LABELS['tblLoanTypes.rates_r'],				"onKeyUp='calEffRate()'");
$form->addElement('text',		'rates_e',				$_LABELS['rate_e'], 							"onFocus='document.iLoanForm.rates_e.blur()' style='color:gray'");
$form->addElement('text',		'fees_at',				$_LABELS['tblLoanTypes.fees_at'],				"onKeyUp='calEffRate()'");
$form->addElement('text',		'fees_af',				$_LABELS['tblLoanTypes.fees_af'],				"onKeyUp='calEffRate()'");
$form->addElement('text',		'installment',			$_LABELS['tblLoanTypes.installment'],			"onKeyUp='calEffRate()'");

//add rules
$form->addRule('rdate',             'yyyy-mm-dd','required');
$form->addRule('rdate',             'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');

$form->addRule('kp',				$_LABELS['kp_err'],		'required');
$form->addRule('kp',				$_LABELS['kp_err'],		'numeric');
$form->addRule('installment',		$_LABELS['numeric'],	'required');
$form->addRule('installment',		$_LABELS['numeric'],	'regex', '(^[0-9 ]*$)');
$form->addRule('rates_r',			$_LABELS['numeric'],	'required');
$form->addRule('rates_r',			$_LABELS['numeric'],	'numeric');
$form->addRule('fees_at',			$_LABELS['numeric'],	'required');
$form->addRule('fees_at',			$_LABELS['numeric'],	'numeric');
$form->addRule('fees_af',			$_LABELS['numeric'],	'required');
$form->addRule('fees_af',			$_LABELS['numeric'],	'numeric');

$form->freeze($loan->frostdata);

if ($form->validate()) {

  //$values = $form->_submitValues;

  //$values['rates_e'] 		= round(CalEffRate($values['rates_r'],$values['fees_at'],$values['fees_af'],$values['installment'],$values['calendar_type'],$values['payment_frequency']),2);
  //$values['kat'] 			= $values['kp'] * $values['fees_at'] / 100;
  //$values['kaf'] 			= $values['kp'] * $values['fees_af'] / 100;
  //$values['pmt'] 			= ceil(CalMinPMT($values['kp'],$values['kat'],$values['kaf'],$values['rates_r'],$values['installment'],$values['calendar_type'],$values['payment_frequency']));

  $values = $form->_submitValues;
  $file = sprintf('index.popup.php?scr_name=LN.SCR.finCalculatorXPlan&id=%s&rdate=%s&kp=%s&rates_r=%s&fees_at=%s&fees_af=%s&installment=%s&calendar_type=%s&payment_frequency=%s',$values['id'],$values['rdate'],$values['kp'],$values['rates_r'],$values['fees_at'],$values['fees_af'],$values['installment'],$values['calendar_type'],$values['payment_frequency']);
  $tpl->setVariable('showPlan', sprintf('showPlan("%s")',$file));

  }

$form->setDefaults($loan->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>
