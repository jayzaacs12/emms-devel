<?php
error_reporting(E_ERROR);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'class/graph.php';
require_once 'class/survey.php';

WEBPAGE::START();

//require_once 'class/client.php';
//$survey = new SURVEY($_GET[survey_id]);

$data = SURVEY::getGraphData($_GET['survey_id'],$_GET['client_id'],$_GET['lang']);
$grafico = New Graph('','','%',$data);
switch (count($data)) {
  case 0:
    break;
  case 1:
    Graph::printBars($grafico); 
    break;
  default:
    Graph::printBends($grafico); 
  }	
?>