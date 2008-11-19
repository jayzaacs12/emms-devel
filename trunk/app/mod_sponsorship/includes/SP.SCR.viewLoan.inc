<?php
require_once '../includes/LN.LIB.functions.inc';
require_once 'Date.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/roles.php';
require_once 'class/client.php';
require_once 'class/society.php';
require_once 'class/business_type.php';
require_once 'class/business.php';

// //echo(getcwd());
// echo $_SERVER['DOCUMENT_ROOT'];
// //include_once 'includes/trace.debugger.php';
// 
// //trace("\n"."doing maintenance 2".getcwd());
// //echo 'doing maintenance';
// //trace('estoy aqui');
// //trace('doing maintenance 2');
// require_once '../includes/LN.LIB.functions.inc';
// require_once 'Date.php';
// echo $_SERVER['DOCUMENT_ROOT'];
// //require_once 'class/loan_type.sp.php';
// require_once '/app/class/loan.php';
// require_once '/app/class/roles.php';
// 
// require_once '/app/class/client.php';
// require_once '/app/class/society.php';
// require_once '/app/class/business_type.php';
// require_once '/app/class/business.php';


$loan  = new LOAN($_REQUEST['id']);

extract(LOAN::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

if (count($status_controls)) {
  $c = 0;
  $separator = '';
  $tpl->setVariable('status_bar_label',			$_LABELS['statusSelector']);
  $tpl->addBlockfile('status_bar','status_bar','ST.navbar.tpl');
  foreach ($status_controls as $key => $val) {
    $tpl->setCurrentBlock("st_navbar");
    $tpl->setVariable('width',					sprintf("%spx",(30*$c)));
    $tpl->setVariable('separator',				$separator);
    $tpl->setVariable('link',						$val['link']);
    $tpl->setVariable('text',						$val['text']);
    $tpl->parseCurrentBlock("st_navbar");
    $c = 1;
    $separator = '|';
    }
  }

$tpl->setVariable('loan_label',			      $_LABELS['tblLoans.id']);
$tpl->setVariable('loan_code',					  $loan_code);
$tpl->setVariable('loan_code_label',			$_LABELS['tblLoans.loan_code']);
$tpl->setVariable('status',						    $_LABELS[sprintf("tblLoans.status.%s",$status)]);
$tpl->setVariable('status_label',				  $_LABELS['tblLoans.status']);
$tpl->setVariable('currency',					    $currency);
$tpl->setVariable('currency_label',				$_LABELS['tblFunds.currency_id']);

$client = new CLIENT($client_id);
$tpl->setVariable('borrower_label',				     $_LABELS['tblClients.id']);
//$tpl->setVariable('borrower_type',				$_LABELS[sprintf('tblLoanTypes.borrower_type.%s',$borrower_type)]);
$tpl->setVariable('borrower_name',				     $borrower_name);
$tpl->setVariable('borrower_name_label',	     $_LABELS['name']);
//$tpl->setVariable('href_borrower',				$borrower_link);
$tpl->setVariable('borrower_code',				     $borrower_code);
$tpl->setVariable('borrower_code_label',       $_LABELS['tblClients.code']);
$tpl->setVariable('borrower_birthdate',			   $client->data['birthdate']);
$tpl->setVariable('borrower_birthdate_label',  $_LABELS['tblClients.birthdate']);
$tpl->setVariable('borrower_education',			   $_LABELS[sprintf('tblClients.education.%s',$client->data['education'])]);
$tpl->setVariable('borrower_education_label',  $_LABELS['tblClients.education']);
$tpl->setVariable('borrower_cstatus',			     $_LABELS[sprintf('tblClients.cstatus.%s',$client->data['cstatus'])]);
$tpl->setVariable('borrower_cstatus_label',    $_LABELS['tblClients.cstatus']);
$tpl->setVariable('borrower_dependants',			 $client->data['dependants']);
$tpl->setVariable('borrower_dependants_label', $_LABELS['tblClients.dependants']);
$tpl->setVariable('borrower_group',				     $client->data['society'] ? $client->data['society'] : $_LABELS['R-I']);
$tpl->setVariable('borrower_group_label',      $_LABELS['tblClients.society_id']);
$tpl->setVariable('borrower_zone', 		         $client->data['zone']);
$tpl->setVariable('borrower_zone_label',       $_LABELS['tblClients.zone_id']);
$tpl->setVariable('borrower_memo',			       $client->data['memo'] ? $client->data['memo'] : $_LABELS['noData']);
$tpl->setVariable('borrower_memo_label',       $_LABELS['tblClients.memo']);


//if ($_CONF['business_check']) {
  if ($business_id) {
    $business	= new BUSINESS($business_id);
    $tpl->setVariable('business_label',             $_LABELS['tblBusiness.id']);
    $tpl->setVariable('business_activity', 		      $_LABELS[sprintf('tblBusinessTypes.activity.%s',$business->data['activity'])]);
    $tpl->setVariable('business_activity_label',    $_LABELS['tblBusinessTypes.activity']);
    $tpl->setVariable('business_type', 			        $business->data['type']);
    $tpl->setVariable('business_type_label',        $_LABELS['tblBusinessTypes.type']);
    $tpl->setVariable('business_name', 				      $business->data['name']);
    $tpl->setVariable('business_name_label',        $_LABELS['tblBusiness.name']);
    $tpl->setVariable('business_description', 		  $business->data['description'] ? $business->data['description'] : $_LABELS['noData']);
    $tpl->setVariable('business_description_label', $_LABELS['tblBusiness.description']);

//    $tpl->setVariable('href_business', 			        sprintf('index.php?scr_name=LN.SCR.pickBusiness&loan_id=%s&client_id=%s',$_REQUEST['id'],$client_id));
    }
//  }

$tpl->setVariable('survey', 					           $survey);
$tpl->setVariable('href_survey', 				         $href_survey);
$tpl->setVariable('survey_label', 		           $_LABELS['tblSurveys.id']);
$tpl->setVariable('modality',					           $modality);
$tpl->setVariable('modality_label',		           $_LABELS['tblLoanTypes.description']);
$tpl->setVariable('kp',							             $kp);
$tpl->setVariable('kp_label',					           $_LABELS['RP.GRP.Performance.kpDisbursment']);
$tpl->setVariable('collateralType_label', 		   $_LABELS['tblCollateral.id']);
$tpl->setVariable('currency_symbol',			       $currency_symbol);
$tpl->setVariable('collateral_min_value', 		   $collateral_min_value);
$tpl->setVariable('collateral_min_value_label',  $_LABELS['tblLoanTypes.margin_c']);
$tpl->setVariable('pg_value', 					         $pg_value);
$tpl->setVariable('pg_value_label', 			       $_LABELS['tblCollateral.pawned_goods']);
$tpl->setVariable('re_value', 					         $re_value);
$tpl->setVariable('re_value_label', 			       $_LABELS['tblCollateral.real_estate']);
$tpl->setVariable('fgd_value',	 				         $fgd_value);
$tpl->setVariable('fgd_value_label', 			       $_LABELS['tblCollateral.fiduciary_goods']);
$tpl->setVariable('fgt_value', 					         $fgt_value);
$tpl->setVariable('fgt_value_label', 			       $_LABELS['tblCollateral.fiduciary_garantees']);
$tpl->setVariable('collateral_value', 			     $collateral_value);
$tpl->setVariable('collateral_value_label', 	   $_LABELS['total']);
$tpl->setVariable('calendar_type',				       $_LABELS[sprintf('tblLoanTypes.calendar_type.%s',$calendar_type)]);
$tpl->setVariable('calendar_type_label',		     $_LABELS['tblLoanTypes.calendar_type']);
$tpl->setVariable('days', 						           $_LABELS['days']);
$tpl->setVariable('margin_label', 				       $_LABELS['margin']);
$tpl->setVariable('margin_k', 					         $margin_k);
$tpl->setVariable('margin_k_label', 			       $_LABELS['principal']);
$tpl->setVariable('margin_r', 					         $margin_r);
$tpl->setVariable('margin_r_label', 			       $_LABELS['interest']);
$tpl->setVariable('margin_d', 					         $margin_d);
$tpl->setVariable('margin_d_label', 			       $_LABELS['tblLoanTypes.margin_d']);
$tpl->setVariable('savings_p', 					         $savings_p);
$tpl->setVariable('savings_p_label', 			       $_LABELS['tblLoans.savings_p']);
$tpl->setVariable('savings_v', 					         $savings_v);
$tpl->setVariable('savings_v_label', 			       $_LABELS['tblLoans.savings_v']);
$tpl->setVariable('installment',				         $installment);
$tpl->setVariable('installment_label',			     $_LABELS['tblLoanTypes.installment']);
$tpl->setVariable('payment_frequency',			     $_LABELS[sprintf('tblLoanTypes.payment_frequency.%s',$payment_frequency)]);
$tpl->setVariable('payment_frequency_label',	   $_LABELS['tblLoanTypes.payment_frequency']);
$tpl->setVariable('rates_label', 				         $_LABELS['rate']);
$tpl->setVariable('fees_at_label',				       $_LABELS['tblLoanTypes.fees_at']);
$tpl->setVariable('fees_at',					           $fees_at);
$tpl->setVariable('fees_af_label',				       $_LABELS['tblLoanTypes.fees_af']);
$tpl->setVariable('fees_af',					           $fees_af);
$tpl->setVariable('rates_r_label',				       $_LABELS['tblLoanTypes.rates_r']);
$tpl->setVariable('rates_r',					           $rates_r);
$tpl->setVariable('rates_e_label',				       $_LABELS['rate_e']);
$tpl->setVariable('rates_e',					           $rates_e);
$tpl->setVariable('rates_d_label',				       $_LABELS['tblLoanTypes.rates_d']);
$tpl->setVariable('rates_d',					           $rates_d);
$tpl->setVariable('creator',					           sprintf($_LABELS['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor',						           sprintf($_LABELS['editedByOn'],$editor,$f_editor_date));
$tpl->setVariable('img',						             file_exists(sprintf('.%s',sprintf(WEBPAGE::_IMG_CLIENT,$borrower_code))) ? sprintf(WEBPAGE::_IMG_CLIENT,$borrower_code) : './img/unknown.png');
$tpl->setVariable('delivered_date',					 $delivered_date);
$tpl->setVariable('delivered_date_label',		 $_LABELS['tblLoans.delivered_date']);



//$tpl->setVariable('pmt_plan_label', $_LABELS['LN.SCR.pmtPlan']);
//$tpl->setVariable('pmt_plan_text', $_LABELS['view']);
//$tpl->setVariable('pmt_plan_href', sprintf('index.php?scr_name=SP.SCR.pmtPlan&ref=SP.SCR.disbursmentHistory&id=%s',$loan->data['id']));

$payments = current(WEBPAGE::$dbh->getAll(sprintf("select sum(pmt) pmt,sum(penalties) penalties,sum(interest) interest,sum(insurances) insurances,sum(fees) fees,sum(principal) principal, min(balance_kp) balance_kp from tblPayments where loan_id = %s",$loan->data['id'])));

$payments['balance_kp']     = $payments['balance_kp'] ? $payments['balance_kp'] : $kp;
$payments['balance_kp_usd'] = round($payments['balance_kp']/33,2);

$tpl->setVariable('pmt_label',            $_LABELS['PMT']);
$tpl->setVariable('pmt_fees',					    $payments['fees'] ? $payments['fees'] : '0.00');
$tpl->setVariable('pmt_fees_label',				$_LABELS['tblPayments.fees']);
$tpl->setVariable('pmt_insurances',		    $payments['insurances'] ? $payments['insurances'] : '0.00');
$tpl->setVariable('pmt_insurances_label', $_LABELS['tblPayments.insurances']);
$tpl->setVariable('pmt_principal',		    $payments['principal'] ? $payments['principal'] : '0.00');
$tpl->setVariable('pmt_principal_label',  $_LABELS['tblPayments.principal']);
$tpl->setVariable('pmt_interest',			    $payments['interest'] ? $payments['interest'] : '0.00');
$tpl->setVariable('pmt_interest_label',	  $_LABELS['tblPayments.interest']);
$tpl->setVariable('pmt_penalties',		    $payments['penalties'] ? $payments['penalties'] : '0.00');
$tpl->setVariable('pmt_penalties_label',  $_LABELS['tblPayments.penalties']);
$tpl->setVariable('pmt_total',				    $payments['pmt'] ? $payments['pmt'] : '0.00');
$tpl->setVariable('pmt_total_label',		  $_LABELS['total']);

$tpl->setVariable('recovery_kp_label',	  $_LABELS['RP.GRP.Performance.kpRecovery']);
$tpl->setVariable('recovery_kp',			    $payments['principal'] ? $payments['principal'] : '0.00');
$tpl->setVariable('balance_kp_label',		  $_LABELS['tblPayments.balance_kp']);
$tpl->setVariable('balance_kp',					  $payments['balance_kp']);
$tpl->setVariable('balance_kp_usd_label', $_LABELS['tblPayments.balance_kp']);
$tpl->setVariable('balance_kp_usd',			  $payments['balance_kp_usd'] ? $payments['balance_kp_usd'] : '0.00');

//echo '</pre>';
//     print_r($payments);
//echo '</pre>';
//$balance_kp	= $loan->data['balance_kp'];




//include '../includes/ST.SCR.buttonMan.inc';
 //   echo 'TEST6';exit;




?>