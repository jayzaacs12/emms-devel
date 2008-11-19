<?
$tpl->addBlockfile('content','content',sprintf("%s.%s",'BS.SCR.browseClients','tpl'));
$tpl->setCurrentBlock("content") ;
$applySurvey = true;
include 'BS.SCR.browseClients.inc';
?>