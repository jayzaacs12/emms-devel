<?php
require_once 'class/fund.php';

extract(FUND::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('fund',				$fund);
$tpl->setVariable('code_label',			$_LABELS['tblFunds.code']);
$tpl->setVariable('code',				$code);
$tpl->setVariable('currency_label',		$_LABELS['tblFunds.currency_id']);
$tpl->setVariable('currency',			$currency);
$tpl->setVariable('status_label',		$_LABELS['tblFunds.status']);
$tpl->setVariable('status',				$_LABELS[sprintf('tblFunds.status.%s',$status)]);
$tpl->setVariable('description_label',	$_LABELS['tblFunds.description']);
$tpl->setVariable('description',		$description);
$tpl->setVariable('creator',			sprintf($_LABELS['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor',				sprintf($_LABELS['editedByOn'],$editor,$f_editor_date));
$tpl->setVariable('fund_img',			$img_path);

include './includes/ST.SCR.buttonMan.inc';

?>