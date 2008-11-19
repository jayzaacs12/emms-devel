<?php
$date_from 	= $_REQUEST['date_from'] 	? $_REQUEST['date_from'] 	: date('Y-m-d');
$date_to 	= $_REQUEST['date_to'] 		? $_REQUEST['date_to'] 		: date('Y-m-d');
$tpl->setVariable('dateRangeSelector', 	$_LABELS['BS.SCR.dateRangeSelector']);
$tpl->setVariable('oldrange', 			sprintf('%s - %s',$date_from,$date_to));

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select z.parent_id as zone_id, zz.zone, c.gender,
		 		count(iom.id) as hits, iom.cause, iom.type
		 		from tblClientIOM as iom, tblZones as z, tblZones as zz, tblClients as c where c.id = iom.client_id and zz.id = z.parent_id and z.id = iom.zone_id and iom.internal = '0' and iom.date >= '%s' and iom.date <= '%s' group by zone_id, gender, cause, type", $date_from, $date_to));

foreach($data as $key=>$val) {
	
  $ldata[$val['zone_id']]['zone'] 	 = str_replace(' ','&nbsp;',$val['zone']);
  
  $ldata[$val['zone_id']]['IN_F'] 		+= (($val['type'] == 'I')&&($val['gender'] == 'F')) ? $val['hits'] : 0;
  $ldata[$val['zone_id']]['IN_M'] 		+= (($val['type'] == 'I')&&($val['gender'] == 'M')) ? $val['hits'] : 0;
  
  $ldata[$val['zone_id']]['OUT_F'] 		+= (($val['type'] == 'O')&&($val['gender'] == 'F')) ? $val['hits'] : 0;
  $ldata[$val['zone_id']]['OUT_M'] 		+= (($val['type'] == 'O')&&($val['gender'] == 'M')) ? $val['hits'] : 0;
    
  $ldata[$val['zone_id']]['C_A'] 		+= (($val['type'] == 'O')&&($val['cause'] == 'A'))  ? $val['hits'] : 0;
  $ldata[$val['zone_id']]['C_B'] 		+= (($val['type'] == 'O')&&($val['cause'] == 'B'))  ? $val['hits'] : 0;
  $ldata[$val['zone_id']]['C_C'] 		+= (($val['type'] == 'O')&&($val['cause'] == 'C'))  ? $val['hits'] : 0;
  $ldata[$val['zone_id']]['C_D'] 		+= (($val['type'] == 'O')&&($val['cause'] == 'D'))  ? $val['hits'] : 0;
  $ldata[$val['zone_id']]['C_E'] 		+= (($val['type'] == 'O')&&($val['cause'] == 'E'))  ? $val['hits'] : 0;
  $ldata[$val['zone_id']]['C_F'] 		+= (($val['type'] == 'O')&&($val['cause'] == 'F'))  ? $val['hits'] : 0;
  $ldata[$val['zone_id']]['C_G'] 		+= (($val['type'] == 'O')&&($val['cause'] == 'G'))  ? $val['hits'] : 0;
  $ldata[$val['zone_id']]['C_H'] 		+= (($val['type'] == 'O')&&($val['cause'] == 'H'))  ? $val['hits'] : 0;
  $ldata[$val['zone_id']]['C_I'] 		+= (($val['type'] == 'O')&&($val['cause'] == 'I'))  ? $val['hits'] : 0;
  $ldata[$val['zone_id']]['C_J'] 		+= (($val['type'] == 'O')&&($val['cause'] == 'J'))  ? $val['hits'] : 0;
    
  $total['zone'] 	 					 = $_LABELS['total'];
  
  $total['IN_F'] 						+= (($val['type'] == 'I')&&($val['gender'] == 'F')) ? $val['hits'] : 0;
  $total['IN_M'] 						+= (($val['type'] == 'I')&&($val['gender'] == 'M')) ? $val['hits'] : 0;
  
  $total['OUT_F'] 						+= (($val['type'] == 'O')&&($val['gender'] == 'F')) ? $val['hits'] : 0;
  $total['OUT_M'] 						+= (($val['type'] == 'O')&&($val['gender'] == 'M')) ? $val['hits'] : 0;
    
  $total['C_A'] 						+= (($val['type'] == 'O')&&($val['cause'] == 'A'))  ? $val['hits'] : 0;
  $total['C_B'] 						+= (($val['type'] == 'O')&&($val['cause'] == 'B'))  ? $val['hits'] : 0;
  $total['C_C'] 						+= (($val['type'] == 'O')&&($val['cause'] == 'C'))  ? $val['hits'] : 0;
  $total['C_D'] 						+= (($val['type'] == 'O')&&($val['cause'] == 'D'))  ? $val['hits'] : 0;
  $total['C_E'] 						+= (($val['type'] == 'O')&&($val['cause'] == 'E'))  ? $val['hits'] : 0;
  $total['C_F'] 						+= (($val['type'] == 'O')&&($val['cause'] == 'F'))  ? $val['hits'] : 0;
  $total['C_G'] 						+= (($val['type'] == 'O')&&($val['cause'] == 'G'))  ? $val['hits'] : 0;
  $total['C_H'] 						+= (($val['type'] == 'O')&&($val['cause'] == 'H'))  ? $val['hits'] : 0;
  $total['C_I'] 						+= (($val['type'] == 'O')&&($val['cause'] == 'I'))  ? $val['hits'] : 0;
  $total['C_J'] 						+= (($val['type'] == 'O')&&($val['cause'] == 'J'))  ? $val['hits'] : 0;
    
  }

if (count($ldata)) { $ldata['total'] = $total; }

$head = array('zone'=>$_LABELS['tblZones.zone'],'IN_F'=>sprintf('IN-F*'),'IN_M'=>sprintf('IN-M*'),'OUT_F'=>sprintf('OUT-F*'),'OUT_M'=>sprintf('OUT-M*'),'A'=>sprintf('C-A**'),'B'=>sprintf('C-B**'),'C-C'=>sprintf('C-C**'),'C-D'=>sprintf('C-D**'),'C-E'=>sprintf('C-E**'),'C-F'=>sprintf('C-F**'),'C-G'=>sprintf('C-G**'),'C-H'=>sprintf('C-H**'),'C-I'=>sprintf('C-I**'),'C-J'=>sprintf('C-J**'));
$tpl->setVariable('rpt_label', 			$_LABELS['clientIOM']);
$tpl->setVariable('rpt_subtitle_label', $_LABELS['RP.SCR.clientIOMByBranch']);
$tpl->setVariable('date_range_label', 	$_LABELS['dateRange']);
$tpl->setVariable('date_range', 		sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($ldata) ? WEBPAGE::printchart($ldata,$head) : $_LABELS['noData']);

$tpl->setVariable('iomInPeriod', 	$_LABELS['iomInPeriod']);
$tpl->setVariable('outCauses', 		$_LABELS['outCauses']);
$tpl->setVariable('legend', 		sprintf('<b>%s:</b><br><b>IN</b> - %s, <b>OUT</b> - %s, <b>F</b> - %s, <b>M</b> - %s<br><b>C-A</b> - %s, <b>C-B</b> - %s, <b>C-C</b> - %s, <b>C-D</b> - %s, <b>C-E</b> - %s<br> <b>C-F</b> - %s, <b>C-G</b> - %s, <b>C-H</b> - %s, <b>C-I</b> - %s, <b>C-J</b> - %s',$_LABELS['legend'],$_LABELS['IOM_IN'],$_LABELS['IOM_OUT'],$_LABELS['tblClients.gender.F'],$_LABELS['tblClients.gender.M'],$_LABELS['tblClientIOM.cause.A'],$_LABELS['tblClientIOM.cause.B'],$_LABELS['tblClientIOM.cause.C'],$_LABELS['tblClientIOM.cause.D'],$_LABELS['tblClientIOM.cause.E'],$_LABELS['tblClientIOM.cause.F'],$_LABELS['tblClientIOM.cause.G'],$_LABELS['tblClientIOM.cause.H'],$_LABELS['tblClientIOM.cause.I'],$_LABELS['tblClientIOM.cause.J']));

?>

