<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_type.php';

$ltype = new LOAN_TYPE($_REQUEST['id']);
$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('ltypeForm','POST');

$form->addElement('static', 	      'submit', 	          '', $btn->imgsubmit('ltypeForm',$_LABELS['submit']));
$form->addElement('static', 	      'reset', 	            '', $btn->imgreset('ltypeForm',$_LABELS['reset']));
$form->addElement('static', 	      'cancel', 	          '', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden',		      'scr_name',		        $_REQUEST['scr_name']);
$form->addElement('hidden',		      'id',			            $_REQUEST['id']);
$form->addElement('hidden',		      'editor_id',			    WEBPAGE::$userID);
$form->addElement('hidden',		      'editor_date',			  date('Y-m-d'));

$form->addElement('static', 	      'rate', 	            '', $_LABELS['rate']);
$form->addElement('static', 	      'margin', 	          '', $_LABELS['margin']);
$form->addElement('static', 	      'insurance',          '', $_LABELS['insurance']);

$form->addElement('select',		      'currency_id',			  $_LABELS['tblFunds.currency_id'],			       (array('-'=>'') + $ltype->currencys()));
$form->addElement('select',		      'payment_frequency',	$_LABELS['tblLoanTypes.payment_frequency'],	 (array('-'=>'') + $ltype->payment_frequencys()), 	"onChange='calEffRate()'");
$form->addElement('select',		      'borrower_type',		  $_LABELS['tblLoanTypes.borrower_type'],		   (array('-'=>'') + $ltype->borrower_types()));
$form->addElement('select',		      'calendar_type',		  $_LABELS['tblLoanTypes.calendar_type'],		   (array('-'=>'') + $ltype->calendar_types()), 		"onChange='calEffRate()'");
$form->addElement('text',		        'rates_r',				    $_LABELS['tblLoanTypes.rates_r'],		         "onKeyUp='calEffRate()'");
$form->addElement('advcheckbox',    'rates_r_lock',		    '',		                                       $_LABELS['lock'],'class=checkbox');
$form->addElement('text',		        'rates_d',				    $_LABELS['tblLoanTypes.rates_d']);
$form->addElement('advcheckbox',    'rates_d_lock',		    '',		                                       $_LABELS['lock'],'class=checkbox');
$form->addElement('text',		        'rates_e',				    $_LABELS['rate_e'], 'disabled');
$form->addElement('text',		        'fees_at',				    $_LABELS['tblLoanTypes.fees_at'],		         "onKeyUp='calEffRate()'");
$form->addElement('advcheckbox',    'fees_at_lock',		    '',		                                       $_LABELS['lock'],'class=checkbox');
$form->addElement('text',		        'fees_af',				    $_LABELS['tblLoanTypes.fees_af'],		         "onKeyUp='calEffRate()'");
$form->addElement('advcheckbox',    'fees_af_lock',		    '',		                                       $_LABELS['lock'],'class=checkbox');
$form->addElement('text',		        'installment',			  $_LABELS['tblLoanTypes.installment'],	       "onKeyUp='calEffRate()'");
$form->addElement('advcheckbox',    'installment_lock',	  '',		                                       $_LABELS['lock'],'class=checkbox');
$form->addElement('text',		        'savings_p',			    $_LABELS['tblLoanTypes.savings_p']);
$form->addElement('advcheckbox',    'savings_p_lock',		  '',		                                       $_LABELS['lock'],'class=checkbox');
$form->addElement('text',		        'margin_d',				    $_LABELS['tblLoanTypes.margin_d']);
$form->addElement('advcheckbox',    'margin_d_lock',		  '',		                                       $_LABELS['lock'],'class=checkbox');
$form->addElement('text',		        'margin_c',				    $_LABELS['tblLoanTypes.margin_c']);
$form->addElement('advcheckbox',    'margin_c_lock',		  '',		                                       $_LABELS['lock'],'class=checkbox');
$form->addElement('text',		        'description',			  $_LABELS['tblLoanTypes.description']);

//add rules
$form->addRule('currency_id',		    $_LABELS['selectOption'],'required');
$form->addRule('currency_id',		    $_LABELS['selectOption'],'alphanumeric');
$form->addRule('payment_frequency',	$_LABELS['selectOption'],'required');
$form->addRule('payment_frequency',	$_LABELS['selectOption'],'alphanumeric');
$form->addRule('borrower_type',		  $_LABELS['selectOption'],'required');
$form->addRule('borrower_type',		  $_LABELS['selectOption'],'alphanumeric');
$form->addRule('calendar_type',		  $_LABELS['selectOption'],'required');
$form->addRule('calendar_type',		  $_LABELS['selectOption'],'alphanumeric');

$form->addRule('description',		    sprintf($_LABELS['rangelength'],1,64),'required');
$form->addRule('description',		    sprintf($_LABELS['rangelength'],1,64),'rangelength', array(1,64));
$form->addRule('description',		    $_LABELS['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');

$form->addRule('installment',	      $_LABELS['numeric'],	'required');
$form->addRule('installment',	      $_LABELS['numeric'],	'regex', '(^[0-9 ]*$)');
$form->addRule('rates_r',		        $_LABELS['numeric'],	'required');
$form->addRule('rates_r',		        $_LABELS['numeric'],	'numeric');
$form->addRule('rates_d',		        $_LABELS['numeric'],	'required');
$form->addRule('rates_d',		        $_LABELS['numeric'],	'numeric');
$form->addRule('fees_at',		        $_LABELS['numeric'],	'required');
$form->addRule('fees_at',		        $_LABELS['numeric'],	'numeric');
$form->addRule('fees_af',		        $_LABELS['numeric'],	'required');
$form->addRule('fees_af',		        $_LABELS['numeric'],	'numeric');
$form->addRule('savings_p',		      $_LABELS['numeric'],	'required');
$form->addRule('savings_p',		      $_LABELS['numeric'],	'numeric');
$form->addRule('margin_d',		      $_LABELS['numeric'],	'required');
$form->addRule('margin_d',		      $_LABELS['numeric'],	'regex', '(^[0-9 ]*$)');
$form->addRule('margin_c',		      $_LABELS['numeric'],	'required');
$form->addRule('margin_c',		      $_LABELS['numeric'],	'numeric');

$form->freeze($ltype->frostdata);

if ($form->validate()) { 

  $form->freeze();
  $values = $form->_submitValues;
  foreach ($values as $key=>$val) { $ltype->data[$key] = $val; }
  $ltype->data['id'] = $ltype->data['id'] ? $ltype->update() : $ltype->add();
  WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.viewLoanType&id=%s&ico=scs&msg=scs',$ltype->data['id']));

  }

$form->setDefaults($ltype->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>