<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/client.php';
require_once 'class/roles.php';
require_once 'class/society.php';
require_once 'class/business_type.php';
require_once 'class/business.php';
require_once 'class/zone.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/loan_master.php';

$btn  		= new TTFButton(WEBPAGE::$theme);
$loan 		= new LOAN($_REQUEST['id'],$_REQUEST['loan_type_id']); 

switch ($loan->data['status']) {
  case 'N' : break;
  case 'O' : break;
  case 'S' : break;
  case 'R' : break;
  case 'RO': break;
  default: 	
    $loan->frostdata = array_merge($loan->frostdata,array('kp','pg_value','re_value','fgd_value','fgt_value')); 
//    WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.viewILoan&id=%s&ico=err&msg=loan.edit.err',$loan->data['id']));
  }

$clientID	= $loan->data['client_id'] 		? $loan->data['client_id'] 		: $_REQUEST['client_id'];
$lTypeID	= $loan->data['loan_type_id'] 	? $loan->data['loan_type_id'] 	: $_REQUEST['loan_type_id'];
$businessID	= $loan->data['business_id'] 	? $loan->data['business_id'] 	: $_REQUEST['business_id'];

$loan->load_borrower($clientID);

$ltype 		= new LOAN_TYPE($lTypeID);

if ($businessID) {
  $business	= new BUSINESS($businessID);
  $tpl->setVariable('business_label', $_LABELS['tblBusiness.name']);
  $tpl->setVariable('business', sprintf("%s<br><br>",$business->data['name']));
  }

$tpl->setVariable('currency', 				$ltype->data['symbol']);
$tpl->setVariable('client', 				$loan->data['borrower_name']);
$tpl->setVariable('client_code', 			$loan->data['borrower_code']);
$tpl->setVariable('client_label', 			$_LABELS['tblClients.id']);
$tpl->setVariable('loanType', 				$ltype->data['description']);
$tpl->setVariable('loanType_label', 		$_LABELS['tblLoanTypes.description']);
$tpl->setVariable('rates_label', 			$_LABELS['rate']);
$tpl->setVariable('margin_label', 			$_LABELS['margin']);
$tpl->setVariable('days', 					$_LABELS['days']);
$tpl->setVariable('collateralType_label', 	$_LABELS['tblCollateral.id']);
$tpl->setVariable('img', 					$loan->data['borrower_img_path']);

$form = new HTML_QuickForm('iLoanForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('iLoanForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('iLoanForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'ref',			$_REQUEST['ref']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);
$form->addElement('hidden',		'business_id',	$_REQUEST['business_id']);
$form->addElement('hidden',		'loan_type_id',	$_REQUEST['loan_type_id']);
$form->addElement('hidden',		'client_id',	$_REQUEST['client_id']);
$form->addElement('hidden',		'margin_c',		$ltype->data['margin_c']);
$form->addElement('hidden',		'lt_savings_p',	$ltype->data['savings_p']);

$form->addElement('static', 	'rate', 	'', $_LABELS['rate']);
$form->addElement('static', 	'margin', 	'', $_LABELS['margin']);
$form->addElement('static', 	'insurance','', $_LABELS['insurance']);

$form->addElement('select',		'payment_frequency',	$_LABELS['tblLoanTypes.payment_frequency'],	(array('-'=>'') + $ltype->payment_frequencys()), 	"onChange='calEffRate()'");
$form->addElement('select',		'calendar_type',		$_LABELS['tblLoanTypes.calendar_type'],		(array('-'=>'') + $ltype->calendar_types()), 		"onChange='calEffRate()'");

$form->addElement('text',		'kp',					$_LABELS['tblLoans.kp'],  						sprintf("onKeyUp='document.iLoanForm.collateral_min_value.value=(document.iLoanForm.kp.value)*(1+0.01*document.iLoanForm.margin_c.value);document.iLoanForm.savings_p.value=((document.iLoanForm.kp.value)*(0.01*document.iLoanForm.lt_savings_p.value))/%s;'",$loan->CalNumOfPMT()));
//$form->addElement('text',		'collateral_min_value',	$_LABELS['tblLoanTypes.margin_c'], 				"onFocus='document.iLoanForm.collateral_min_value.blur()' style='color:gray'");
$form->addElement('text',		'collateral_min_value',	$_LABELS['tblLoanTypes.margin_c']);
$form->addElement('text',		'rates_r',				$_LABELS['tblLoanTypes.rates_r'],				"onKeyUp='calEffRate()'");
$form->addElement('text',		'rates_d',				$_LABELS['tblLoanTypes.rates_d']);
$form->addElement('text',		'rates_e',				$_LABELS['rate_e'], 							"onFocus='document.iLoanForm.rates_e.blur()' style='color:gray'");
//$form->addElement('text',		'savings_p',			$_LABELS['tblLoans.savings_p'], 				"onFocus='document.iLoanForm.savings_p.blur()' style='color:gray'");
$form->addElement('text',		'savings_p',			$_LABELS['tblLoans.savings_p']);
$form->addElement('text',		'savings_v',			$_LABELS['tblLoans.savings_v']);
$form->addElement('text',		'fees_at',				$_LABELS['tblLoanTypes.fees_at'],				"onKeyUp='calEffRate()'");
$form->addElement('text',		'fees_af',				$_LABELS['tblLoanTypes.fees_af'],				"onKeyUp='calEffRate()'");
$form->addElement('text',		'installment',			$_LABELS['tblLoanTypes.installment'],			"onKeyUp='calEffRate()'");
$form->addElement('text',		'margin_d',				$_LABELS['tblLoanTypes.margin_d']);
$form->addElement('text',		'pg_value',				$_LABELS['tblCollateral.pawned_goods'],			"onKeyUp='calSubTotal()'");
$form->addElement('text',		're_value',				$_LABELS['tblCollateral.real_estate'],			"onKeyUp='calSubTotal()'");
$form->addElement('text',		'fgd_value',			$_LABELS['tblCollateral.fiduciary_goods'],		"onKeyUp='calSubTotal()'");
$form->addElement('text',		'fgt_value',			$_LABELS['tblCollateral.fiduciary_garantees'],	"onKeyUp='calSubTotal()'");
$form->addElement('text',		'collateral_value',		$_LABELS['total'], 								"onFocus='document.iLoanForm.collateral_value.blur()' style='color:gray'");

//add rules
$form->addRule('payment_frequency',	$_LABELS['selectOption'],'required');
$form->addRule('payment_frequency',	$_LABELS['selectOption'],'alphanumeric');
$form->addRule('calendar_type',		$_LABELS['selectOption'],'required');
$form->addRule('calendar_type',		$_LABELS['selectOption'],'alphanumeric');

$form->addRule('kp',				$_LABELS['kp_err'],		'required');
$form->addRule('kp',				$_LABELS['kp_err'],		'numeric');
$form->addRule('savings_v',			$_LABELS['numeric'],	'required');
$form->addRule('savings_v',			$_LABELS['numeric'],	'numeric');
$form->addRule('installment',		$_LABELS['numeric'],	'required');
$form->addRule('installment',		$_LABELS['numeric'],	'regex', '(^[0-9 ]*$)');
$form->addRule('rates_r',			$_LABELS['numeric'],	'required');
$form->addRule('rates_r',			$_LABELS['numeric'],	'numeric');
$form->addRule('rates_d',			$_LABELS['numeric'],	'required');
$form->addRule('rates_d',			$_LABELS['numeric'],	'numeric');
$form->addRule('fees_at',			$_LABELS['numeric'],	'required');
$form->addRule('fees_at',			$_LABELS['numeric'],	'numeric');
$form->addRule('fees_af',			$_LABELS['numeric'],	'required');
$form->addRule('fees_af',			$_LABELS['numeric'],	'numeric');
$form->addRule('margin_r',			$_LABELS['numeric'],	'required');
$form->addRule('margin_r',			$_LABELS['numeric'],	'regex', '(^[0-9 ]*$)');
$form->addRule('margin_k',			$_LABELS['numeric'],	'required');
$form->addRule('margin_k',			$_LABELS['numeric'],	'regex', '(^[0-9 ]*$)');
$form->addRule('margin_d',			$_LABELS['numeric'],	'required');
$form->addRule('margin_d',			$_LABELS['numeric'],	'regex', '(^[0-9 ]*$)');
$form->addRule('margin_c',			$_LABELS['numeric'],	'required');
$form->addRule('margin_c',			$_LABELS['numeric'],	'numeric');
$form->addRule('margin_c',			$_LABELS['numeric'],	'numeric');

$form->addRule('collateral_value',	sprintf("%s ^",$_LABELS['required']), 'required');
$form->addRule('collateral_value',	sprintf($_LABELS['lowCollateral'],$_REQUEST['collateral_min_value']), 'callback', 'checkcollateral');
function checkcollateral() {
  if ($_REQUEST['collateral_value'] < $_REQUEST['collateral_min_value']) { 
    return false; 
    } else {
    return true;
    }
  }


$form->freeze($loan->frostdata);

if ($form->validate()) { 
  
  $form->removeElement('submit');
  $form->removeElement('reset');
  $form->removeElement('cancel');

  $form->freeze();
  
  $values = $form->_submitValues;
  
  $values['savings_p']		= ceil($values['savings_p']);
  
  $values['editor_date'] 	= date('Y-m-d');
  $values['creator_date'] 	= $loan->data['creator_date'] ? $loan->data['creator_date'] : $values['editor_date'];
  $values['editor_id']   	= WEBPAGE::$userID;
  $values['creator_id']  	= $loan->data['creator_id']   ? $loan->data['creator_id']   : $values['editor_id'];

  $values['status']			= $loan->data['status'];
  $values['rates_e'] 		= round(CalEffRate($values['rates_r'],$values['fees_at'],$values['fees_af'],$values['installment'],$values['calendar_type'],$values['payment_frequency']),2);
  $values['kat'] 			= $values['kp'] * $values['fees_at'] / 100;
  $values['kaf'] 			= $values['kp'] * $values['fees_af'] / 100;
  $values['pmt'] 			= ceil(CalMinPMT($values['kp'],$values['kat'],$values['kaf'],$values['rates_r'],$values['installment'],$values['calendar_type'],$values['payment_frequency']));
  $values['client_zone_id']	= $client->data['zone_id'];
//  $values['zone_id']		= WEBPAGE::$userZone;
  
  $zone = new ZONE($client->data['zone_id']);

  $values['loan_code']		= $values['loan_code']  ? $values['loan_code']  : (date(ym).'.'.str_pad(WEBPAGE::$userZone, 3, '0', STR_PAD_LEFT).'.'.str_pad($values['loan_type_id'], 3, '0', STR_PAD_LEFT).'.'.str_pad($values['client_id'], 6, '0', STR_PAD_LEFT));
  $values['program_id']		= $values['program_id'] ? $values['program_id'] : $zone->zonedata['program_id'];

  $lastID = SQL::insert('tblLoans',$values,'id');
  
  LOAN_MASTER::updateAmount($loan->data['master_id']);
  WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.viewILoan&id=%s',$lastID));
  
  } 

$form->setDefaults($loan->data);
//if ($loan->isFrost()) { $form->freeze(); }
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>