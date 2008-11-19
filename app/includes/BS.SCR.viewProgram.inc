<?php
require_once 'class/program.php';

extract(PROGRAM::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('program',			$program);
$tpl->setVariable('status_label',		$_LABELS['tblPrograms.status']);
$tpl->setVariable('status',				$_LABELS[sprintf('tblPrograms.status.%s',$status)]);
$tpl->setVariable('funds_label',		$_LABELS['tblPrograms.fund_id']);
$tpl->setVariable('funds',				implode('<br>',WEBPAGE::hotlist('AC.SCR.viewFund','id',PROGRAM::getFunds($_REQUEST['id']))));
$tpl->setVariable('description_label',	$_LABELS['tblPrograms.description']);
$tpl->setVariable('description',		$description);
$tpl->setVariable('creator',			sprintf($_LABELS['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor',				sprintf($_LABELS['editedByOn'],$editor,$f_editor_date));
$tpl->setVariable('fund_img',			$img_path);

if ($confidential) { 
  $tpl->setVariable('confidential', sprintf(" - %s",$_LABELS['tblPrograms.confidential']));
  }

include './includes/ST.SCR.buttonMan.inc';

?>