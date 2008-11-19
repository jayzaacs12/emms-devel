<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'Date.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/user.php';
require_once 'class/client.php';
require_once 'class/society.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';

$client = new CLIENT();
$btn  	= new TTFButton(WEBPAGE::$theme);

$form 	= new HTML_QuickForm('applyForm','POST');

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);
$form->addElement('static', 	'submit', 		'', $btn->imgsubmit('applyForm',$_LABELS['submit']));
$form->addElement('text',		'notes',		$_LABELS['memo'], 'class=large');

$_CONF['pmt_receipt_flag_a'] ?  $form->addElement('advcheckbox','pmt_receipt_flag_a',	'', $_CONF['pmt_receipt_flag_a'],'class=checkbox') : '';
$_CONF['pmt_receipt_flag_b'] ?  $form->addElement('advcheckbox','pmt_receipt_flag_b',	'', $_CONF['pmt_receipt_flag_b'],'class=checkbox') : '';

$data = WEBPAGE::$dbh->getAll(sprintf("select lm.borrower_id, lcd.loan_id, lmd.master_id, l.client_id, l.loan_code, concat(c.first,' ',c.last) as client from tblLoansCurrentData as lcd, tblLoansMasterDetails as lmd, tblLoansMaster as lm, tblLoans as l, tblClients as c where c.id = l.client_id and l.id = lcd.loan_id and lcd.xp_pmt_date <= lcd.cn_date and lcd.cn_delay <= l.margin_d and lmd.loan_id = lcd.loan_id and lm.id = lmd.master_id and lm.borrower_type = 'B' and lm.id = %s", $_REQUEST['id']));
$totals = array('inx'=>$_LABELS['total'],'client'=>'','loan_code'=>'','date'=>'','fees'=>0,'insurances'=>0,'principal'=>0,'interest'=>0,'pmt'=>0,'balance_kp'=>'','apply'=>'');

$c = 0;

foreach($data as $key=>$val) {
  $society_id                = $val['borrower_id'];
  $master[$val['master_id']] = $val['master_id'];

  $client->data['id'] = $val['client_id'];
  $client->checkConfidentiality();
  if ($client->data[confidential]) {
    $val['client'] = $client->data['name'];
    }

  $loan = new LOAN($val['loan_id']);
  $loan->load_borrower($loan->data['client_id']);

  $pmt_data							= $loan->getNextPaymentData();

  $pmt[$c]['inx']					= $c + 1;
  $pmt[$c]['client']				= $val['client'];
  $pmt[$c]['loan_code']				= $val['loan_code'];
  $pmt[$c]['date']					= $pmt_data['pmt_date'];
  $pmt[$c]['fees']					= $pmt_data['fees'];
  $pmt[$c]['insurances']			= $pmt_data['insurances'];
  $pmt[$c]['principal']				= $pmt_data['principal'];
  $pmt[$c]['interest']				= $pmt_data['interest'];
  $pmt[$c]['pmt']					= $pmt_data['pmt'];
  $pmt[$c]['balance_kp']			= $pmt_data['balance_kp'];
  $pmt[$c]['apply']					= sprintf("<input type=checkbox name='%s' id='%s' class=checkbox checked>", $val['loan_id'], $val['loan_id']);

  $totals['fees']					+= $pmt[$c]['fees'];
  $totals['insurances']				+= $pmt[$c]['insurances'];
  $totals['principal']				+= $pmt[$c]['principal'];
  $totals['interest']				+= $pmt[$c]['interest'];
  $totals['pmt']					+= $pmt[$c]['pmt'];

  $c++;

  $idata[$val['loan_id']]['date']			= $loan->data['cn_date'];
  $idata[$val['loan_id']]['pmt']			= $pmt_data['pmt'];
  $idata[$val['loan_id']]['penalties']		= $loan->data['cn_penalties'];
  $idata[$val['loan_id']]['delay']			= $loan->data['cn_delay'];
  $idata[$val['loan_id']]['interest']		= $pmt_data['interest'];
  $idata[$val['loan_id']]['insurances']		= $pmt_data['insurances'];
  $idata[$val['loan_id']]['fees']			= $pmt_data['fees'];
  $idata[$val['loan_id']]['principal']		= $pmt_data['principal'];
  $idata[$val['loan_id']]['balance_kp']		= $pmt_data['balance_kp'];
  $idata[$val['loan_id']]['balance_kaf']	= $pmt_data['balance_kaf'];
  $idata[$val['loan_id']]['balance_kat']	= $pmt_data['balance_kat'];
  $idata[$val['loan_id']]['r_from_date']	= $pmt_data['r_from_date'];
  $idata[$val['loan_id']]['xp_pmt_date']	= $pmt_data['xp_pmt_date'];
  $idata[$val['loan_id']]['xp_pmt']			= $pmt_data['xp_pmt'];
  }

$pmt[$c] = $totals;

if ($form->validate()) {

  if (count($master)==1) {
    $receipt_data = WEBPAGE::$dbh->getAll(sprintf("select max(p.id) as last_pID, %s as master_id from tblPayments as p, tblLoansMasterDetails as lmd, tblLoans as l where lmd.master_id = %s and p.loan_id = lmd.loan_id", current($master), current($master)));
    } else {
    exit; // do something better here...
    }

  $loan = new LOAN();
  $loan->data['status'] 		= 'G';
  $pmtIDs = array();
  foreach($idata as $key=>$val) {
  	$loan->data['id'] 			= $key;
  	$loan->data['balance_kp'] 	= $val['balance_kp'];
  	$loan->data['cn_date'] 		= $val['date'];
  	if ($_POST[$key]) {
  	  WEBPAGE::$dbh->query(sprintf("insert into tblPayments (loan_id,date,pmt,penalties,delay,interest,insurances,fees,principal,balance_kp,balance_kaf,balance_kat,user_id) values ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",$key,$val['date'],$val['pmt'],$val['penalties'],$val['delay'],$val['interest'],$val['insurances'],$val['fees'],$val['principal'],$val['balance_kp'],$val['balance_kaf'],$val['balance_kat'],WEBPAGE::$userID));
  	  $pmtIDs[] = mysql_insert_id(WEBPAGE::$dbh->connection);
  	  WEBPAGE::$dbh->query(sprintf("update tblLoansCurrentData set balance_kp = '%s', balance_kaf = '%s', balance_kat = '%s', r_from_date = '%s', xp_pmt_date = '%s', xp_pmt = '%s', cn_delay = 0, cn_penalties	= 0 where loan_id = '%s'", $val['balance_kp'],$val['balance_kaf'],$val['balance_kat'],$val['r_from_date'],$val['xp_pmt_date'],$val['xp_pmt'],$key));
  	  $loan->data['balance_kp'] ? '' : $loan->cancelled();
  	  }
    }
  if(( $n = count($pmtIDs)) > 0) {
  	$balance = WEBPAGE::$dbh->getAll(sprintf("select lmd.master_id, sum(lcd.balance_kp) balance_kp, sum(lcd.balance_kat) balance_kat, sum(balance_kaf) balance_kaf from tblLoansCurrentData lcd, tblLoansMasterDetails lmd where lmd.master_id = %s and lcd.loan_id = lmd.loan_id group by lmd.master_id",$_REQUEST['id']));
    if (count($balance) != 1) {
      $balance_kp = 0; $balance_kaf = 0; $balance_kat = 0;
      } else {
      $balance_kp = $balance[0]['balance_kp']; $balance_kaf = $balance[0]['balance_kaf']; $balance_kat = $balance[0]['balance_kat'];
      }
    $receipt_id = SQL::insert('tblReceipts',array('loanmaster_id'=>$_REQUEST['id'],'balance_kp'=>$balance_kp,'balance_kaf'=>$balance_kaf,'balance_kat'=>$balance_kat,'notes'=>$_REQUEST['notes'],'flag_a'=>$_REQUEST['pmt_receipt_flag_a'] ? 1 : 0, 'flag_b'=>$_REQUEST['pmt_receipt_flag_b'] ? 1 : 0, 'user_id'=>WEBPAGE::$userID,'date'=>date('Y-m-d')));
    $lnkData = array();
    foreach($pmtIDs as $key=>$val) {
      $lnkData[] = sprintf("(%s,%s)",$receipt_id,$val);
      }
    SQL::insert_mult('tblLinkReceiptsPayments','receipt_id,payment_id',implode(',',$lnkData));
    WEBPAGE::redirect(sprintf("index.popup.php?scr_name=LN.SCR.viewReceipt&id=%s",$receipt_id));
    }

  } else {



  extract(SOCIETY::getTemplateData($society_id));

  $tpl->setVariable('addPaymentBulk',		$_LABELS['LN.SCR.addPaymentBulk']);
  $tpl->setVariable('cn_date',				date('Y-m-d'));

  $tpl->setVariable('society_label',		$_LABELS[sprintf('tblSocieties.category.%s',$category)]);
  $tpl->setVariable('society',				$name);
  $tpl->setVariable('code',					$code);
  $tpl->setVariable('zone_label',		    $_LABELS['tblSocieties.zone_id']);
  $tpl->setVariable('zone',					$zone);

  $head = array('#',$_LABELS['tblClients.id'],$_LABELS['tblLoans.loan_code'],$_LABELS['tblPayments.date'],$_LABELS['tblPayments.fees'],$_LABELS['tblPayments.insurances'],$_LABELS['tblPayments.principal'],$_LABELS['tblPayments.interest'],$_LABELS['tblPayments.pmt'],$_LABELS['tblPayments.balance_kp'],$_LABELS['apply']);

  $tpl->setVariable('due_chart', 			WEBPAGE::printchart($pmt,$head));

  $form->setDefaults();
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $form->accept($renderer);

  }
?>
