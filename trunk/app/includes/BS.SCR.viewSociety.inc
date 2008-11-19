<?php
require_once 'class/coolbutton.php';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';
require_once 'class/society.php';

extract(SOCIETY::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('society',			$name);
$tpl->setVariable('code_label',			$_LABELS['tblSocieties.code']);
$tpl->setVariable('code',				$code);
$tpl->setVariable('category_label',		$_LABELS['tblSocieties.category']);
$tpl->setVariable('category',			$_LABELS[sprintf('tblSocieties.category.%s',$category)]);
if ($parent) {
  $tpl->setVariable('bde_label',		$_LABELS['tblSocieties.category.BG']);
  $tpl->setVariable('bde',				$parent);
  }
$tpl->setVariable('advisor_label',		$_LABELS['tblSocieties.advisor_id']);
$tpl->setVariable('advisor',			$advisor);
$tpl->setVariable('zone_label',		    $_LABELS['tblSocieties.zone_id']);
$tpl->setVariable('zone',				$zone);
$tpl->setVariable('memo_label',			$_LABELS['tblSocieties.memo']);
$tpl->setVariable('memo',				$memo);
$tpl->setVariable('creator',			sprintf($_LABELS['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor',				sprintf($_LABELS['editedByOn'],$editor,$f_editor_date));

$tpl->setVariable('refreshForm',		sprintf("<form name=refreshForm method=get>
													<input type=hidden name=scr_name value='%s'>
													<input type=hidden name=id value='%s'>
													<input type=hidden name=ref value='%s'>
													</form>",WEBPAGE::$scr_name,$_REQUEST['id'],$_REQUEST['ref']));

if (is_array($members)) {

  $tpl->setVariable('memberlist_label',	$_LABELS['memberlist']);

  $tpl->setVariable('legend',	sprintf("
							 <b>P</b>: %s, 
							 <b>T</b>: %s,
							 <b>S</b>: %s", 
							 $_LABELS[president],$_LABELS[treasurer],$_LABELS[secretary]));

  foreach ($members as $key => $data) {
    $tpl->setCurrentBlock("memberlist");  
    foreach ($data['member'] as $i => $row) { 
      $tpl->setCurrentBlock("memberdetails");
      $tpl->setVariable("member_name", sprintf(WEBPAGE::_HOTLINK,'BS.SCR.viewClient','id',$i,$row[name])) ;
      $tpl->setVariable("president", $row[president]) ;
      $tpl->setVariable("treasurer", $row[treasurer]) ;
      $tpl->setVariable("secretary", $row[secretary]) ;
      $tpl->parseCurrentBlock("memberdetails");
      }
    $tpl->setCurrentBlock("memberlist");
    if (!($parent)) {
      $tpl->setVariable("group_name", sprintf(WEBPAGE::_HOTLINK,'BS.SCR.viewSociety','id',$key,$data[name])) ;
//	  $movebutton = New COOLBUTTON($data[movebutton]);
//      $tpl->setVariable("move_button", COOLBUTTON::getButton($movebutton));
//	  $deactivatebutton = New COOLBUTTON($data[deactivatebutton]);
//      $tpl->setVariable("deactivate_button", COOLBUTTON::getButton($deactivatebutton));
	  }
	$tpl->parseCurrentBlock("memberlist");
    }
  }

include './includes/ST.SCR.buttonMan.inc';

?>