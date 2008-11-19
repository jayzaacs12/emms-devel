<?php
exit;
$master_id = current(WEBPAGE::$dbh->getAssoc(sprintf("select loan_id,master_id from tblLoansMasterDetails where loan_id = '%s'", $_REQUEST['id'])));

WEBPAGE::$dbh->query(sprintf("delete from tblLoansMasterDetails where loan_id = '%s'", $_REQUEST['id']));

$master_id_post = current(WEBPAGE::$dbh->getAssoc(sprintf("select loan_id,master_id from tblLoansMasterDetails where master_id = '%s'", $master_id)));
if ($master_id_post) {
  WEBPAGE::$dbh->query(sprintf("update tblLoansMaster as lm set lm.amount = (select sum(l.kp) from tblLoans as l, tblLoansMasterDetails as lmd where lmd.master_id = %s and lmd.loan_id = l.id) where lm.id = %s",$master_id,$master_id));
  } else {
  WEBPAGE::$dbh->query(sprintf("delete from tblLoansMaster where id = '%s'", $master_id));
  WEBPAGE::$dbh->query(sprintf("delete from tblFundsLoansMasterPct where master_id = '%s'", $master_id));
  }

WEBPAGE::$dbh->query(sprintf("delete from tblLoanStatusHistory where loan_id = '%s'", $_REQUEST['id']));
WEBPAGE::$dbh->query(sprintf("delete from tblLoans where id = '%s'", $_REQUEST['id']));
WEBPAGE::$dbh->query(sprintf("delete from tblLoansCurrentData where loan_id = '%s'", $_REQUEST['id']));

//WEBPAGE::redirect('index.popup.php?scr_name=MG.SCR.clientLookUp');
WEBPAGE::redirect(sprintf('index.popup.php?scr_name=MG.SCR.browseClients&advisor_id=%s',$_GET['advisor_id']));
?>