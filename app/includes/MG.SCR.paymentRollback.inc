<?
exit;
//$loan = WEBPAGE::$dbh->getAll("SELECT p.loan_id from tblPayments AS p, tblLoans AS l where p.loan_id = l.id and l.advisor_id =57 and p.date = '2006-03-30'");

//foreach ($loan as $key=>$val) {

//  $_REQUEST['id'] = $val['loan_id'];

  //get payment data
  $data = WEBPAGE::$dbh->getAssoc(sprintf("select id,transaction_id,pmt as amount,principal,fees,insurances,interest,penalties from tblPayments where loan_id = %s order by id",$_REQUEST['id']));
  $data = array_pop($data);

  //fix tblTCredits
  WEBPAGE::$dbh->query(sprintf("update tblTCredits set amount = (amount - %s), principal = (principal - %s), fees = (fees - %s), insurances = (insurances - %s), interest = (interest - %s), penalties = (penalties - %s) where code = '%s'", $data['amount'],$data['principal'],$data['fees'],$data['insurances'],$data['interest'],$data['penalties'],$data['transaction_id']));
  //printf("<br>update tblTCredits set amount = (amount - %s), principal = (principal - %s), fees = (fees - %s), insurances = (insurances - %s), interest = (interest - %s), penalties = (penalties - %s) where code = '%s'", $data['amount'],$data['principal'],$data['fees'],$data['insurances'],$data['interest'],$data['penalties'],$data['transaction_id']);

  //fix tblLoansCurrentData .... solo cuando se eliminan todos los pagos, de lo contrario seria desde bk
  WEBPAGE::$dbh->query(sprintf("delete from tblLoansCurrentData where loan_id = '%s'", $_REQUEST['id']));
  //printf("<br>delete from tblLoansCurrentData where loan_id = '%s'", $_REQUEST['id']);
  WEBPAGE::$dbh->query(sprintf("insert into tblLoansCurrentData (loan_id,balance_kp,balance_kaf,balance_kat,r_from_date,xp_pmt_date,xp_pmt,cn_date,cn_delay,cn_penalties) select l.id,l.kp,l.kaf,l.kat,l.delivered_date,l.first_payment_date,l.pmt,l.delivered_date,0,0 from tblLoans as l where l.id = %s",$_REQUEST['id']));
  //printf("<br>insert into tblLoansCurrentData (loan_id,balance_kp,balance_kaf,balance_kat,r_from_date,xp_pmt_date,xp_pmt,cn_date,cn_delay,cn_penalties) select l.id,l.kp,l.kaf,l.kat,l.delivered_date,l.first_payment_date,l.pmt,l.delivered_date,0,0 from tblLoans as l where l.id = %s",$_REQUEST['id']);

  //fix tblPayments
  WEBPAGE::$dbh->query(sprintf("delete from tblPayments where loan_id = '%s'", $_REQUEST['id']));
  //printf("<br>delete from tblPayments where loan_id = '%s'", $_REQUEST['id']);

//  }
?>