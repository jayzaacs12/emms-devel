<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/society.php';

$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('pickCatForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('pickCatForm',$_LABELS['submit']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);

foreach (SOCIETY::categories() AS $key=>$val) {
  $cat_radio[] = $form->createElement('radio',null,null,$_LABELS[sprintf('tblSocieties.category.%s',$key)],$key, 'class=radio');
  }
$form->addGroup($cat_radio, 'cat', $_LABELS['tblSocieties.category'],' ');
$form->addRule('cat',	$_LABELS['required'],'required');

$form->validate() ? WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.addSociety&cat=%s',$form->_submitValues[cat])) : '';   

$tpl->setVariable('pickOptionToContinue', $_LABELS['pickOptionToContinue']);

$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>