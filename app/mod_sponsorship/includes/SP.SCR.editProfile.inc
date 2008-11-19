<?php
$show_gohome = true;

require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/sponsor.php';

$sponsor = new SPONSOR(WEBPAGE::$userID);
$btn     = new TTFButton(WEBPAGE::$theme);

$form    = new HTML_QuickForm('userForm','POST');

$form->addElement('static', 'submit', 	  '', $btn->imgsubmit('userForm',$_LABELS['submit']));
$form->addElement('static', 'reset', 	    '', $btn->imgreset('userForm',$_LABELS['reset']));
$form->addElement('static', 'cancel', 	  '', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden', 'scr_name',   $_REQUEST['scr_name']);
$form->addElement('hidden', 'id',			    WEBPAGE::$userID);

$form->addElement('text',		'username',		$_LABELS['tblUsers.username'],   'class=large');
$form->addElement('text',		'sponsor',	  $_LABELS['tblSponsors.sponsor'], 'class=large');
$form->addElement('text',		'first',		  $_LABELS['tblUsers.first'],      'class=large');
$form->addElement('text',		'middle',		  $_LABELS['tblUsers.middle'],     'class=large');
$form->addElement('text',		'last',			  $_LABELS['tblUsers.last'],       'class=large');
$form->addElement('text',		'email',		  $_LABELS['tblUsers.email'],      'class=large');

$form->addElement('textarea',	'memo',			$_LABELS['tblUsers.memo']);

//add rules
$form->addRule('username',	sprintf($_LABELS['rangelength'],4,12),'required');
$form->addRule('username',	sprintf($_LABELS['rangelength'],4,12),'rangelength', array(4,12));
$form->addRule('username',	$_LABELS['lettersonly'],'lettersonly');
$form->addRule('sponsor',	  sprintf($_LABELS['rangelength'],1,255),'required');
$form->addRule('sponsor',	  sprintf($_LABELS['rangelength'],1,255),'rangelength', array(1,255));
//$form->addRule('sponsor',	  $_LABELS['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('first',		  sprintf($_LABELS['rangelength'],1,128),'required');
$form->addRule('first',		  sprintf($_LABELS['rangelength'],1,128),'rangelength', array(1,128));
//$form->addRule('first',		  $_LABELS['lettersonly'],'lettersonly');
$form->addRule('middle',	  sprintf($_LABELS['rangelength'],1,128),'rangelength', array(1,128));
//$form->addRule('middle',	  $_LABELS['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('last',		  sprintf($_LABELS['rangelength'],1,128),'required');
$form->addRule('last',		  sprintf($_LABELS['rangelength'],1,128),'rangelength', array(1,128));
//$form->addRule('last',		  $_LABELS['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('email',		  $_LABELS['required'],'required');
$form->addRule('email',		  $_LABELS['emailorblank'],'emailorblank');
$form->addRule('memo',		  sprintf($_LABELS['maxlength'],255),'maxlength', 255);

$form->freeze($sponsor->frostdata);

if ($form->validate()) {

  $form->freeze();

  $values = $form->_submitValues;

  $sponsor->data['sponsor']     = $values['sponsor'];
  $sponsor->data['first']       = $values['first'];
  $sponsor->data['middle']      = $values['middle'];
  $sponsor->data['last']        = $values['last'];
  $sponsor->data['email']       = $values['email'];
  $sponsor->data['memo']        = $values['memo'];
//  $sponsor->data['editor_id']   = $sponsor->data['editor_id'];
//  $sponsor->data['editor_date'] = date('Y-m-d');

/*
  if ($values['password'] != $values['oldpassword']) {
  	$values['password'] =	crypt($values['password'], md5($values['password']));
  	} else {
  	unset ($values['password']);
  	}
*/
  $sponsor->update();
  
  $_SESSION['_authsession']['data']['first']   = $sponsor->data['first'];
  $_SESSION['_authsession']['data']['middle']  = $sponsor->data['middle'];
  $_SESSION['_authsession']['data']['last']    = $sponsor->data['last'];
  $_SESSION['_authsession']['data']['sponsor'] = $sponsor->data['sponsor'];
  
  WEBPAGE::redirect(sprintf('mod_sponsorship/index.php?scr_name=%s&ico=scs&msg=scs',WEBPAGE::_DEFAULT_FRONTPAGE));

  }

$tpl->setVariable('contact_info', $_LABELS['contact_info']);
$form->setDefaults($sponsor->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>