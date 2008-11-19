<?php
require_once './includes/LN.LIB.functions.inc';
$loan_status = unpackloanstatus($_CONF[loan_status],';','|',',');

// being defensive from user's hacking
if ((!(($_REQUEST[id])&&($_REQUEST[status])&&($_REQUEST[p_status])))||(!( (in_array($_REQUEST[status], $loan_status[$_REQUEST[p_status]][options])) && (WEBPAGE::$userAccessCode & intval($loan_status[$_REQUEST[status]][mask])) ))) {
  $message = 'loggedOut';
  $auth->logout();
  loginFunction();
  exit;
  }

require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/program.php';
require_once 'class/roles.php';
require_once 'class/client.php';
require_once 'class/society.php';
require_once 'class/business_type.php';
require_once 'class/business.php';
require_once 'class/zone.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';

$loan = new LOAN($_REQUEST['id']);
($loan->data['status'] == 'D') 	? WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.doCheckRelease&id=%s',$loan->data['master_id'])) : '';
$loan->survey_check() 			? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=SV.SCR.applySurveyStepB&loan_id=%s&client_id=%s&id=%s&ico=inf&msg=rqloan.inf.survey',$loan->data['id'],$loan->data['client_id'],$_CONF['survey_id']));
$loan->business_check() 		? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.pickBusiness&loan_id=%s&client_id=%s&ico=inf&msg=rqloan.inf.business',$loan->data['id'],$loan->data['client_id']));

$loan->load_borrower($loan->data['client_id']);
$program = new PROGRAM($loan->data['program_id']);

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('status_label',		$_LABELS['tblLoans.status']);
$tpl->setVariable('p_status',			$_LABELS[sprintf("tblLoans.status.%s",$_REQUEST['p_status'])]);
$tpl->setVariable('status',			$_LABELS[sprintf("tblLoans.status.%s",$_REQUEST['status'])]);
$tpl->setVariable('loan_code_label',	        $_LABELS['tblLoans.loan_code']);
$tpl->setVariable('loan_code',			$loan->data['loan_code']);
$tpl->setVariable('client_label',		$_LABELS['tblClients.id']);
$tpl->setVariable('borrower_name',		$loan->data['borrower_name']);
$tpl->setVariable('borrower_code',		$loan->data['borrower_code']);

$form = new HTML_QuickForm('updateStatusForm','POST');

$form->addElement('static', 	                'submit', 	'', $btn->imgsubmit('updateStatusForm',$_LABELS['submit']));
$form->addElement('static', 	                'reset', 	'', $btn->imgreset('updateStatusForm',$_LABELS['reset']));
$form->addElement('static', 	                'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden',		        'scr_name',			$_REQUEST['scr_name']);
$form->addElement('hidden',		        'ref',				$_REQUEST['ref']);
$form->addElement('hidden',		        'id',				$_REQUEST['id']);
$form->addElement('hidden',		        'status',			$_REQUEST['status']);
$form->addElement('hidden',		        'p_status',			$_REQUEST['p_status']);
$form->addElement('hidden',		        'margin_c',		$ltype->data['margin_c']);

$form->addElement('textarea',	'memo',		$_LABELS['memo'], 'rows=5');

//add rules
$form->addRule('memo',		sprintf($_LABELS['maxlength'],255),'maxlength', 255);

if ($_REQUEST[status] == 'D') {
  $form->addElement('select',		'fund',		$_LABELS['tblFunds.id'],		(array('-'=>'') + $program->funds()), 'class=large');
  $form->addRule('fund',			$_LABELS['selectOption'],'required');
  $form->addRule('fund',			$_LABELS['selectOption'],'alphanumeric');
  }

if ($_REQUEST['status'] == 'LI') { //if 'legal-in' (loan writeoff) print details of pending payments

  $li_details = WEBPAGE::$dbh->getAll(sprintf("select lcd.balance_kp,lcd.balance_kaf,lcd.balance_kat,lod.interest,lod.penalties,lcd.cn_delay,lod.hits from (tblLoansCurrentData lcd) left join tblLoansOnDelinquency lod on lod.loan_id = lcd.loan_id where lcd.loan_id = '%s' and lod.date = lcd.cn_date",$_REQUEST['id']));
  $li_details_head = array('balance_kp'=>$_LABELS['KP'],'balance_kaf'=>$_LABELS['INS'],'balance_kat'=>$_LABELS['FEES'],'interest'=>$_LABELS['RT'],'penalties'=>$_LABELS['PN'],'delay'=>$_LABELS['delay'],'hits'=>$_LABELS['due_payments']);

  $tpl->setVariable('li_details_label', 	$_LABELS['current_balance']);
  $tpl->setVariable('li_details', 	WEBPAGE::printchart($li_details,$li_details_head));

  }

if ($form->validate()) {

  $form->freeze();
  $values = $form->_submitValues;

  switch ($values[status]) {
    case "R": break;
    case "S": break;
    case "A": break;
    case "D":
      //insert new record at tblFundsLoansPct
      SQL::insert('tblFundsLoansPct',array('id'=>'Null','loan_id'=>$values['id'],'fund_id'=>$values['fund'],'pct'=>'100'));
      break;
    case "G": break;
    case "LI":
      $li_details = current($li_details);
      $amount = $li_details['balance_kp']+$li_details['balance_kaf']+$li_details['balance_kat']+$li_details['interest']+$li_details['penalties'];
      WEBPAGE::$dbh->query(sprintf("insert into tblLoanWriteOff (id,loan_id,amount,principal,insurance,fees,interest,penalties,date,user_id) values ('Null','%s','%s','%s','%s','%s','%s','%s','%s','%s')",$values['id'],$amount,$li_details['balance_kp'],$li_details['balance_kaf'],$li_details['balance_kat'],$li_details['interest'],$li_details['penalties'],date('Y-m-d'),WEBPAGE::$userID));
      WEBPAGE::$dbh->query(sprintf("delete from tblLoansCurrentData where loan_id = %s", $values['id']));
      break;
    case "LO": break;
    case "RT": break;
    case "RO": break;
    case  "O": break;
    default: exit;
    }

    //Insert comment
    SQL::insert('tblLoanStatusHistory',array('id'=>'Null','loan_id'=>$values['id'],'p_status'=>$values['p_status'],'status'=>$values['status'],'date'=>date('Y-m-d'),'user_id'=>WEBPAGE::$userID,'memo'=>$values['memo']));

    //update status
    $lastID = SQL::insert('tblLoans',array('id'=>$values['id'],'status'=>$values['status'],$editor_field=>WEBPAGE::$userID,$date_field=>date('Y-m-d')),'id');

    WEBPAGE::redirect(sprintf("index.php?scr_name=LN.SCR.viewILoan&id=%s&ico=scs&msg=scs",$lastID));

  }

$form->setDefaults();
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>