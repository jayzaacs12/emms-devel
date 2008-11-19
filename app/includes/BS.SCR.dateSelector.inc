<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';

$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('dateForm','POST');

$form->addElement('static', 	'submit', 	'', 					$btn->imgsubmit('dateForm',$_LABELS['submit']));
$form->addElement('static', 	'olddate_l', 						$_LABELS['olddate'], 	$_REQUEST['olddate']);

$form->addElement('hidden',		'scr_name',	$_REQUEST['scr_name']);
$form->addElement('hidden',		'olddate',	$_REQUEST['olddate']);
$form->addElement('hidden',		'href',		$_REQUEST['href']);

$form->addElement('text',		'date',		$_LABELS['date'], 		$_REQUEST['date']);

//add rules
$form->addRule('date',	'yyyy-mm-dd','required');
$form->addRule('date',	'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');

if ($form->validate()) { 
  
  $form->freeze();
  $values = $form->_submitValues; 
  WEBPAGE::redirect(sprintf('%s&date=%s',$values['href'],$values['date']));
  
  } 

$tpl->setVariable('form_title',	$_LABELS['BS.SCR.dateSelector']);

$form->setDefaults();
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>