<?php
$show_gohome = true;

require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/sponsor.php';

$sponsor = new SPONSOR(WEBPAGE::$userID);
$btn     = new TTFButton(WEBPAGE::$theme);

$form    = new HTML_QuickForm('userForm','POST');

$form->addElement('static',     'submit', 	   '', $btn->imgsubmit('userForm',$_LABELS['submit']));
$form->addElement('static',     'reset', 	     '', $btn->imgreset('userForm',$_LABELS['reset']));
$form->addElement('static',     'cancel', 	   '', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden',     'scr_name',    $_REQUEST['scr_name']);
$form->addElement('hidden',     'id',			     WEBPAGE::$userID);

$form->addElement('password',		'password',	   $_LABELS['tblUsers.password'], 'class=large');
$form->addElement('password',		'verify',		   $_LABELS['verify'],            'class=large');

//add rules
$form->addRule('password',	sprintf($_LABELS['rangelength'],6,16),'required');
$form->addRule('password',	sprintf($_LABELS['rangelength'],6,16),'rangelength', array(6,16));
$form->addRule('verify',	  $_LABELS['verifyPass'],'required');
$form->addRule('verify',	  $_LABELS['verifyPass'],'regex',sprintf("(%s)",$_POST['password']));

$form->freeze($sponsor->frostdata);

if ($form->validate()) {

  $form->freeze();

  $values = $form->_submitValues;
  $sponsor->changePassword(crypt($values['password'], md5($values['password'])));

  WEBPAGE::redirect(sprintf('mod_sponsorship/index.php?scr_name=%s&ico=scs&msg=scs',WEBPAGE::_DEFAULT_FRONTPAGE));

  }

$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>