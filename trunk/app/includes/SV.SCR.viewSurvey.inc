<?php
require_once 'class/survey.php';

extract(SURVEY::getTemplateData($_REQUEST['id']));

$tpl->setVariable('name',				$name);
$tpl->setVariable('description',		$description);
  
$c = 1;
foreach ($survey_items as $key=>$val) {
  $txt = explode('|',$val[answer_txt]);
  $num = explode('|',$val[answer_num]);
  foreach($txt as $x => $y) {
    $tpl->setCurrentBlock("option");
    $tpl->setVariable("option_txt", $y) ;
    $tpl->setVariable("option_num", $num[$x]) ;
    $tpl->parseCurrentBlock("option");
    }
  $tpl->setCurrentBlock("item");
  $tpl->setVariable("question", $val[question]) ;
  $tpl->setVariable("num", $c++) ;
  $tpl->parseCurrentBlock("item");
  }

?>