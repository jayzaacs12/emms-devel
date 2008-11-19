<?
require_once './includes/LN.LIB.functions.inc';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';
require_once 'class/user.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';

$loan = new LOAN($_GET['id']);
$loan->load_borrower($loan->data['client_id']);
$ltype = new LOAN_TYPE($loan->data['loan_type_id']);	

$data = WEBPAGE::$dbh->getAll(sprintf("select '-' as inx, lsh.date,RIGHT(lsh.time,8) as time,lsh.status, concat(u.first,' ',u.last) as user, lsh.memo from tblLoanStatusHistory as lsh, tblUsers as u where lsh.loan_id = %s and u.id = lsh.user_id order by lsh.id", $_REQUEST['id']));
$c = 1;
foreach ($data as $key=>$val) {
  $data[$key]['inx'] = $c++;
  $data[$key]['status'] = $_LABELS[sprintf('tblLoans.status.%s',$val['status'])];
  }
$head = array('inx'=>'#','date'=>$_LABELS['date'],'time'=>$_LABELS['tblLoanStatusHistory.time'],'status'=>$_LABELS['tblLoans.status'],'user'=>$_LABELS['tblUsers.id'],'memo'=>$_LABELS['memo']);

$tpl->setVariable('borrower_name',			$loan->data['borrower_name']);
$tpl->setVariable('borrower_code',			$loan->data['borrower_code']);
$tpl->setVariable('borrower_label',			$_LABELS['tblClients.id']);
$tpl->setVariable('loan_code',				$loan->data['loan_code']);
$tpl->setVariable('loan_code_label',		$_LABELS['tblLoans.loan_code']);
$tpl->setVariable('currency_symbol',		$ltype->data['currency_symbol']);
$tpl->setVariable('statusHistory_label',	$_LABELS['LN.RPT.loanStatusHistory']);
$tpl->setVariable('modality',				$ltype->data['description']);
$tpl->setVariable('modality_label',			$_LABELS['tblLoanTypes.description']);

$tpl->setVariable('chart',			WEBPAGE::printchart($data,$head));

?>