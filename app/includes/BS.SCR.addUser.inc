<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/user.php';

$user = new USER($_REQUEST['id']);
$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('userForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('userForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('userForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);
$form->addElement('hidden',		'oldpassword');

$form->addElement('password',	'password',		$_LABELS['tblUsers.password']);
$form->addElement('password',	'verify',		$_LABELS['verify']);

$form->addElement('text',		'username',		$_LABELS['tblUsers.username']);
$form->addElement('text',		'first',		$_LABELS['tblUsers.first']);
$form->addElement('text',		'middle',		$_LABELS['tblUsers.middle']);
$form->addElement('text',		'last',			$_LABELS['tblUsers.last']);
$form->addElement('text',		'birthdate',	$_LABELS['tblUsers.birthdate']);
$form->addElement('text',		'code',			$_LABELS['tblUsers.code']);
$form->addElement('text',		'email',		$_LABELS['tblUsers.email']);

$form->addElement('textarea',	'memo',			$_LABELS['tblUsers.memo']);

$form->addElement('select',		'zone_id',		$_LABELS['tblUsers.zone_id'],		$user->zones());
$form->addElement('select',		'super_id',		$_LABELS['tblUsers.super_id'],		$user->supervisors());

$form->addElement('advcheckbox','active',		$_LABELS['tblUsers.active'],		$_LABELS['tblUsers.active.1'],'class=checkbox');

foreach ($user->roles() as $key => $value) {
  $roles_advcheckbox[] = $form->createElement('advcheckbox',$key,null,$value['label'],'class=checkbox',$value['code']);
  }
$form->addGroup($roles_advcheckbox, 'roles', $_LABELS['tblUsers.access_code'],'<br>');

$gender_radio[] = $form->createElement('radio',null,null,$_LABELS['tblUsers.gender.M'],'M', 'class=radio');
$gender_radio[] = $form->createElement('radio',null,null,$_LABELS['tblUsers.gender.F'],'F', 'class=radio');
$form->addGroup($gender_radio, 'gender', $_LABELS['tblUsers.gender'],' ');

$form->registerRule('checkusername', 	'callback', 'checkusername');

//add rules
$form->addRule('username', 	$_LABELS['usernameExists'], 'checkusername', true);
$form->addRule('username',	sprintf($_LABELS['rangelength'],4,12),'required');
$form->addRule('username',	sprintf($_LABELS['rangelength'],4,12),'rangelength', array(4,12));
$form->addRule('username',	$_LABELS['lettersonly'],'lettersonly');
//$form->addRule('password',	sprintf($_LABELS['rangelength'],6,16),'required');
//$form->addRule('password',	sprintf($_LABELS['rangelength'],6,16),'rangelength', array(6,16));
//$form->addRule('verify',	$_LABELS['verifyPass'],'regex',sprintf("(%s)",$_POST['password']));
$form->addRule('password',	sprintf($_LABELS['rangelength'],6,16),'required');
$form->addRule('password',	sprintf($_LABELS['rangelength'],6,16),'rangelength', array(6,16));
$form->addRule('verify',	  sprintf($_LABELS['rangelength'],6,16),'required');
$form->addRule('verify',	  $_LABELS['verifyPass'],'regex',sprintf("(^(%s{1})$)",$_POST['password']));
$form->addRule('first',		sprintf($_LABELS['rangelength'],1,16),'required');
$form->addRule('first',		sprintf($_LABELS['rangelength'],1,16),'rangelength', array(1,16));
$form->addRule('first',		$_LABELS['lettersonly'],'lettersonly');
$form->addRule('middle',	sprintf($_LABELS['rangelength'],1,16),'rangelength', array(1,16));
$form->addRule('middle',	$_LABELS['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('last',		sprintf($_LABELS['rangelength'],1,32),'required');
$form->addRule('last',		sprintf($_LABELS['rangelength'],1,32),'rangelength', array(1,32));
$form->addRule('last',		$_LABELS['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('birthdate',	'yyyy-mm-dd','required');
$form->addRule('birthdate',	'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('code',		$_LABELS['required'],'required');
$form->addRule('code',		$_CONF['regex_govID_tip'],'regex',$_CONF['regex_govID']);
$form->addRule('email',		$_LABELS['emailorblank'],'emailorblank');
$form->addRule('gender',	$_LABELS['required'],'required');
$form->addRule('memo',		sprintf($_LABELS['maxlength'],255),'maxlength', 255);



$form->freeze($user->frostdata);

if ($form->validate()) {

  $form->freeze();

  $values = $form->_submitValues;

  $values['editor_date']  = date('Y-m-d');
  $values['creator_date'] = $user->userdata['creator_date'] ? $user->userdata['creator_date'] : $values['editor_date'];
  $values['editor_id']    = WEBPAGE::$userID;
  $values['creator_id']   = $user->userdata['creator_id']   ? $user->userdata['creator_id']   : $values['editor_id'];
  $values['access_code']  = array_sum($values['roles']);
  if ($values['password'] != $values['oldpassword']) {
  	$values['password'] =	crypt($values['password'], md5($values['password']));
  	} else {
  	unset ($values['password']);
  	}

  $lastID = SQL::insert('tblUsers',$values,'id');

  WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewUser&id=%s&ico=scs&msg=scs',$lastID));

  }

$form->setDefaults($user->userdata);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

function checkusername($username) {

  $check = WEBPAGE::$dbh->getAll(sprintf("SELECT id,username FROM tblUsers WHERE username = '%s' AND id != '%s'",$username,$_REQUEST['id']));
  if (count($check)) { return false; } else { return true; }

  }
?>