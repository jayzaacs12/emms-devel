<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';

$master 	= new LOAN_MASTER($_REQUEST['id']);
$lmc = $master->get_duplicates();
$lmc[$_REQUEST['id']] = $_REQUEST['id']; 

$c = 1;
foreach($lmc as $key => $val) {
    
    $master = new LOAN_MASTER($val);
    $loan  = new LOAN(0,$master->data['loan_type_id']);
    $master->loadloans();
    $master->loadBorrowerData();

    $duplicates[$val]['hit']            = $c;
    $duplicates[$val]['borrower']       = $master->data['borrower']->data['name'];
    $duplicates[$val]['amount']         = $master->data['amount'];
    $duplicates[$val]['modality']       = $loan->data['modality'];
    $duplicates[$val]['check_status']   = $_LABELS[sprintf('tblLoansMaster.check_status.%s',$master->data['check_status'])];
    $duplicates[$val]['check_number']   = $master->data['check_number'];
    $duplicates[$val]['view']           = sprintf("<a href='index.popup.php?scr_name=LN.SCR.viewLoanMasterPrintable&id=%s'>%s</a>",$val,$_LABELS['view']);
    $c++;
}

$head = array('#',$_LABELS['tblLoansMaster.borrower_id'],$_LABELS['tblLoansMaster.amount'],$_LABELS['tblLoansMaster.loan_type_id'],$_LABELS['tblLoansMaster.check_status'],$_LABELS['tblLoansMaster.check_number'],$_LABELS['view']);

$tpl->setVariable('page_title',	$_LABELS['LN.SCR.browseLoanMasterDuplicates']);
$tpl->setVariable('chart', WEBPAGE::printchart($duplicates,$head));


?>
