<?php
error_reporting(E_ERROR);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'class/graph.php';

WEBPAGE::START();
$_LABELS = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_LABELS_FILE,$_GET['lang']));

$data[0] = WEBPAGE::$dbh->getAssoc('select zp.short_name, count(c.id) from tblClients as c, tblZones as z, tblZones as zp 
    							where c.zone_id > 0 and z.id = c.zone_id and zp.id = z.parent_id group by zp.id');

$graph = New Graph('',$_LABELS['tblUsers.zone_id'],$_LABELS['active_associates'],$data);		
Graph::printBars($graph); 
?>
