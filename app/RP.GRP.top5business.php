<?php
error_reporting(E_ERROR);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'class/graph.php';

WEBPAGE::START();

$top = 5;

$mrow1 = WEBPAGE::$dbh->getAll("SELECT bt.type, count( b.id ) AS hits, pz.short_name AS zone
								FROM tblBusiness AS b, tblBusinessTypes AS bt, tblZones AS z, tblClients AS c, tblZones AS pz
								WHERE bt.id = b.type_id
								AND FIND_IN_SET( c.id, b.client_list )
								AND z.id = c.zone_id
								AND pz.id = z.parent_id
								GROUP BY bt.id, pz.id"); 
$mrow1 = is_array($mrow1) ? $mrow1 : array();

$data1['TOTAL'] = array();
foreach($mrow1 as $i => $row) {
  $data1[$row['zone']][$row['type']] = $row['hits'];
  $data1['TOTAL'][$row['type']] += $row['hits'];
  }
$types['OTROS'] = '';  
foreach($data1 as $z => $row) {
  arsort($row);
  $c = $top;
  foreach($row as $t => $h) {
    if ($c > 0) {
	  $data2[$z][$t] = $h; 
	  $zones[$z] = ''; 
	  $types[$t] = '';
	  } else {
	  $data2[$z]['OTROS'] += $h;
	  }
    $c--;
    }
  }

foreach($zones as $i => $val_i) {
  foreach($types as $j => $val_j) {
    $data3[$j][$i] = $data2[$i][$j] ? $data2[$i][$j] : 0;
    }
  }
$grafico = New Graph('','','%',$data3);			
Graph::printStdCols($grafico);
exit;
