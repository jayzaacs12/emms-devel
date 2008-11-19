<?
//$tpl->setVariable('graph_title', $_LABELS['RP.GRP.attrition']);
//$tpl->setVariable('graph', sprintf("<img src='RP.GRP.attrition.php?lang=%s' alt=''>",WEBPAGE::$lang));

$tpl->setVariable('cd_graph_title', $_LABELS['RP.GRP.clientDistribution']);
$tpl->setVariable('cd_graph', sprintf("<img src='RP.GRP.clientDistribution.php?lang=%s' alt=''>",WEBPAGE::$lang));

$tpl->setVariable('gr_graph_title', $_LABELS['RP.GRP.genderRatio']);
$tpl->setVariable('gr_graph', sprintf("<img src='RP.GRP.genderRatio.php?lang=%s' alt=''>",WEBPAGE::$lang));
?>
