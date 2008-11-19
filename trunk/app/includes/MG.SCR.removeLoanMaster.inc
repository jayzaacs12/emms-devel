<?
exit;
$master = WEBPAGE::$dbh->getAssoc(sprintf("SELECT master_id,master_id FROM tblLoansMasterDetails WHERE loan_id IN ( SELECT id FROM tblLoans WHERE installment =0 ) GROUP BY master_id"));
$data = array();
foreach ($master as $key=>$master_id) {

// fixing tblTDebits
  WEBPAGE::$dbh->getAll(sprintf("delete * from tblTDebits where code = (select check_number from tblLoansMaster where id = %s)", $master_id));
//    printf("<br>delete * from tblTDebits where code = (select check_number from tblLoansMaster where id = %s)", $master_id);

  $loan = WEBPAGE::$dbh->getAssoc(sprintf("SELECT loan_id,loan_id FROM tblLoansMasterDetails where master_id = %s",$master_id));
  foreach ($loan as $lkey=>$loan_id) {
  	// fixing tblLoansOnDelinquency
    WEBPAGE::$dbh->query(sprintf("delete from tblLoansOnDelinquency where loan_id = '%s'", $loan_id));
//    printf("<br>delete from tblLoansOnDelinquency where loan_id = '%s'", $loan_id);

   // fixing tblRiskPortfolio - Necesita ajustes (control de fechas) para usar generalizado
   $loan_details = current(WEBPAGE::$dbh->getAssoc(sprintf("select id,zone_id,program_id,advisor_id,kp from tblLoans where id = '%s'", $loan_id)));
   WEBPAGE::$dbh->query(sprintf("update tblRiskPortfolio as amount = (amount -  %s) where zone_id = %s and program_id = %s and advisor_id = %s",$loan_details['amount'],$loan_details['zone_id'],$loan_details['program_id'],$loan_details['advisor_id']));
//    printf("<br>update tblRiskPortfolio as amount = (amount -  %s) where zone_id = %s and program_id = %s and advisor_id = %s",$loan_details['amount'],$loan_details['zone_id'],$loan_details['program_id'],$loan_details['advisor_id']);

    // fixing tblLoanStatusHistory
    WEBPAGE::$dbh->query(sprintf("delete from tblLoanStatusHistory where loan_id = '%s'", $loan_id));
//    printf("<br>delete from tblLoanStatusHistory where loan_id = '%s'", $loan_id);

    // fixing tblLoans
    WEBPAGE::$dbh->query(sprintf("delete from tblLoans where id = '%s'", $loan_id));
//    printf("<br>delete from tblLoans where id = '%s'", $loan_id);

    // fixing tblLoansCurrentData
    WEBPAGE::$dbh->query(sprintf("delete from tblLoansCurrentData where loan_id = '%s'", $loan_id));
//    printf("<br>delete from tblLoansCurrentData where loan_id = '%s'", $loan_id);
    }

  // fixing tblFundsLoansMasterPct
  WEBPAGE::$dbh->query(sprintf("delete from tblFundsLoansMasterPct where master_id = '%s'", $master_id));
//  printf("<br>delete from tblFundsLoansMasterPct where master_id = '%s'", $master_id);

  // fixing tblLoansMasterDetails
  WEBPAGE::$dbh->query(sprintf("delete from tblLoansMasterDetails where master_id = '%s'", $master_id));
//  printf("<br>delete from tblLoansMasterDetails where master_id = '%s'", $master_id);

  // fixing tblLoansMaster
  WEBPAGE::$dbh->query(sprintf("delete from tblLoansMaster where id = '%s'", $master_id));
//  printf("<br>delete from tblLoansMaster where id = '%s'", $master_id);
  }
?>