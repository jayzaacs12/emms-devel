<?php
require_once 'HTML/Template/ITX.php';
require_once 'class/TTFButton.php';
$tpl = new HTML_Template_ITX('./templates');
$tpl->loadTemplateFile('index.popup.tpl');
//root template parser
$tpl->setCurrentBlock("html") ;
$tpl->setVariable('html_title_client_name',	strip_tags($_LABELS[WEBPAGE::$scr_name]));
$tpl->setVariable('client_name',	$_CONF['client_name']);
$tpl->setVariable('theme',			WEBPAGE::$themePath);
$tpl->setVariable('content_title',$_LABELS[WEBPAGE::$scr_name]);
$tpl->setVariable('theme_path',sprintf("%s%s",WEBPAGE::_THEMES_PATH,WEBPAGE::$theme));
//javascript block parser
if ( file_exists(sprintf("./templates/%s.%s",WEBPAGE::$scr_name,'js') )) { 
  $tpl->addBlockfile('js_functions_scr','js_functions_scr',sprintf("%s.%s",WEBPAGE::$scr_name,'js'));
  $tpl->setCurrentBlock("js_functions_scr") ;
  $tpl->setVariable('javascript',' ');
  $tpl->parseCurrentBlock("js_functions_scr") ;
  }if ( file_exists(sprintf("./templates/%s.%s",$_REQUEST['ref'],'js') )) {   $tpl->addBlockfile('js_functions_ref','js_functions_ref',sprintf("%s.%s",$_REQUEST['ref'],'js'));  $tpl->setCurrentBlock("js_functions_ref") ;  $tpl->setVariable('javascript',' ');  $tpl->parseCurrentBlock("js_functions_ref") ;  }
//content parser
$tpl->addBlockfile('content','content',sprintf("%s.%s",WEBPAGE::$scr_name,'tpl'));
$tpl->setCurrentBlock("content") ;
if ( file_exists(sprintf("./includes/%s.%s",WEBPAGE::$scr_name,'inc') )) {
  include sprintf("./includes/%s.%s",WEBPAGE::$scr_name,'inc');
  } 
$tpl->parseCurrentBlock("content") ;
$tpl->parseCurrentBlock("html") ;
$tpl->show();
?>