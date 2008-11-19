<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/roles.php';
require_once 'class/eSCRIPT.php';

$script = new eSCRIPT($_REQUEST['id']);

$btn  	= new TTFButton(WEBPAGE::$theme);
$form 	= new HTML_QuickForm('maskForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('maskForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('maskForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('static', 	'eSCRIPT_link', 	$_LABELS['eSCRIPT'], $script->data['link']);
$form->addElement('static', 	'eSCRIPT_lang', 	$_LABELS['description'], $script->data[WEBPAGE::$lang]);


$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);

foreach ($script->roles() as $key => $value) {
  $roles_advcheckbox[] = $form->createElement('advcheckbox',$key,null,$value['label'],'class=checkbox',$value['code']);
  }
$form->addGroup($roles_advcheckbox, 'roles', $_LABELS['tblUsers.access_code'],'<br>');

if ($form->validate()) { 
  
  $form->freeze(); 
  $values = $form->_submitValues;  
  $mask['id']  			= $values['id'];  
  $mask['access_code']  = array_sum($values['roles']);  
  $lastID = SQL::insert('tblMenus',$mask,'id'); 
  
  WEBPAGE::redirect(sprintf('index.php?scr_name=MN.SCR.browseAccessMasks&ico=scs&msg=editAccessMask.scs'));
  
  } 

$form->setDefaults($script->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>
