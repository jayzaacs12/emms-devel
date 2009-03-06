<?php
/*
 * Upon deletion, MySQL triggers will log  relevant changes
 * For more information see triggers on tables
 * tblLoans
 * tblLoanStatusHistory
 * tblLoansMaster
 * tblLoansMasterDetails
 * tblFundsLoansMasterPct
 *
 */
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/loan_master.php';

$loan = new LOAN($_REQUEST['id']);
switch ($loan->data['status']) {
  case 'N' : break;
  case 'O' : break;
  case 'S' : break;
  case 'A' : break;
  case 'D' : break;
  case 'R' : break;
  case 'RO': break;
  case 'RT': break;
  default: 	
    WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.viewILoan&id=%s&ico=err&msg=loan.delete.err',$loan->data['id']));
  }
// Next line is important to track down date and user deleting the record using a db trigger
WEBPAGE::$dbh->query(sprintf("update tblLoans set editor_id = '%s', editor_date = CURDATE() where id = '%s'", WEBPAGE::$userID,$loan->data['id']));

WEBPAGE::$dbh->query(sprintf("delete from tblLoansMasterDetails where loan_id = '%s'", $loan->data['id']));
WEBPAGE::$dbh->query(sprintf("delete from tblLoanStatusHistory where loan_id = '%s'", $loan->data['id']));
WEBPAGE::$dbh->query(sprintf("delete from tblLoans where id = '%s'", $loan->data['id']));

LOAN_MASTER::updateAmount($loan->data['master_id']);

// si no hay prestamos debajo del loanMaster, entonces eliminar el loanMaster
if (!count(WEBPAGE::$dbh->getAll(sprintf("select master_id from tblLoansMasterDetails where master_id = '%s'",$loan->data['master_id'])))) {
  // Next line is important to track down date and user deleting the record using a db trigger
  WEBPAGE::$dbh->query(sprintf("update tblLoansMaster set editor_id = '%s', editor_date = CURDATE() where id = '%s'", WEBPAGE::$userID,$loan->data['master_id']));

  WEBPAGE::$dbh->query(sprintf("delete from tblLoansMaster where id = '%s'", $loan->data['master_id']));
  WEBPAGE::$dbh->query(sprintf("delete from tblFundsLoansMasterPct where master_id = '%s'", $loan->data['master_id']));
  WEBPAGE::redirect('index.php?scr_name=LN.SCR.browseChecks&ico=scs&msg=loan.delete.scs');
  }

WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.viewLoanMaster&id=%s&ico=scs&msg=loan.delete.scs',$loan->data['master_id']));
?>
