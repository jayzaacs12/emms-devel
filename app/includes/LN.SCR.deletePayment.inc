<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';

$tpl->setVariable('content_title', $_LABELS['LN.SCR.deletePayment']);

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('applyForm','POST');

$form->addElement('hidden',		'scr_name',			$_REQUEST['scr_name']);
$form->addElement('hidden',		'ref',				$_REQUEST['ref']);
$form->addElement('hidden',		'id',				$_REQUEST['id']);
$form->addElement('hidden',		'zone_id',			$_REQUEST['zone_id']);
$form->addElement('hidden',		'advisor_id',		$_REQUEST['advisor_id']);
$form->addElement('hidden',		'date',				$_REQUEST['date']);
$form->addElement('hidden',		'borrower_type',	$_REQUEST['borrower_type']);
$form->addElement('static', 	'submit', 			'', $btn->imgsubmit('applyForm',$_LABELS['LN.SCR.deletePayment']));

if ($form->validate()) { 
 
  $values = $form->_submitValues;
  
  //get loan_id and payment date
  $data = WEBPAGE::$dbh->getAll(sprintf("select l.status,l.id,p.date from tblLoans as l, tblPayments as p where p.id = %s and l.id = p.loan_id", $values['id']));
  $date 		= $data[0]['date'];
  $loan_id 		= $data[0]['id'];
  $loan_status 	= $data[0]['status'];
  
  //rollback tblPayments
  WEBPAGE::$dbh->query(sprintf("insert into tblPaymentsRollback select * from tblPayments where loan_id = %s and date = '%s'", $loan_id,$date));
  WEBPAGE::$dbh->query(sprintf("delete from tblPayments where loan_id = %s and date = '%s'", $loan_id,$date));

  //rollback tblLoansCurrentData
  WEBPAGE::$dbh->query(sprintf("delete from tblLoansCurrentData where loan_id = %s", $loan_id));
  WEBPAGE::$dbh->query(sprintf("insert into tblLoansCurrentData select * from tblLoansCurrentDataBackup where loan_id = %s", $loan_id));

  //rollback tblLoanStatusHistory and tblLoans.status if required
  if ($loan_status == 'C') {
  	WEBPAGE::$dbh->query(sprintf("update tblLoans set status = 'G' where id = %s", $loan_id));
  	WEBPAGE::$dbh->query(sprintf("delete from tblLoanStatusHistory where status = 'C' and loan_id = %s", $loan_id));
    }
  
  WEBPAGE::redirect(sprintf("index.popup.php?scr_name=LN.SCR.browseSubmittedPayments&date=%s&zone_id=%s&advisor_id=%s&borrower_type=%s",$values['date'],$values['zone_id'],$values['advisor_id'],$values['borrower_type']));

  } else {

//$mrow = WEBPAGE::$dbh->getAll(sprintf("select * from tblPayments where id = %s and user_id = %s and transaction_id = 0", $_REQUEST['id'],WEBPAGE::$userID));
  $mrow = WEBPAGE::$dbh->getAll(sprintf("SELECT l.loan_code, concat(c.first, ' ', c.last) as client, p.pmt, p.principal, p.insurances, p.fees, p.interest, p.penalties, p.balance_kp, IF(p.special,'%s','%s') as special FROM tblLoans as l, tblClients as c, tblPayments as p, tblUsers as u WHERE u.id = p.user_id and p.transaction_id = 0 and l.id = p.loan_id and c.id = l.client_id and p.loan_id in (select loan_id from tblPayments where id = %s) and p.date = '%s' and p.user_id = %s order by client,p.id",$_LABELS['yes'],$_LABELS['no'],$_REQUEST['id'],$_REQUEST['date'],WEBPAGE::$userID));
  $head = array('loan_code'=>$_LABELS['tblLoans.loan_code'],'client'=>$_LABELS['tblClients.id'],'pmt'=>$_LABELS['tblPayments.pmt'],'principal'=>$_LABELS['tblPayments.principal'],'insurances'=>$_LABELS['tblPayments.insurances'],'fees'=>$_LABELS['tblPayments.fees'],'interest'=>$_LABELS['tblPayments.interest'],'penalties'=>$_LABELS['tblPayments.penalties'],'balance_kp'=>$_LABELS['tblPayments.balance_kp'],'special'=>$_LABELS['tblPayments.special']);
  if (!count($mrow)) {
    $tpl->setVariable('message', 	WEBPAGE::printmessage('err',sprintf('%s<br>%s',$_LABELS['stp'],$_LABELS['err'])));
    } else {
    $total  = array();
    $mrow[] = $total;
    $tpl->setVariable('message', 	WEBPAGE::printmessage('wrn',sprintf('%s<br>%s',$_LABELS['wrn.deletePayment'],$_LABELS['confirm'])));
    $tpl->setVariable('chart', WEBPAGE::printchart($mrow,$head));
    }

  $form->setDefaults();
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $form->accept($renderer);

  }



?>