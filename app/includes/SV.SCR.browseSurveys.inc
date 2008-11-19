<?php
//require_once 'class/survey.php';

//$survey = new SURVEY();

$fields[] 					= 's.name';
$extra['name']['header']	= 'tblSurveys.name';
$extra['name']['pack']		= '%s';
$extra['name']['align']		= 'left';
  
$fields[] 					= 's.id as view';
$extra['view']['header']	= 'view';
$extra['view']['pack']		= sprintf("<a href='%s?scr_name=SV.SCR.viewSurvey&ref=SV.SCR.browseSurveys&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['view']);
$extra['view']['align']		= 'center';

$fields[] 					= 's.id as edit';
$extra['edit']['header']	= 'edit';
$extra['edit']['pack']		= sprintf("<a href='%s?scr_name=SV.SCR.addSurvey&ref=SV.SCR.editSurvey&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['edit']);
$extra['edit']['align']		= 'center';

if ($_GET[client_id]) {
  $fields[] 					= 's.id as apply';
  $extra['apply']['header']		= 'apply';
  $extra['apply']['pack']		= sprintf("<a href='%s?scr_name=SV.SCR.applySurveyStepB&ref=SV.SCR.applySurveyStepA&client_id=%s&id=%s'>%s</a>", $PHP_SELF,$_GET[client_id],'%s',$_LABELS['apply']);
  $extra['apply']['align']		= 'center';
  }

$params = true;
if ($_GET[stats_client_id]) {
  $fields[] 					= 's.id as stats';
  $extra['stats']['header']		= 'stats';
  $extra['stats']['pack']		= sprintf("<a href='%s?scr_name=SV.SCR.viewGraph&client_id=%s&survey_id=%s'>%s</a>", $PHP_SELF,$_GET[stats_client_id],'%s',$_LABELS['stats']);
  $extra['stats']['align']		= 'center';
  
  $params = sprintf('sa.client_id = %s AND s.id = sa.survey_id',$_GET[stats_client_id]);
  }

  $extra['param']['ref']		= WEBPAGE::$scr_name;
  
  $mrow = SQL::select_distinct('tblSurveys AS s, tblSurveyAnswers AS sa',$fields,$params);
  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);
  require_once 'BS.SCR.pager.inc';
  
?>