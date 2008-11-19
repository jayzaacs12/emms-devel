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

$loan  = new LOAN($_REQUEST['id']);
$loan->load_legal_data();

extract(LOAN::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('title',					           $_LABELS[WEBPAGE::$scr_name]);

$tpl->setVariable('general_info',					     $_LABELS['general_info']);

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
$tpl->setVariable('currency',					         $currency);
$tpl->setVariable('currency_label',				     $_LABELS['tblFunds.currency_id']);
$tpl->setVariable('modality',					         $modality);
$tpl->setVariable('modality_label',				     $_LABELS['tblLoanTypes.description']);
$tpl->setVariable('kp',							           $kp);
$tpl->setVariable('kp_label',					         $_LABELS['tblLoans.kp']);
$tpl->setVariable('currency_symbol',			     $currency_symbol);

// historial de saldos
$tpl->setVariable('transactions_resume',			 $_LABELS['transactions_resume']);
$tpl->setVariable('transactions_resume_chart', WEBPAGE::printchart_ii($loan->data['legal']['transactions_resume'],array('total'=>$_LABELS['total'],'principal'=>$_LABELS['KP'],'insurance'=>$_LABELS['INS'],'fees'=>$_LABELS['FEES'],'interest'=>$_LABELS['RT'],'penalties'=>$_LABELS['PN'],'others'=>$_LABELS['others'],'type'=>$_LABELS['type'],'cycle'=>$_LABELS['cycle']),array('totals'=>true,'cols'=>array('total','principal','insurance','fees','interest','penalties','others')),array(),array('legend'=>true)));

//  historial de pagos
$tpl->setVariable('payments',			             $_LABELS['payment_history']);
$tpl->setVariable('payments_chart',            WEBPAGE::printchart_ii($loan->data['legal']['payments'],array('date'=>$_LABELS['date'],'total'=>$_LABELS['tblPayments.pmt'],'principal'=>$_LABELS['KP'],'insurance'=>$_LABELS['INS'],'fees'=>$_LABELS['FEES'],'interest'=>$_LABELS['RT'],'penalties'=>$_LABELS['PN'],'others'=>$_LABELS['others'],'username'=>$_LABELS['tblUsers.username']),array('totals'=>true,'subtotals'=>true,'cols'=>array('total','principal','insurance','fees','interest','penalties','others')),array(),array('legend'=>true)));

// balance at the beginning of each cycle
$tpl->setVariable('ini_balances',			         $_LABELS['ini_balances']);
$tpl->setVariable('ini_balances_chart',        WEBPAGE::printchart_ii($loan->data['legal']['ini_balances'],array('total'=>$_LABELS['total'],'principal'=>$_LABELS['KP'],'insurance'=>$_LABELS['INS'],'fees'=>$_LABELS['FEES'],'interest'=>$_LABELS['RT'],'penalties'=>$_LABELS['PN'],'date'=>$_LABELS['date'],'cycle'=>$_LABELS['cycle']),array(),array(),array('legend'=>true)));

// historial de cargos legales
$tpl->setVariable('write_off_charges',			   $_LABELS['legal_charges']);
$tpl->setVariable('write_off_charges_chart', 	 WEBPAGE::printchart_ii($loan->data['legal']['dcharges'],array('date'=>$_LABELS['date'],'amount'=>$_LABELS['amount'],'description'=>$_LABELS['description'],'username'=>$_LABELS['tblUsers.username']),array('totals'=>true,'subtotals'=>false,'cols'=>array('amount')),array(),array('legend'=>true)));

// show applyPayment, write off and applyCharge buttons accordingly
if ($loan->data['status'] == 'LI') {
  if (intval(WEBPAGE::$userAccessCode) & intval($_CONF['LG.SCR.addPayment']) ) { $tpl->setVariable('applyPayment', $btn->imglink(sprintf('%s?scr_name=LG.SCR.addPayment&id=%s',$_PHP_SELF,$loan->data['id']), $_LABELS['applyPayment']));  }
  if ($loan->data['legal']['writeOff']) {
    if (intval(WEBPAGE::$userAccessCode) & intval($_CONF['LG.SCR.addCharge']) ) { $tpl->setVariable('applyCharge', $btn->imglink(sprintf('%s?scr_name=LG.SCR.addCharge&id=%s',$_PHP_SELF,$loan->data['id']), $_LABELS['applyLCharge']));  }
    } else {
    if (intval(WEBPAGE::$userAccessCode) & intval($_CONF['LG.SCR.doWriteOff']) ) { $tpl->setVariable('doWriteOff', $btn->imglink(sprintf('%s?scr_name=LG.SCR.doWriteOff&id=%s',$_PHP_SELF,$loan->data['id']), $_LABELS['writeOff']));  }
    }
  }

$tpl->setVariable('printerlink',			         WEBPAGE::printerlink());

?>