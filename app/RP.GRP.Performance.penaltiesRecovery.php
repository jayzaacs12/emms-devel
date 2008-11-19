<?php
// loan portfolio graph
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
    $mrow   = WEBPAGE::$dbh->getAll(sprintf('select 1000*YEAR(p.date)+DAYOFYEAR(p.date) as date, sum(p.penalties) as penalties, lt.borrower_type from tblPayments as p, tblLoans as l, tblLoanTypes as lt where lt.id = l.loan_type_id and l.id = p.loan_id and 1000*YEAR(p.date)+DAYOFYEAR(p.date) >= "%s" and 1000*YEAR(p.date)+DAYOFYEAR(p.date) <= "%s" and %s = %s group by date, lt.borrower_type',$xrange[0]['date'],$xrange[count($xrange)-1]['date'],$_GET['mode'],$_GET[$_GET['mode']]));
    $xmin   = $_GET['dateFrom'];
    $xmax   = $_GET['dateTo'];
    break; 
  case 'WEEK'	: 
    $xrange = WEBPAGE::$dbh->getAll(sprintf('select 1000*YEAR(c.date)+WEEK(c.date,"%s") as date from tblCalendar as c where c.date >= "%s" and c.date <= "%s" group by date',$_GET['dateFrom'],$_GET['dateFrom'],$_GET['dateTo']));  
    $mrow   = WEBPAGE::$dbh->getAll(sprintf('select 1000*YEAR(p.date)+WEEK(p.date,"%s") as date, sum(p.penalties) as penalties, lt.borrower_type from tblPayments as p, tblLoans as l, tblLoanTypes as lt where lt.id = l.loan_type_id and l.id = p.loan_id and 1000*YEAR(p.date)+WEEK(p.date,"%s") >= "%s" and 1000*YEAR(p.date)+WEEK(p.date,"%s") <= "%s" and %s = %s group by date, lt.borrower_type',$_GET['dateFrom'],$_GET['dateFrom'],$xrange[0]['date'],$_GET['dateFrom'],$xrange[count($xrange)-1]['date'],$_GET['mode'],$_GET[$_GET['mode']]));
    $xmin   = sprintf('%s/%s',$xrange[0]['date']-1000*floor($xrange[0]['date']/1000),floor($xrange[0]['date']/1000));
    $xmax   = sprintf('%s/%s',$xrange[count($xrange)-1]['date']-1000*floor($xrange[count($xrange)-1]['date']/1000),floor($xrange[count($xrange)-1]['date']/1000));
    break; 
  case 'MONTH'	: 
    $xrange = WEBPAGE::$dbh->getAll(sprintf('select 1000*YEAR(c.date)+MONTH(c.date) as date from tblCalendar as c where c.date >= "%s" and c.date <= "%s" group by date',$_GET['dateFrom'],$_GET['dateTo']));  
    $mrow   = WEBPAGE::$dbh->getAll(sprintf('select 1000*YEAR(p.date)+MONTH(p.date) as date, sum(p.penalties) as penalties, lt.borrower_type from tblPayments as p, tblLoans as l, tblLoanTypes as lt where lt.id = l.loan_type_id and l.id = p.loan_id and 1000*YEAR(p.date)+MONTH(p.date) >= "%s" and 1000*YEAR(p.date)+MONTH(p.date) <= "%s" and %s = %s group by date, lt.borrower_type',$xrange[0]['date'],$xrange[count($xrange)-1]['date'],$_GET['mode'],$_GET[$_GET['mode']]));
    $xmin   = sprintf('%s/%s',$xrange[0]['date']-1000*floor($xrange[0]['date']/1000),floor($xrange[0]['date']/1000));
    $xmax   = sprintf('%s/%s',$xrange[count($xrange)-1]['date']-1000*floor($xrange[count($xrange)-1]['date']/1000),floor($xrange[count($xrange)-1]['date']/1000));
    break; 
  case 'YEAR'	: 
    $xrange = WEBPAGE::$dbh->getAll(sprintf('select YEAR(c.date) as date from tblCalendar as c where c.date >= "%s" and c.date <= "%s" group by date',$_GET['dateFrom'],$_GET['dateTo']));  
    $mrow   = WEBPAGE::$dbh->getAll(sprintf('select YEAR(p.date) as date, sum(p.penalties) as penalties, lt.borrower_type from tblPayments as p, tblLoans as l, tblLoanTypes as lt where lt.id = l.loan_type_id and l.id = p.loan_id and YEAR(p.date) >= "%s" and YEAR(p.date) <= "%s" and %s = %s group by date, lt.borrower_type',$xrange[0]['date'],$xrange[count($xrange)-1]['date'],$_GET['mode'],$_GET[$_GET['mode']]));
    $xmin   = $xrange[0]['date'];
    $xmax   = $xrange[count($xrange)-1]['date'];
     break; 
  }

if ((!count($mrow))||(count($xrange)<2)) { 
  $graph = New Graph('',$_LABELS[$_GET['cycle']],sprintf('%s (%s)',$_LABELS['loanPortfolio'],'%'),array(),array());		
  Graph::printEmpty($graph);
  exit;
  }

// initialize data array()
$data[$_LABELS['total']] = array();
foreach ($mrow as $key=>$val) {
  foreach($xrange as $i=>$j) {
    $data[$_LABELS[sprintf('tblLoanTypes.borrower_type.%s',$val['borrower_type'])]][$j['date']] = 0;
    $data[$_LABELS['total']][$j['date']] = 0;
    }
  }

// populate data array() with actual values from sql
foreach ($mrow as $key=>$val) {
  $data[$_LABELS[sprintf('tblLoanTypes.borrower_type.%s',$val['borrower_type'])]][$val['date']] = $val['penalties'];
  $data[$_LABELS['total']][$val['date']] += $val['penalties'];
  }

foreach ($data as $key=>$val) {
  $c = 1;  
  foreach($val as $i=>$j) {
    $dataf[$key][$c++] = $j;   
    }
  }
//print('<pre>');print_r($dataf);print('</pre>'); exit;

$options['x-labels']['on'] = 0;
$options['x-labels']['alt'] = sprintf($_LABELS['from_x_to_y'],$xmin,$xmax);
$graph = New Graph('',$_LABELS[$_GET['cycle']],sprintf('%s (%s)',$_LABELS['RP.GRP.Performance.penaltiesRecovery'],'$'),$dataf,$options);		
Graph::printLines($graph);

?>

