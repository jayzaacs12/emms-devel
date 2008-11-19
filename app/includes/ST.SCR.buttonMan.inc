<?php
require_once 'class/coolbutton.php';

$c = 0;
for($i=0;$i<count($buttondata);$i++) {
  if ( WEBPAGE::$userAccessCode & intval($_CONF[$buttondata[$i][id]]) ) {
    $buttondata[$i][id] = sprintf("bd%s",$i);
    $button[$c] = $buttondata[$i];
    $c++;
    } 		
  }

$button[$c][id] 		= "back";
$button[$c][href] 		= sprintf("JavaScript:history.go(-1)");
$button[$c][alt] 		= $_LABELS['back'];
$button[$c][onClick] 	= "";
$button[$c][ico] 		= "back";	
/*
$c++;
$button[$c][id] 		= "forward";
$button[$c][href] 		= sprintf("JavaScript:history.go(1)");
$button[$c][alt] 		= $_LABELS['forward'];
$button[$c][onClick] 	= "";
$button[$c][ico] 		= "forward";	
*/
$c++;
$button[$c][id] 		= "help";
$button[$c][href] 		= sprintf("javascript:openWin('index.php?scr_name=HP.SCR.help&hlp=%s&hlp_ctxt=%s','HELP','menubar=no,scrollbars=yes,resizable=yes,width=300,height=600,top=40,left=%s')",$scr_name,$hlp_ctxt,($screenWidth-315));
$button[$c][alt] 		= $_LABELS['help'];
$button[$c][onClick]	= "";
$button[$c][ico] 		= "help";	

/*
$c++;
$button[$c][id] 		= "close";
$button[$c][href] 		= "index.php";
$button[$c][alt] 		= $_LABELS['close'];
$button[$c][onClick] 	= "";
$button[$c][ico] 		= "ok";			
*/

$tpl->addBlockfile('toolbar','toolbar','toolbar.tpl');
foreach ($button as $key => $val) {
  $coolbutton = New COOLBUTTON($val);
  $tpl->setCurrentBlock("bar");
  $tpl->setVariable("button", COOLBUTTON::getButton($coolbutton)) ;
  $tpl->parseCurrentBlock("bar");
  }

/*

    function printbar($data)
    {
    $num = count($data->button);
    for ($i=0;$i<$num;$i++) {
		$button = New COOLBUTTON($data->button[$i]);
		COOLBUTTON::printbutton($button);	
//		printf (" %s <a href=\"%s\" onClick=\"%s\">%s<a>", $pad, $data->button[$i][href], $data->button[$i][onClick], $data->button[$i][alt]);
 //   	$pad = "|";
		}	
    }

}
*/
?>