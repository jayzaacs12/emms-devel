<?php
error_reporting(E_ERROR);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'class/graph.php';

WEBPAGE::START();
$_LABELS = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_LABELS_FILE,$_GET['lang']));

//get question list   $ql_a
$sql = sprintf("SELECT question_list FROM tblSurveys WHERE id = '%s'",$_GET[survey_id]);
$row = current(WEBPAGE::$dbh->getAll($sql));
$ql = $row[question_list];
$ql_a = explode(',', $ql);

//get answer num - cal.   $an_a
$sql = sprintf("SELECT id, answer_num, category FROM tblSurveyItems WHERE FIND_IN_SET(id, '%s')",$ql);
$mrow = WEBPAGE::$dbh->getAll($sql);

for ($i=0;$i<count($mrow);$i++) {
  $an_a_cal[$mrow[$i][id]] = explode('|', $mrow[$i][answer_num]);
  $an_a_cat[$mrow[$i][id]] = $mrow[$i][category];
  foreach ($an_a_cal[$mrow[$i][id]] as $key => $value) {
    $an_a_cmax[$mrow[$i][id]] = MAX($an_a_cmax[$mrow[$i][id]],$value);
    }
  }

// get survey answers $sa
$sql = "SELECT client_id, answer_list FROM tblSurveyAnswers WHERE survey_id = '$_GET[survey_id]'";
$mrow = WEBPAGE::$dbh->getAll($sql);
for ($i=0;$i<count($mrow);$i++) {
  $sa_a_ref = explode(',',$mrow[$i][answer_list]);
  unset ($sa_a_cal);
  foreach ($sa_a_ref as $key => $value) {
    $sa_a_cal[] = $an_a_cal[$ql_a[$key]][$value-1];
    }
  $sa_a[$mrow[$i][client_id]][] = $sa_a_cal;
  }

// ready to graph
foreach($sa_a as $key => $value) {
  foreach ($value as $ki => $vi) {
    foreach ($vi as $kj => $vj) {
      $dat[$an_a_cat[$ql_a[$kj]]][$ki][$kj][] = $sa_a[$key][$ki][$kj]; //this is a nice subproduct
      $cal_graph[$an_a_cat[$ql_a[$kj]]][$ki] += $sa_a[$key][$ki][$kj];
      $max_graph[$an_a_cat[$ql_a[$kj]]][$ki] += $an_a_cmax[$ql_a[$kj]];
//      if ($an_a_cat[$ql_a[$kj]]) { $graph[$_LABELS[sprintf('tblSurveyItems.category.%s',$an_a_cat[$ql_a[$kj]])]][$ki] = 100*($cal_graph[$an_a_cat[$ql_a[$kj]]][$ki]/$max_graph[$an_a_cat[$ql_a[$kj]]][$ki]); }
//      change line above for line below... change $ki into $ki+1... this way first loan cycle displays 1 instead of 0...
      if ($an_a_cat[$ql_a[$kj]]) { $c[$ki]++; $graph[$_LABELS[sprintf('tblSurveyItems.category.%s',$an_a_cat[$ql_a[$kj]])]][$ki] = 100*($cal_graph[$an_a_cat[$ql_a[$kj]]][$ki]/$max_graph[$an_a_cat[$ql_a[$kj]]][$ki]); }
//		printf('[%s][%s] -> %s/%s<br>',$an_a_cat[$ql_a[$kj]],$ki,100*($cal_graph[$an_a_cat[$ql_a[$kj]]][$ki]),$max_graph[$an_a_cat[$ql_a[$kj]]][$ki]);
      }
    }
  }

// get rid off last cycles with less than 5% answers than the first cycle
foreach($graph as $cat => $val) {
  foreach($val as $cycle => $score) {
    if (($c[$cycle]/$c[0]) > 0.05 ) { $fgraph[$cat][$cycle + 1] = $score; }
    }
  }

$grafico = New Graph('','Loan cycle','%',$fgraph);
Graph::printBends($grafico);
?>