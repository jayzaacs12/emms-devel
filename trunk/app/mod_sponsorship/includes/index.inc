<?php
require_once 'HTML/Template/ITX.php';
require_once '../class/TTFButton.php';

$tpl = new HTML_Template_ITX('./templates');
$tpl->loadTemplateFile('index.tpl');

//root template parser
$tpl->setCurrentBlock("html") ;
$tpl->setVariable('html_title_client_name',	strip_tags($_CONF['client_name']));
$tpl->setVariable('client_name',	$_CONF['client_name']);
$tpl->setVariable('theme',			WEBPAGE::$themePath);
$tpl->setVariable('sponsorship_program_name',		$_CONF['sponsorship_program_name']);
$tpl->setVariable('screenWidth',	WEBPAGE::$screenWidth);
$tpl->setVariable('greetings',		sprintf($_LABELS['welcome'],$_SESSION['_authsession']['username'],date($_CONF['date_format'])));

$tpl->setVariable('sponsor_name',		  $_SESSION['_authsession']['data']['sponsor']);
$tpl->setVariable('sponsor_contact',  sprintf('%s %s %s',$_SESSION['_authsession']['data']['first'],$_SESSION['_authsession']['data']['middle'],$_SESSION['_authsession']['data']['last']));

if ($_LABELS[$_GET['msg']]) {
  $tpl->addBlockfile('message','message','ST.SCR.message.tpl');
  $tpl->setCurrentBlock("message") ;
  $tpl->setVariable('ico',sprintf('%s/icons/%s.png',WEBPAGE::$themePath,$_GET['ico']));
  $tpl->setVariable('msg',$_LABELS[$_GET['msg']]);
  $tpl->parseCurrentBlock("message") ;
  }

//content parser
$tpl->setVariable('content_title',$_LABELS[WEBPAGE::$scr_name]);
$tpl->addBlockfile('content','content',sprintf("%s.%s",WEBPAGE::$scr_name,'tpl'));
$tpl->setCurrentBlock("content") ;

if ( file_exists(sprintf("./includes/%s.%s",WEBPAGE::$scr_name,'inc') )) {
  include sprintf("./includes/%s.%s",WEBPAGE::$scr_name,'inc');
  } else {
  $tpl->addBlockfile('message','message','ST.SCR.message.tpl');
  $tpl->setCurrentBlock("message") ;
  $tpl->setVariable('ico',sprintf('%s/icons/%s.png',WEBPAGE::$themePath,'inf'));
  $tpl->setVariable('msg',$_LABELS['inf.na']);
  $tpl->parseCurrentBlock("message") ;
  }

//if ( WEBPAGE::$scr_name != WEBPAGE::_DEFAULT_FRONTPAGE) {
if ( $show_gohome) {
  $tpl->setVariable('gohome_ico',sprintf("<a href='index.php?scr_name=%s'><img border=0 src='%s/icons/48x48/home.png'></a>",WEBPAGE::_DEFAULT_FRONTPAGE,WEBPAGE::$themePath));
  $tpl->setVariable('gohome_txt', sprintf("<a href='index.php?scr_name=%s'>%s</a>",WEBPAGE::_DEFAULT_FRONTPAGE,$_LABELS['quick_jumps']));
  }

$tpl->parseCurrentBlock("content") ;


$tpl->parseCurrentBlock("html") ;

$tpl->show();
?>