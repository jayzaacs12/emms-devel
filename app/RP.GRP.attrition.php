<?php
error_reporting(E_ERROR);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'class/graph.php';

WEBPAGE::START();
$_LABELS = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_LABELS_FILE,$_GET['lang']));

$mrow1 = WEBPAGE::$dbh->getAll("
				SELECT iom.cause, COUNT( iom.cause ) AS hits, pz.short_name as zone_id
				FROM tblClientIOM as iom, tblZones AS z, tblZones AS pz
				WHERE iom.type = 'O' AND z.id = iom.zone_id AND pz.id = z.parent_id
				GROUP BY iom.cause, z.parent_id"); 
$mrow1 = is_array($mrow1) ? $mrow1 : array();

$zones[TOTAL] = '';
foreach($mrow1 as $i => $row) {
  $data1[$row[cause]][$row[zone_id]] = $row[hits];
  $data1[$row[cause]][TOTAL] += $row[hits];
  $zones[$row[zone_id]] = '';
  $causes[$row[cause]] = '';
  }

foreach($zones as $i => $val_i) {
  foreach($causes as $j => $val_j) {
    $data2[$_LABELS[sprintf("tblClientIOM.cause.%s",$j)]][$i] = $data1[$j][$i] ? $data1[$j][$i] : 0;
 //   $data2[$j][$i] = $data1[$j][$i] ? $data1[$j][$i] : 0;
    }
  }

$grafico = New Graph('','','%',$data2);			
Graph::printStdCols($grafico);
exit;
?>
