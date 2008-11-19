<?php
require_once 'HTML/Template/ITX.php';
require_once 'class/TTFButton.php';

$tpl = new HTML_Template_ITX('./templates');
$tpl->loadTemplateFile('index.tpl');


//tabmenu template parser
for ( $i=0;$i<count(WEBPAGE::$tabmenu);$i++ ) {
  $tpl->setCurrentBlock("tabitem");
  $tpl->setVariable("URL", sprintf("index.php?scr_name=%s",WEBPAGE::$tabmenu[$i]['URL'])) ;
  $tpl->setVariable("TAB", WEBPAGE::$tabmenu[$i]['TAB']) ;
  $tpl->parseCurrentBlock("tabitem");
  }
$tpl->setCurrentBlock("tabmenu") ;
$tpl->parseCurrentBlock("tabmenu") ;

//current path template parser
$path = WEBPAGE::getCurrentPath(); 
for ( $i=0;$i<count($path);$i++ ) {
  $tpl->setCurrentBlock("pathitem");
  if ($i) $tpl->setVariable("PAD",' > ');
  $tpl->setVariable("PATH", $path[$i]) ;
  $tpl->parseCurrentBlock("pathitem");
  }
$tpl->setCurrentBlock("path") ;
$tpl->parseCurrentBlock("path") ;

//root template parser
$tpl->setCurrentBlock("html") ;
$tpl->setVariable('html_title_client_name',	strip_tags($_CONF['client_name']));
$tpl->setVariable('client_name',	$_CONF['client_name']);
$tpl->setVariable('theme',			WEBPAGE::$themePath);
$tpl->setVariable('screenWidth',	WEBPAGE::$screenWidth);
$tpl->setVariable('greetings',		sprintf($_LABELS['welcome'],WEBPAGE::$userName,date($_CONF['date_format'])));
$tpl->setVariable('treemenulabel',	$_LABELS['menu']);
$tpl->setVariable('navtree',		WEBPAGE::$navtree);

if ($_LABELS[$_GET['msg']]) {
  $tpl->addBlockfile('message','message','ST.SCR.message.tpl');
  $tpl->setCurrentBlock("message") ;
  $tpl->setVariable('ico',sprintf('%s/icons/%s.png',WEBPAGE::$themePath,$_GET['ico']));
  $tpl->setVariable('msg',$_LABELS[$_GET['msg']]);
  $tpl->parseCurrentBlock("message") ;
  }
//  printf("%s.%s",WEBPAGE::$scr_name,'tpl'); exit;
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
$tpl->parseCurrentBlock("content") ;


$tpl->parseCurrentBlock("html") ;

$tpl->show();
?>