<?php
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/user.php';

extract(USER::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('user_name',			$name);
$tpl->setVariable('username_label',		$_LABELS['tblUsers.username']);
$tpl->setVariable('username',			$username);
$tpl->setVariable('code_label',			$_LABELS['tblUsers.code']);
$tpl->setVariable('code',				$code);
$tpl->setVariable('email_label',		$_LABELS['tblUsers.email']);
$tpl->setVariable('email',				$email);
$tpl->setVariable('super_label',		$_LABELS['tblUsers.super_id']);
$tpl->setVariable('super',				$super);
$tpl->setVariable('zone_label',			$_LABELS['tblZones.zone']);
$tpl->setVariable('zone',				$zone);
$tpl->setVariable('birthdate_label',	$_LABELS['tblUsers.birthdate']);
$tpl->setVariable('birthdate',			$f_birthdate);
$tpl->setVariable('gender_label',		$_LABELS['tblUsers.gender']);
$tpl->setVariable('gender',				$_LABELS[sprintf('tblUsers.gender.%s',$gender)]);
$tpl->setVariable('access_code_label',	$_LABELS['tblUsers.access_code']);
$tpl->setVariable('access_code',		implode('<br>',$membership));
$tpl->setVariable('memo_label',			$_LABELS['tblUsers.memo']);
$tpl->setVariable('memo',				$memo);
$tpl->setVariable('creator_label',		$_LABELS['tblUsers.creator_id']);
$tpl->setVariable('creator',			sprintf($_LABELS['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor_label',		$_LABELS['tblUsers.editor_id']);
$tpl->setVariable('editor',				sprintf($_LABELS['editedByOn'],$editor,$f_editor_date));
$tpl->setVariable('user_img',			$img_path);

include './includes/ST.SCR.buttonMan.inc';

?>