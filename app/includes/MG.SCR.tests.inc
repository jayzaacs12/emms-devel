<?
exit;
$mrow = WEBPAGE::$dbh->getAll(sprintf("SELECT sum( l.kp ) as amount, lmd.master_id
FROM tblLoans AS l, tblLoansMasterDetails AS lmd
WHERE lmd.loan_id = l.id
GROUP BY lmd.master_id"));
$c = 1;
foreach($mrow as $key=>$val) {
  $master = WEBPAGE::$dbh->getAll(sprintf("select id,amount from tblLoansMaster where id = %s", $val['master_id']));
  if ($master[0]['amount'] != $val['amount']) {
    WEBPAGE::$dbh->query(sprintf("update tblLoansMaster set amount = '%s' where id = %s", $val['amount'], $val['master_id']));
    printf("<br>%s::%s",$c,$val['master_id']); $c++;
	}
  }

?>