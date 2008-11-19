<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_type.php';

extract(LOAN_TYPE::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('description',				$description);
$tpl->setVariable('borrower_type_label',		$_LABELS['tblLoanTypes.borrower_type']);
$tpl->setVariable('borrower_type',				$_LABELS[sprintf('tblLoanTypes.borrower_type.%s',$borrower_type)]);
$tpl->setVariable('calendar_label',				$_LABELS['tblLoanTypes.calendar']);
$tpl->setVariable('calendar',					$_LABELS[sprintf('tblLoanTypes.calendar.%s',$calendar)]);
$tpl->setVariable('currency_label',				$_LABELS['tblFunds.currency_id']);
$tpl->setVariable('currency',					$currency);
$tpl->setVariable('installment_label',			$_LABELS['tblLoanTypes.installment']);
$tpl->setVariable('installment',				$installment);
$tpl->setVariable('payment_frequency_label',	$_LABELS['tblLoanTypes.payment_frequency']);
$tpl->setVariable('payment_frequency',			$_LABELS[sprintf('tblLoanTypes.payment_frequency.%s',$payment_frequency)]);
$tpl->setVariable('margin_c_label',				$_LABELS['tblLoanTypes.margin_c']);
$tpl->setVariable('margin_c',					$margin_c);
$tpl->setVariable('margin_d_label',				$_LABELS['tblLoanTypes.margin_d']);
$tpl->setVariable('margin_d',					$margin_d);
$tpl->setVariable('savings_p_label',			$_LABELS['tblLoanTypes.savings_p']);
$tpl->setVariable('savings_p',					$savings_p);
$tpl->setVariable('fees_at_label',				$_LABELS['tblLoanTypes.fees_at']);
$tpl->setVariable('fees_at',					$fees_at);
$tpl->setVariable('fees_af_label',				$_LABELS['tblLoanTypes.fees_af']);
$tpl->setVariable('fees_af',					$fees_af);
$tpl->setVariable('rates_r_label',				$_LABELS['tblLoanTypes.rates_r']);
$tpl->setVariable('rates_r',					$rates_r);
$tpl->setVariable('rates_e_label',				$_LABELS['rate_e']);
$tpl->setVariable('rates_e',					$rates_e);
$tpl->setVariable('rates_d_label',				$_LABELS['tblLoanTypes.rates_d']);
$tpl->setVariable('rates_d',					$rates_d);
$tpl->setVariable('creator',					sprintf($_LABELS['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor',						sprintf($_LABELS['editedByOn'],$editor,$f_editor_date));
$tpl->setVariable('ltype_img',					$img_path);

include './includes/ST.SCR.buttonMan.inc';

?>