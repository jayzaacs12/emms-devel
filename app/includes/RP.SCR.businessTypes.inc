<?
$tpl->setVariable('ba_graph_title', $_LABELS['RP.GRP.businessActivities']);
$tpl->setVariable('ba_graph', sprintf("<img src='RP.GRP.businessActivities.php?lang=%s' alt=''>",WEBPAGE::$lang));
$tpl->setVariable('bt_graph_title', $_LABELS['RP.GRP.top5business']);
$tpl->setVariable('bt_graph', sprintf("<img src='RP.GRP.top5business.php' alt=''>"));
?>
