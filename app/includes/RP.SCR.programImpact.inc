<?
$tpl->setVariable('graph_title', $_LABELS['RP.GRP.programImpact']);
$tpl->setVariable('graph', sprintf("<img src='RP.GRP.programImpact.php?survey_id=%s&lang=%s' alt=''>",$_CONF['survey_id'],WEBPAGE::$lang));
?>
