<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/business_type.php';

$btype = new BUSINESS_TYPE($_REQUEST['id']);
$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('btypeForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('btypeForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('btypeForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);

$form->addElement('text',		'type',			$_LABELS['tblBusinessTypes.type'], 'class=large');
$activity_radio[] = $form->createElement('radio',null,null,$_LABELS['tblBusinessTypes.activity.S'],'S', 'class=radio');
$activity_radio[] = $form->createElement('radio',null,null,$_LABELS['tblBusinessTypes.activity.I'],'I', 'class=radio');
$activity_radio[] = $form->createElement('radio',null,null,$_LABELS['tblBusinessTypes.activity.C'],'C', 'class=radio');
$activity_radio[] = $form->createElement('radio',null,null,$_LABELS['tblBusinessTypes.activity.F'],'F', 'class=radio');
$form->addGroup($activity_radio, 'activity', $_LABELS['tblBusinessTypes.activity'],' ');

//add rules
$form->addRule('type',			sprintf($_LABELS['rangelength'],1,32),'required');
$form->addRule('type',			sprintf($_LABELS['rangelength'],1,32),'rangelength', array(1,32));
$form->addRule('type',			$_LABELS['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('activity',		$_LABELS['selectOption'],'required');

$form->freeze($btype->frostdata);

if ($form->validate()) { 
  
  $form->freeze();
  
  $values = $form->_submitValues;
  
  $values['editor_date']  = date('Y-m-d');
  $values['creator_date'] = $btype->data['creator_date'] ? $btype->data['creator_date'] : $values['editor_date'];
  $values['editor_id']    = WEBPAGE::$userID;
  $values['creator_id']   = $btype->data['creator_id']    ? $btype->data['creator_id']   : $values['editor_id'];
  
  $lastID = SQL::insert('tblBusinessTypes',$values,'id');
  
  WEBPAGE::redirect('index.php?scr_name=BS.SCR.browseBusinessTypes');
  
  } 

$form->setDefaults($btype->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>