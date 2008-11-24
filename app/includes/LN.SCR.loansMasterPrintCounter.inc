<?php
/*
 * Increments tblLoansMaster.chk_process counter if the following requirements are met
 * 1. Check status is 'Approved'
 * 2. User requesting has access to LN.SCR.checkDisbursement
 */
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';

$master 	= new LOAN_MASTER($_REQUEST['id']);

//Planned update to index.popup.php will allow to skip the
//'intval(WEBPAGE::$userAccessCode) & intval($_CONF['LN.SCR.checkDisbursement'])' validation below
if ((!( intval(WEBPAGE::$userAccessCode) & intval($_CONF['LN.SCR.checkDisbursement']) ))||($master->data['check_status'] != 'A')) {
    $tpl->setVariable('hits','');
    } else {
    WEBPAGE::$dbh->query(sprintf("update tblLoansMaster set chk_process = (chk_process + 1) where id = %s",$master->data['id']));
    $tpl->setVariable('hits',1);   
    }
?>
