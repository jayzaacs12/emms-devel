<?php
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';

extract(CLIENT::getTemplateData($_REQUEST['id']));

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('name',				$name);
if ($society_id) {
  $tpl->setVariable('society_label',	$_LABELS['tblClients.society_id']);
  $tpl->setVariable('society',			$society);
  $tpl->setVariable('society_code',		$society_code);
  }
if ($advisor_id) { $status = 'A'; } else { $status = 'I'; }
$tpl->setVariable('status_label',		$_LABELS['tblClients.status']);
$tpl->setVariable('status',				$_LABELS[sprintf('tblClients.status.%s',$status)]);
$tpl->setVariable('code_label',			$_LABELS['tblClients.code']);
$tpl->setVariable('code',				$code);
$tpl->setVariable('skills_label',		$_LABELS['tblClients.skills']);
$tpl->setVariable('skills',				$skills);
$tpl->setVariable('education_label',	$_LABELS['tblClients.education']);
$tpl->setVariable('education',			$education);
$tpl->setVariable('dependants_label',	$_LABELS['tblClients.dependants']);
$tpl->setVariable('dependants',			$dependants);
$tpl->setVariable('cstatus_label',		$_LABELS['tblClients.cstatus']);
$tpl->setVariable('cstatus',			$cstatus);
$tpl->setVariable('spouse_label',		$_LABELS['tblClients.spouse']);
$tpl->setVariable('spouse',				$spouse);
$tpl->setVariable('nick_label',			$_LABELS['tblClients.nick']);
$tpl->setVariable('nick',				$nick);
$tpl->setVariable('advisor_label',		$_LABELS['tblClients.advisor_id']);
$tpl->setVariable('advisor',			$advisor);
$tpl->setVariable('zone_label',			$_LABELS['tblClients.zone_id']);
$tpl->setVariable('zone',				$zone);
$tpl->setVariable('birthdate_label',	$_LABELS['tblClients.birthdate']);
$tpl->setVariable('birthdate',			$f_birthdate);
$tpl->setVariable('gender_label',		$_LABELS['tblClients.gender']);
$tpl->setVariable('gender',				$_LABELS[sprintf('tblUsers.gender.%s',$gender)]);
if (count($business)) {
  $tpl->setVariable('business',			implode('<br>',WEBPAGE::hotlist('BS.SCR.viewBusiness','id',$business)));
  $tpl->setVariable('business_label',		$_LABELS['tblBusiness.id']);
  }
// Modified - Update 2006-11-27
$tpl->setVariable('phone_label',         $_LABELS['tblClients.phone']);
$tpl->setVariable('phone',               $phone);
$tpl->setVariable('mobile_label',        $_LABELS['tblClients.mobile']);
$tpl->setVariable('mobile',               $mobile);
$tpl->setVariable('email_label',         $_LABELS['tblClients.email']);
$tpl->setVariable('email',               $email);
$tpl->setVariable('address_label',       $_LABELS['tblClients.address']);
$tpl->setVariable('address',               $address);
//
$tpl->setVariable('memo_label',			$_LABELS['tblClients.memo']);
$tpl->setVariable('memo',				$memo);
$tpl->setVariable('creator_label',		$_LABELS['tblClients.creator_id']);
$tpl->setVariable('creator',			sprintf($_LABELS['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor_label',		$_LABELS['tblClients.editor_id']);
$tpl->setVariable('editor',				sprintf($_LABELS['editedByOn'],$editor,$f_editor_date));
$tpl->setVariable('img',				$img_path);

include './includes/ST.SCR.buttonMan.inc';

?>