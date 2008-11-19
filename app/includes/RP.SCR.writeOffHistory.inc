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
				select l.client_id, concat(c.last,', ',c.first) as client, c.code as ccode, count(lwo.id) as hits, sum(lwo.amount) as amount, u.username as advisor, p.program as program, z.zone as branch
				from tblLoanWriteOff as lwo, tblLoans as l, tblClients as c, tblUsers as u, tblPrograms as p, tblZones as z, tblLoanTypes as lt
				where lwo.date >= '%s' and lwo.date <= '%s' and lt.id = l.loan_type_id and lt.borrower_type != 'G' and l.id = lwo.loan_id and c.id = l.client_id and u.id = l.advisor_id and p.id = l.program_id and z.id = l.zone_id group by l.client_id, u.id, p.id, z.id order by l.client_id", $date_from, $date_to));

foreach($data as $key=>$val) {
	
  $client->data['id'] = $val['client_id'];
  $client->checkConfidentiality();
  if ($client->data['confidential']) {
    $data[$key]['client'] = $client->data['name'];
    $data[$key]['ccode']  = $client->data['code'];
    }	
    
  $ldata[$key]['client'] 	 	 = str_replace(' ','&nbsp;',$data[$key]['client']);
  $ldata[$key]['ccode'] 	 	 = str_replace(' ','&nbsp;',$data[$key]['ccode']);  
  $ldata[$key]['hits'] 			 = $val['hits'];    
  $ldata[$key]['amount'] 		 = $val['amount'];      
  $ldata[$key]['advisor'] 		 = $val['advisor'];    
  $ldata[$key]['program'] 		 = $val['program'];    
  $ldata[$key]['branch'] 		 = $val['branch'];    

  $total['client'] 	 			 = $_LABELS['total'];
  $total['ccode'] 	 			 = ''; 
  $total['hits'] 				+= $val['hits'];    
  $total['amount'] 				+= $val['amount'];   
  $total['advisor'] 	 		 = ''; 
  $total['program'] 			 = '';    
  $total['branch'] 				 = '';   
     
  }

if (count($ldata)) { $ldata['total'] = $total; }

$head = array('client'=>$_LABELS['tblClients.id'],'ccode'=>$_LABELS['tblClients.code'],'hits'=>sprintf('%s*',$_LABELS['loans']),'amount'=>sprintf('%s*',$_LABELS['writeOff_amount']),'advisor'=>$_LABELS['tblClients.advisor_id'],'program'=>$_LABELS['tblPrograms.program'],'branch'=>$_LABELS['tblZones.zone']);
$tpl->setVariable('rpt_label', 			$_LABELS['writeOffHistory']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.writeOffHistory']);
$tpl->setVariable('date_range_label', 	$_LABELS['dateRange']);
$tpl->setVariable('date_range', 		sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($ldata) ? WEBPAGE::printchart($ldata,$head) : $_LABELS['noData']);

$tpl->setVariable('writeOffInPeriod', 	$_LABELS['writeOffInPeriod']);

?>

