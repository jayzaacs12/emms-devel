<?php
// echo("breakpoint 1");
// exit;

$_REQUEST['bid']  	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.pickBorrower&ico=%s&msg=%s',$_REQUEST['ico'],$_REQUEST['msg']));
$_REQUEST['bt']   	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.pickBorrower&ico=%s&msg=%s',$_REQUEST['ico'],$_REQUEST['msg']));
$checkloan = WEBPAGE::$dbh->getAll(sprintf("select id from tblLoansMaster where borrower_id = %s and borrower_type = '%s' and check_status != 'R'",$_REQUEST['bid'],$_REQUEST['bt']));
!count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.viewLoanMaster&id=%s&ico=inf&msg=inf.loanRequestInProcess',$checkloan[0]['id']));
$_REQUEST['ltid'] 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.pickLoanType&bt=%s&bid=%s',$_REQUEST['bt'],$_REQUEST['bid']));

require_once './includes/LN.LIB.functions.inc';
require_once 'class/zone.php';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/roles.php';
require_once 'class/client.php';
require_once 'class/society.php';
require_once 'class/borrower.php';
 // PATCH: 2008.07.21
require_once 'Date.php';

$borrower = new BORROWER($_REQUEST['bt'],$_REQUEST['bid']);



is_array($borrower->data['members']) ? '' : WEBPAGE::redirect('index.php?scr_name=LN.SCR.requestLoan&ico=err&msg=rqloan.err.borrower');



$master 	= new LOAN_MASTER();
$zone 		= new ZONE($borrower->data['zone_id']);
$loan 		= new LOAN(0,$_REQUEST['ltid']);

$master->data['borrower_id'] 	= $_REQUEST['bid'];
$master->data['borrower_type'] 	= $_REQUEST['bt'];
$master->data['loan_type_id'] 	= $_REQUEST['ltid'];
$master->data['zone_id']		= $zone->zonedata['parent_id'];
$master->data['program_id']     = $zone->zonedata['program_id'];

// PATCH: 2008.07.21
if (!$master->data['id']){
  $xdate = new Date();
  $xdate->AddSeconds(24*60*60*$_CONF['xp_delivered_date_margin']);
  $master->data['xp_delivered_date'] =  $xdate->format('%Y-%m-%d');
  $master->data['xp_first_payment_date'] = $loan->next_payment_date($xdate->format('%Y-%m-%d'));
}


$master_id = SQL::insert('tblLoansMaster',$master->data, 'id');

$loan->data['zone_id']			= $zone->zonedata['parent_id'];
$loan->data['client_zone_id']   = $borrower->data['zone_id'];
$loan->data['program_id']       = $zone->zonedata['program_id'];
$loan->data['advisor_id']       = $borrower->data['advisor_id'];


foreach($borrower->data['members'] as $key => $val) {
  $loan->data['loan_code'] = (date(ym).'.'.str_pad($loan->data['zone_id'], 3, '0', STR_PAD_LEFT).'.'.str_pad($_REQUEST['ltid'], 3, '0', STR_PAD_LEFT).'.'.str_pad($key, 6, '0', STR_PAD_LEFT));
  $loan->data['client_id'] = $key;
  $loan_id = SQL::insert('tblLoans',$loan->data,'id');
  SQL::insert('tblLoansMasterDetails',array('master_id'=>$master_id,'loan_id'=>$loan_id));
  }

WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.viewLoanMaster&id=%s&ico=scs&msg=rqloan.scs',$master_id));

?>