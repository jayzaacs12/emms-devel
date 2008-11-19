<?php
require_once 'class/roles.php';
$_REQUEST['bt']  ? '' : WEBPAGE::redirect('index.php?logout=1');
$_REQUEST['bid'] ? '' : WEBPAGE::redirect('index.php?logout=1');

switch ($_REQUEST['bt']) {
  case 'B':
    require_once 'class/society.php';
    $borrower = new SOCIETY($_REQUEST['bid']);
    break;
  case 'G':
    require_once 'class/society.php';
    $borrower = new SOCIETY($_REQUEST['bid']);
    break;
  case 'I':
    require_once 'class/client.php';
    $borrower = new CLIENT($_REQUEST['bid']);
    break;
  default:
    WEBPAGE::redirect('index.php?logout=1');
    exit;
  }

$extra['param']['value']['name']	    				= $borrower->data['name'];
$extra['param']['value']['code']	    				= $borrower->data['code'];
$extra['param']['value']['zone_id']	    				= $borrower->data['zone_id'];
$extra['param']['value']['advisor_id']					= $borrower->data['advisor_id'];
$extra['param']['value']['borrower_type']	    		= $_REQUEST['bt'];

$extra['param']['label']['tblSocieties.name']	    	= $borrower->data['name'];
$extra['param']['label']['tblSocieties.code']	    	= $borrower->data['code'];
$extra['param']['label']['tblSocieties.zone_id']	    = $borrower->data['zone'];
$extra['param']['label']['tblSocieties.advisor_id']		= $borrower->data['advisor'];
$extra['param']['label']['tblLoanTypes.borrower_type']	= $_LABELS[sprintf('tblLoanTypes.borrower_type.%s',$_REQUEST['bt'])];
    
$extra['param']['ref']		    		= 'LN.SCR.requestLoan';

$fields[] 								= 'description';
$extra['description']['header']			= 'tblLoanTypes.description';
$extra['description']['pack']			= '%s';
$extra['description']['align']			= 'left';
    
$fields[] 								= 'installment';
$extra['installment']['header']			= 'tblLoanTypes.installment';
$extra['installment']['pack']			= '%s';
$extra['installment']['align']			= 'center';

$fields[] 								= 'payment_frequency';
$extra['payment_frequency']['header']	= 'tblLoanTypes.payment_frequency';
$extra['payment_frequency']['pack']		= '%s';
$extra['payment_frequency']['align']	= 'center';

$fields[] 								= 'id as view';
$extra['view']['header']				= 'view';
$extra['view']['pack']					= sprintf("<a href='%s?scr_name=LN.SCR.viewLoanType&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['view']);
$extra['view']['align']					= 'center';

$fields[] 								= 'id as requestLoan';
$extra['requestLoan']['header']			= 'requestLoan';
$extra['requestLoan']['pack']			= sprintf("<a href='%s?scr_name=LN.SCR.requestLoan&bt=%s&bid=%s&ltid=%s'>%s</a>", $PHP_SELF,$_REQUEST['bt'],$_REQUEST['bid'],'%s',$_LABELS['requestLoan']);
$extra['requestLoan']['align']			= 'center';



$mrow = SQL::select('tblLoanTypes',$fields,sprintf("borrower_type = '%s'",$_REQUEST['bt']));
foreach ($mrow as $key => $val) { 
  $mrow[$key]['payment_frequency'] = $_LABELS[sprintf("tblLoanTypes.payment_frequency.%s",$val['payment_frequency'])];
  }
array_unshift($mrow,$extra);

WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);
require_once 'BS.SCR.pager.inc';

?>