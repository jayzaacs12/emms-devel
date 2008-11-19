<?php
$show_gohome = true;

$data1 = WEBPAGE::$dbh->getAll("select * from tblLoanTypes");

foreach($data1 as $key=>$val) {

//  $data[$key]['description'] 	 	    = $val['description'];
  $data[$key]['borrower_type'] 	    = $_LABELS[sprintf('tblLoanTypes.borrower_type.%s',$val['borrower_type'])];
  $data[$key]['installment'] 	 	    = $val['installment'];
  $data[$key]['payment_frequency']  = $_LABELS[sprintf('tblLoanTypes.payment_frequency.%s',$val['payment_frequency'])];
  $data[$key]['rates_r'] 	 	        = $val['rates_r'];
  $data[$key]['rates_d'] 	 	        = $val['rates_d'];
  $data[$key]['fees_af']            = $val['fees_af'];
  $data[$key]['fees_at']            = $val['fees_at'];
  $data[$key]['margin_d']           = $val['margin_d'];

}

$data = array_reverse($data);

//$extra['description']['header']	      = 'tblLoanTypes.description';
//$extra['description']['pack']	        = '%s';
//$extra['description']['align']	      = 'right';

$extra['borrower_type']['header']		  = 'tblLoanTypes.borrower_type';
$extra['borrower_type']['pack']		    = '%s';
$extra['borrower_type']['align']		  = 'right';

$extra['installment']['header']	      = 'tblLoanTypes.installment';
$extra['installment']['pack']	        = '%s';
$extra['installment']['align']	      = 'right';

$extra['payment_frequency']['header'] = 'tblLoanTypes.payment_frequency';
$extra['payment_frequency']['pack']	  = '%s';
$extra['payment_frequency']['align']  = 'right';

$extra['rates_r']['header']	          = 'tblLoanTypes.rates_r';
$extra['rates_r']['pack']		          = '%s';
$extra['rates_r']['align']	          = 'right';

$extra['rates_d']['header']	          = 'tblLoanTypes.rates_d';
$extra['rates_d']['pack']		          = '%s';
$extra['rates_d']['align']		        = 'right';

$extra['fees_af']['header']	          = 'tblLoanTypes.fees_af';
$extra['fees_af']['pack']	            = '%s';
$extra['fees_af']['align']	          = 'right';

$extra['fees_at']['header']	          = 'tblLoanTypes.fees_at';
$extra['fees_at']['pack']		          = '%s';
$extra['fees_at']['align']	          = 'right';

$extra['margin_d']['header']	        = 'tblLoanTypes.margin_d';
$extra['margin_d']['pack']		        = '%s';
$extra['margin_d']['align']	          = 'right';

$extra['param']['ref']		            = WEBPAGE::$scr_name;

array_unshift($data,$extra);

WEBPAGE::makecachefile($data,WEBPAGE::$queryCache);
require_once '../includes/BS.SCR.pager.inc';

?>