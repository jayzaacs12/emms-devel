<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once './mod_sponsorship/class/sponsor.php';

$sponsor = new SPONSOR($_REQUEST['id']);
$btn     = new TTFButton(WEBPAGE::$theme);

$form    = new HTML_QuickForm('userForm','POST');

$form->addElement('static', 'submit', 	  '', $btn->imgsubmit('userForm',$_LABELS['submit']));
$form->addElement('static', 'reset', 	    '', $btn->imgreset('userForm',$_LABELS['reset']));
$form->addElement('static', 'cancel', 	  '', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden', 'scr_name',   $_REQUEST['scr_name']);
$form->addElement('hidden', 'id',			    $sponsor->data['id']);
$form->addElement('hidden', 'oldpassword');

$form->addElement('password',	'password',	$_LABELS['tblUsers.password'],   'class=large');
$form->addElement('password',	'verify',		$_LABELS['verify'],              'class=large');

$form->addElement('text',		'username',		$_LABELS['tblUsers.username'],   'class=large');
$form->addElement('text',		'sponsor',	  $_LABELS['tblSponsors.sponsor'], 'class=large');
$form->addElement('text',		'first',		  $_LABELS['tblUsers.first'],      'class=large');
$form->addElement('text',		'middle',		  $_LABELS['tblUsers.middle'],     'class=large');
$form->addElement('text',		'last',			  $_LABELS['tblUsers.last'],       'class=large');
$form->addElement('text',		'email',		  $_LABELS['tblUsers.email'],      'class=large');

$form->addElement('textarea',	'memo',			$_LABELS['tblUsers.memo']);

$form->addElement('select',     'zone_id',          $_LABELS['tblPrograms.program'],    $sponsor->getZones(),              'multiple size="5" class=large');

$form->registerRule('checkusername', 	'callback', 'checkusername');

//add rules
$form->addRule('username', 	$_LABELS['usernameExists'], 'checkusername', true);
$form->addRule('username',	sprintf($_LABELS['rangelength'],4,12),'required');
$form->addRule('username',	sprintf($_LABELS['rangelength'],4,12),'rangelength', array(4,12));
$form->addRule('username',	$_LABELS['lettersonly'],'lettersonly');
$form->addRule('password',	sprintf($_LABELS['rangelength'],6,16),'required');
$form->addRule('password',	sprintf($_LABELS['rangelength'],6,16),'rangelength', array(6,16));
$form->addRule('verify',	  sprintf($_LABELS['rangelength'],6,16),'required');
$form->addRule('verify',	  $_LABELS['verifyPass'],'regex',sprintf("(^(%s{1})$)",$_POST['password']));
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

  $sponsor->data['username']    = $values['username'];
  $sponsor->data['password']    = crypt($values['password'], md5($values['password']));
  $sponsor->data['sponsor']     = $values['sponsor'];
  $sponsor->data['first']       = $values['first'];
  $sponsor->data['middle']      = $values['middle'];
  $sponsor->data['last']        = $values['last'];
  $sponsor->data['email']       = $values['email'];
  $sponsor->data['memo']        = $values['memo'];
  $sponsor->data['zone_id']     = $values['zone_id'];
  $sponsor->data['editor_id']   = WEBPAGE::$userID;
  $sponsor->data['editor_date'] = date('Y-m-d');

  if ($values['password'] != $values['oldpassword']) {
  	$sponsor->changePassword($sponsor->data['password']);
  	}

  $sponsor->data['id'] = $sponsor->data['id'] ? $sponsor->update() : $sponsor->add() ;

  WEBPAGE::redirect(sprintf('index.php?scr_name=SP.SCR.viewSponsor&ref=SP.SCR.browseSponsors&id=%s&ico=scs&msg=scs',$sponsor->data['id']));

  }

$tpl->setVariable('contact_info', $_LABELS['contact_info']);
$form->setDefaults($sponsor->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

function checkusername($username) {

  $check = WEBPAGE::$dbh->getAll(sprintf("SELECT id, username FROM tblSponsors WHERE username = '%s' AND id != '%s'",$username,$_REQUEST['id']));
  if (count($check)) { return false; } else { return true; }

  }
?>