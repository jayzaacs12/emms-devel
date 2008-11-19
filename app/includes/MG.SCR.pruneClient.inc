<?php
exit;
//$id_array = WEBPAGE::$dbh->getAll("select id from tblClients where advisor_id = 0 limit 30");
//foreach ($id_array as $outer_key=>$outer_val)
//{
//$_REQUEST['id'] = $outer_val[id];

$_REQUEST['id'] ? '' : exit;
WEBPAGE::$dbh->query(sprintf("delete from tblBusiness where client_list = '%s'", $_REQUEST['id']));
WEBPAGE::$dbh->query("delete from tblBusinessIOM where business_id not in (select id from tblBusiness)");
WEBPAGE::$dbh->query(sprintf("delete from tblClients where id = '%s'", $_REQUEST['id']));
WEBPAGE::$dbh->query(sprintf("delete from tblClientIOM where id = '%s'", $_REQUEST['id']));
WEBPAGE::$dbh->query(sprintf("delete from tblSurveyAnswers where client_id = '%s'", $_REQUEST['id']));

$loans = WEBPAGE::$dbh->getAll(sprintf("select id from tblLoans where client_id = '%s'", $_REQUEST['id']));
foreach ($loans as $key=>$val) {
  WEBPAGE::$dbh->query(sprintf("delete from tblLoansMasterDetails where loan_id = '%s'", $val['id']));
  WEBPAGE::$dbh->query(sprintf("delete from tblLoanStatusHistory where loan_id = '%s'", $val['id']));
  WEBPAGE::$dbh->query(sprintf("delete from tblLoans where id = '%s'", $val['id']));
  WEBPAGE::$dbh->query(sprintf("delete from tblLoansCurrentData where loan_id = '%s'", $val['id']));

  $master_id = current(WEBPAGE::$dbh->getAssoc(sprintf("select loan_id,master_id from tblMasterDetails where loan_id = '%s'", $val['id'])));
  if ($master_id) {
    WEBPAGE::$dbh->query(sprintf("update tblLoansMaster as lm set lm.amount = (select sum(l.kp) from tblLoans as l, tblLoansMasterDetails as lmd where lmd.master_id = %s and lmd.loan_id = l.id) where lm.id = %s",$master_id,$master_id));
    } else {
    WEBPAGE::$dbh->query(sprintf("delete from tblLoansMaster where id = '%s'", $master_id));
    WEBPAGE::$dbh->query(sprintf("delete from tblFundsLoansMasterPct where master_id = '%s'", $master_id));
    }
  }
//}
//WEBPAGE::redirect('index.popup.php?scr_name=MG.SCR.clientLookUp');
WEBPAGE::redirect(sprintf('index.popup.php?scr_name=MG.SCR.browseClients&advisor_id=%s',$_REQUEST['advisor_id']));
?>