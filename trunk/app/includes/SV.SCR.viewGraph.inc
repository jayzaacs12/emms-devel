<?
require_once 'class/survey.php';
require_once 'class/client.php';
$survey = new SURVEY($_GET['survey_id']);
$client = new CLIENT($_GET['client_id']);
$tpl->setVariable('client', $client->data['name']);
$tpl->setVariable('href_client', sprintf("index.php?scr_name=BS.SCR.viewClient&id=%s",$client->data['id']));
$tpl->setVariable('client_code', $client->data['code']);
$tpl->setVariable('client_label', $_LABELS['tblClients.id']);
$tpl->setVariable('graph_title', $survey->data['name']);
$tpl->setVariable('graph', sprintf("<img src='SV.SCR.graphDrawer.php?survey_id=%s&client_id=%s&lang=%s'>",$_GET['survey_id'],$_GET['client_id'],WEBPAGE::$lang));
$tpl->setVariable('c_graph_title', $_LABELS['RP.GRP.programImpact']);
$tpl->setVariable('c_graph', sprintf("<img src='RP.GRP.programImpact.php?survey_id=%s&lang=%s'>",$_GET['survey_id'],WEBPAGE::$lang));
?>