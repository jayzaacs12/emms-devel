<?php
// client portfolio graph
// called by RP.SCR.performancePlotter
error_reporting(E_ERROR);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'class/graph.php';

WEBPAGE::START();
$_LABELS = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_LABELS_FILE,$_GET['lang']));
$_CONF   = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_CONF_FILE,$_GET['lang']));

if ($_GET['mode'] == 'total') { $_GET['mode'] = 1; $_GET['1'] = 1; }

switch ($_GET['cycle']) {
  case 'DAY'	: 
    $xrange = WEBPAGE::$dbh->getAll(sprintf('select 1000*YEAR(c.date)+DAYOFYEAR(c.date) as date from tblCalendar as c where c.date >= "%s" and c.date <= "%s" group by date',$_GET['dateFrom'],$_GET['dateTo']));  
    $mrow   = WEBPAGE::$dbh->getAll(sprintf('select 1000*YEAR(cp.date)+DAYOFYEAR(cp.date) as date, sum(cp.clients) as clients, sum(cp.female) as female, sum(cp.male) as male, sum(cp.client_i) as client_i, sum(cp.client_b) as client_b from tblClientPortfolio as cp where 1000*YEAR(cp.date)+DAYOFYEAR(cp.date) >= "%s" and 1000*YEAR(cp.date)+DAYOFYEAR(cp.date) <= "%s" and %s = %s group by date',$xrange[0]['date'],$xrange[count($xrange)-1]['date'],$_GET['mode'],$_GET[$_GET['mode']]));
    $xmin   = $_GET['dateFrom'];
    $xmax   = $_GET['dateTo'];
    break; 
  case 'WEEK'	: 
    $xrange = WEBPAGE::$dbh->getAll(sprintf('select 1000*YEAR(c.date)+WEEK(c.date,"%s") as date from tblCalendar as c where c.date >= "%s" and c.date <= "%s" group by date',$_GET['dateFrom'],$_GET['dateFrom'],$_GET['dateTo']));  
    $mrow   = WEBPAGE::$dbh->getAll(sprintf('select 1000*YEAR(cp.date)+WEEK(cp.date,"%s") as date, sum(cp.clients)/7 as clients, sum(cp.female)/7 as female, sum(cp.male)/7 as male, sum(cp.client_i)/7 as client_i, sum(cp.client_b)/7 as client_b from tblClientPortfolio as cp where 1000*YEAR(cp.date)+WEEK(cp.date,"%s") >= "%s" and 1000*YEAR(cp.date)+WEEK(cp.date,"%s") <= "%s" and %s = %s group by date',$_GET['dateFrom'],$_GET['dateFrom'],$xrange[0]['date'],$_GET['dateFrom'],$xrange[count($xrange)-1]['date'],$_GET['mode'],$_GET[$_GET['mode']]));
    $xmin   = sprintf('%s/%s',$xrange[0]['date']-1000*floor($xrange[0]['date']/1000),floor($xrange[0]['date']/1000));
    $xmax   = sprintf('%s/%s',$xrange[count($xrange)-1]['date']-1000*floor($xrange[count($xrange)-1]['date']/1000),floor($xrange[count($xrange)-1]['date']/1000));
    break; 
  case 'MONTH'	: 
    $xrange = WEBPAGE::$dbh->getAll(sprintf('select 1000*YEAR(c.date)+MONTH(c.date) as date from tblCalendar as c where c.date >= "%s" and c.date <= "%s" group by date',$_GET['dateFrom'],$_GET['dateTo']));  
    $mrow   = WEBPAGE::$dbh->getAll(sprintf('select 1000*YEAR(cp.date)+MONTH(cp.date) as date, sum(cp.clients)/30 as clients, sum(cp.female)/30 as female, sum(cp.male)/30 as male, sum(cp.client_i)/30 as client_i, sum(cp.client_b)/30 as client_b from tblClientPortfolio as cp where 1000*YEAR(cp.date)+MONTH(cp.date) >= "%s" and 1000*YEAR(cp.date)+MONTH(cp.date) <= "%s" and %s = %s group by date',$xrange[0]['date'],$xrange[count($xrange)-1]['date'],$_GET['mode'],$_GET[$_GET['mode']]));
    $xmin   = sprintf('%s/%s',$xrange[0]['date']-1000*floor($xrange[0]['date']/1000),floor($xrange[0]['date']/1000));
    $xmax   = sprintf('%s/%s',$xrange[count($xrange)-1]['date']-1000*floor($xrange[count($xrange)-1]['date']/1000),floor($xrange[count($xrange)-1]['date']/1000));
    break; 
  case 'YEAR'	: 
    $xrange = WEBPAGE::$dbh->getAll(sprintf('select YEAR(c.date) as date from tblCalendar as c where c.date >= "%s" and c.date <= "%s" group by date',$_GET['dateFrom'],$_GET['dateTo']));  
    $mrow   = WEBPAGE::$dbh->getAll(sprintf('select YEAR(cp.date) as date, sum(cp.clients)/365 as clients, sum(cp.female)/365 as female, sum(cp.male)/365 as male, sum(cp.client_i)/365 as client_i, sum(cp.client_b)/365 as client_b from tblClientPortfolio as cp where YEAR(cp.date) >= "%s" and YEAR(cp.date) <= "%s" and %s = %s group by date',$xrange[0]['date'],$xrange[count($xrange)-1]['date'],$_GET['mode'],$_GET[$_GET['mode']]));
    $xmin   = $xrange[0]['date'];
    $xmax   = $xrange[count($xrange)-1]['date'];
     break; 
  }

if ((!count($mrow))||(count($xrange)<2)) { 
  $graph = New Graph('',$_LABELS[$_GET['cycle']],$_LABELS['RP.GRP.Performance.clientPortfolio'],array(),array());		
  Graph::printEmpty($graph);
  exit;
  }

// initialize data array()
$data[$_LABELS['total']] = array();
foreach($xrange as $i=>$j) {
  $data[$_LABELS['total']][$j['date']] 							= 0;
  $data[$_LABELS['tblClients.gender.F']][$j['date']] 			= 0;
  $data[$_LABELS['tblClients.gender.M']][$j['date']]		 	= 0;
  $data[$_LABELS['tblLoanTypes.borrower_type.B']][$j['date']] 	= 0;
  $data[$_LABELS['tblLoanTypes.borrower_type.I']][$j['date']] 	= 0;
  }

// populate data array() with actual values from sql
foreach ($mrow as $key=>$val) {
  $data[$_LABELS['total']][$val['date']] 						= $val['clients'];
  $data[$_LABELS['tblClients.gender.M']][$val['date']] 			= $val['male'];
  $data[$_LABELS['tblClients.gender.F']][$val['date']] 			= $val['female'];
  $data[$_LABELS['tblLoanTypes.borrower_type.I']][$val['date']] = $val['client_i'];
  $data[$_LABELS['tblLoanTypes.borrower_type.B']][$val['date']] = $val['client_b'];
  }

foreach ($data as $key=>$val) {
  $c = 1;  
  foreach($val as $i=>$j) {
    $dataf[$key][$c++] = $j;   
    }
  }
//print('<pre>');print_r($dataf);print('</pre>'); exit;

$options['y-round']		 	= 0;
$options['x-labels']['on'] 	= 0;
$options['x-labels']['alt'] = sprintf($_LABELS['from_x_to_y'],$xmin,$xmax);
$graph = New Graph('',$_LABELS[$_GET['cycle']],$_LABELS['RP.GRP.Performance.clientPortfolio'],$dataf,$options);		
Graph::printLines($graph);

?>


