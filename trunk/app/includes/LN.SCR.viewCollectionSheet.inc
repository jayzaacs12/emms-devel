<?php
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';
require_once 'class/society.php';

$client = new CLIENT();
extract(SOCIETY::getTemplateData($_REQUEST['id']));

$tpl->setVariable('viewCollectionSheet',	$_LABELS['LN.SCR.viewCollectionSheet']);
$tpl->setVariable('cn_date',				date('Y-m-d'));

$tpl->setVariable('society_label',			$_LABELS[sprintf('tblSocieties.category.%s',$category)]);
$tpl->setVariable('society',				$name);
$tpl->setVariable('code',					$code);
$tpl->setVariable('zone_label',		    	$_LABELS['tblSocieties.zone_id']);
$tpl->setVariable('zone',					$zone);

$mrow = WEBPAGE::$dbh->getAll(sprintf("select lmd.loan_id from tblLoansMasterDetails as lmd where lmd.master_id in (select lm.id from tblLoansMaster as lm where lm.borrower_type = 'B' and lm.borrower_id = %s and lm.check_status = 'R')", $_GET['id']));
foreach ($mrow as $key=>$val) {
  $loans[] = $val['loan_id'];
  }
$loans = implode(',',$loans);

$mrow = WEBPAGE::$dbh->getAll(sprintf("select l.client_id, concat(c.first,' ',c.last) as client, l.loan_code, l.kp, l.kaf, l.kat, l.savings_p, l.savings_v, lcd.* from tblLoansCurrentData as lcd, tblLoans as l, tblClients as c where c.id = l.client_id and l.id = lcd.loan_id and lcd.loan_id in (%s)", $loans));
$c = 1;
$data = array();
foreach($mrow as $key => $row) {
  $client->data['id'] = $row['client_id'];
  $client->checkConfidentiality();
  if ($client->data[confidential]) {
    $mrow[$key]['client'] = $client->data['name'];
    }
  $data[$key]['inx'] 			= $c++;
  $data[$key]['client'] 		= $mrow[$key]['client'];
  $data[$key]['kp'] 			= $mrow[$key]['kp'];
  $data[$key]['loan_code'] 		= $mrow[$key]['loan_code'];
  $data[$key]['balance_kp'] 	= $mrow[$key]['balance_kp'];
  $data[$key]['balance_kaf'] 	= $mrow[$key]['balance_kaf'];
  $data[$key]['balance_kat'] 	= $mrow[$key]['balance_kat'];
  $data[$key]['xp_pmt_date'] 	= $mrow[$key]['xp_pmt_date'];
  $data[$key]['xp_pmt'] 		= $mrow[$key]['xp_pmt'];
  $data[$key]['savings_p'] 		= $mrow[$key]['savings_p'];
  $data[$key]['savings_v'] 		= $mrow[$key]['savings_v'];
  $data[$key]['cn_delay'] 		= $mrow[$key]['cn_delay'];

  $total['inx'] 				 = '';
  $total['client'] 				 = $_LABELS['total'];
  $total['kp'] 					+= $mrow[$key]['kp'];
  $total['loan_code'] 			 = '';
  $total['balance_kp'] 			+= $mrow[$key]['balance_kp'];
  $total['balance_kaf'] 		+= $mrow[$key]['balance_kaf'];
  $total['balance_kat'] 		+= $mrow[$key]['balance_kat'];
  $total['xp_pmt_date'] 		 = '';
  $total['xp_pmt'] 				+= $mrow[$key]['xp_pmt'];
  $total['savings_p'] 			+= $mrow[$key]['savings_p'];
  $total['savings_v'] 			+= $mrow[$key]['savings_v'];
  $total['cn_delay'] 			+= $mrow[$key]['cn_delay'];
  }

if (count($data)) { $data['total'] = $total; }

$head = array('#',$_LABELS['tblClients.id'],$_LABELS['tblLoans.id'],$_LABELS['tblLoans.loan_code'],$_LABELS['tblPayments.principal'].'*',$_LABELS['tblPayments.insurances'].'*',$_LABELS['tblPayments.fees'].'*',$_LABELS['tblPayments.date'],$_LABELS['tblPayments.pmt'],'SAV-P','SAV-V',$_LABELS['tblPayments.delay'].'**');//, $_LABELS['tblPayments.transaction_id'],$_LABELS['tblPayments.date'],$_LABELS['tblPayments.fees'],$_LABELS['tblPayments.insurances'],$_LABELS['tblPayments.principal'],$_LABELS['tblPayments.interest'],$_LABELS['tblPayments.delay'],$_LABELS['tblPayments.penalties'],$_LABELS['tblPayments.pmt'],$_LABELS['tblPayments.balance_kp'],$_LABELS['tblUsers.id']);

$tpl->setVariable('total_label',			$_LABELS['total']);
$tpl->setVariable('pmt_label',				$_LABELS['tblPayments.pmt']);
$tpl->setVariable('pmt_SAV_P_label',		sprintf('%s + %s',$_LABELS['tblPayments.pmt'], $_LABELS['tblLoans.savings_p']));
$tpl->setVariable('pmt_SAV_P_SAV_V_label',	sprintf('%s + %s + %s',$_LABELS['tblPayments.pmt'], $_LABELS['tblLoans.savings_p'], $_LABELS['tblLoans.savings_v']));
$tpl->setVariable('pmt',					$total['xp_pmt']);
$tpl->setVariable('pmt_SAV_P',				$total['xp_pmt'] + $total['savings_p']);
$tpl->setVariable('pmt_SAV_P_SAV_V',		$total['xp_pmt'] + $total['savings_p'] + $total['savings_v']);

$tpl->setVariable('details',				$_LABELS['details']);

$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : $_LABELS['err']);
$tpl->setVariable('previousBalance', 	$_LABELS['previousBalance']);
$tpl->setVariable('days', 				$_LABELS['days']);

$tpl->setVariable('legend',				sprintf("%s: <b>SAV-P</b> - %s, <b>SAV-V</b> - %s",$_LABELS['legend'], $_LABELS['tblLoans.savings_p'],$_LABELS['tblLoans.savings_v']));

$tpl->setVariable('print', $_LABELS['printSheet']);
?>