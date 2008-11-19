<?php
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';

$client = new CLIENT();
//extract(SOCIETY::getTemplateData($_REQUEST['id']));

$date_from 		= $_REQUEST['date_from'] 	? $_REQUEST['date_from'] 	: date('Y-m-d');
$date_to 		= $_REQUEST['date_to'] 		? $_REQUEST['date_to'] 		: date('Y-m-d');

$tpl->setVariable('dateRangeSelector', 	$_LABELS['BS.SCR.dateRangeSelector']);
$tpl->setVariable('oldrange', 			sprintf('%s - %s',$date_from,$date_to));

$data = WEBPAGE::$dbh->getAll(sprintf("
				select lwo.loan_id, lwo.amount as amount, lwo.date, l.client_id, concat(c.last,', ',c.first) as client, c.code as ccode, l.loan_code as loan, u.username as advisor, p.program as program, z.zone as branch
				from tblLoanWriteOff as lwo, tblLoans as l, tblClients as c, tblUsers as u, tblPrograms as p, tblZones as z, tblLoanTypes as lt
				where lwo.date >= '%s' and lwo.date <= '%s' and lt.id = l.loan_type_id and lt.borrower_type != 'G' and l.id = lwo.loan_id and l.status = 'LI' and c.id = l.client_id and u.id = l.advisor_id and p.id = l.program_id and z.id = l.zone_id order by l.client_id", $date_from, $date_to));

foreach($data as $key=>$val) {
	
  $client->data['id'] = $val['client_id'];
  $client->checkConfidentiality();
  if ($client->data['confidential']) {
    $data[$key]['client'] = $client->data['name'];
    $data[$key]['ccode']  = $client->data['code'];
    }	
    
  $ldata[$key]['client'] 	 	 = str_replace(' ','&nbsp;',$data[$key]['client']);
  $ldata[$key]['ccode'] 	 	 = str_replace(' ','&nbsp;',$data[$key]['ccode']);  
  $ldata[$key]['loan'] 		 	 = $val['loan'];    
  $ldata[$key]['date'] 		 	 = $val['date'];    
  $ldata[$key]['amount'] 		 = $val['amount'];      
  $ldata[$key]['advisor'] 		 = $val['advisor'];    
  $ldata[$key]['program'] 		 = $val['program'];    
  $ldata[$key]['branch'] 		 = $val['branch'];    

  $total['client'] 	 			 = $_LABELS['total'];
  $total['ccode'] 	 			 = ''; 
  $total['loan'] 				 = '';    
  $total['date'] 				 = '';    
  $total['amount'] 				+= $val['amount'];   
  $total['advisor'] 	 		 = ''; 
  $total['program'] 			 = '';    
  $total['branch'] 				 = '';   
     
  }

if (count($ldata)) { $ldata['total'] = $total; }

$head = array('client'=>$_LABELS['tblClients.id'],'ccode'=>$_LABELS['tblClients.code'],'loan'=>$_LABELS['tblLoans.loan_code'],'date'=>$_LABELS['date'],'amount'=>$_LABELS['writeOff_amount'],'advisor'=>$_LABELS['tblClients.advisor_id'],'program'=>$_LABELS['tblPrograms.program'],'branch'=>$_LABELS['tblZones.zone']);
$tpl->setVariable('rpt_label', 			$_LABELS['writeOffPortfolio']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.writeOffPortfolio']);
$tpl->setVariable('date_range_label', 	$_LABELS['dateRange']);
$tpl->setVariable('date_range', 		sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($ldata) ? WEBPAGE::printchart($ldata,$head) : $_LABELS['noData']);

?>


