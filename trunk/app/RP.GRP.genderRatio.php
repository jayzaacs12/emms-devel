<?php
error_reporting(E_ERROR);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'class/graph.php';

WEBPAGE::START();
$_LABELS = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_LABELS_FILE,$_GET['lang']));

$mrow = WEBPAGE::$dbh->getAll('select c.gender, zp.short_name as branch, count(c.id) as hits from tblClients as c, tblZones as z, tblZones as zp 
    							where c.zone_id > 0 and z.id = c.zone_id and zp.id = z.parent_id group by c.gender, zp.id');

foreach($mrow as $key=>$val) {
  $data[$_LABELS[sprintf('tblClients.gender.%s',$val['gender'])]]['TOTAL'] += $val['hits'];	
  $data[$_LABELS[sprintf('tblClients.gender.%s',$val['gender'])]][$val['branch']] = $val['hits'];	
  }

$graph = New Graph('',$_LABELS['tblUsers.zone_id'],'%',$data);		
Graph::printStdCols($graph);
?>
