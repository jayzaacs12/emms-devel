<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/coolbutton.php';

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('spForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('spForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('spForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);

$form->addElement('text',		'name',			'Product Name',				'class=large');
$form->addElement('text',		'shortname',	'Product Short Name',		'class=large');
$form->addElement('text',		'idcode',		'Product Identification',	'class=large');
$form->addElement('text',		'description',	'Description',				'class=large');
$form->addElement('text',		'mopenbalance',	'Minimum Opening Balance',	'class=large');
$form->addElement('text',		'delaymargin',	'Delay margin - days',		'class=large');
$form->addElement('text',		'feeamount',	'Fee Amount',				'class=large');
$form->addElement('text',		'wdrawrestrict',		'Withdrawal Restriction',				'class=large');
$form->addElement('text',		'depositamountvalue',	'Deposit amount value',				'class=large');

$form->addElement('select',		'depositfreq',	'Deposit frequency',		array('','Weekly','Bi-weekly','Monthly','Quarterly','Semiannual','Annual'),	'class=large');
$form->addElement('select',		'rate',			'Anual Rate',				array('','1','2','3','4','5','6','7','8','9','10'),							'class=large');
$form->addElement('select',		'feetype',		'Fee type',					array('','Administrative'),													'class=large');

$form->addElement('textarea',	'description',	'Description', 				'class=large');

$category_radio[] = $form->createElement('radio',null,null,'Individual','I', 'class=radio');
$category_radio[] = $form->createElement('radio',null,null,'Group',		'G', 'class=radio');
$form->addGroup($category_radio, 'category', 'Product category','</td><td>');

$type_radio[] = $form->createElement('radio',null,null,'Voluntary',		'V', 'class=radio');
$type_radio[] = $form->createElement('radio',null,null,'Mandatory',		'M', 'class=radio');
$form->addGroup($type_radio, 'type', 'Product type','</td><td>');

$fixduration_radio[] = $form->createElement('radio',null,null,'Yes',	'Y', 'class=radio');
$fixduration_radio[] = $form->createElement('radio',null,null,'No',		'N', 'class=radio');
$form->addGroup($fixduration_radio, 'fixduration', 'Fix duration?','</td><td>');

$depositamount_radio[] = $form->createElement('radio',null,null,'Value amount',					'A', 'class=radio');
$depositamount_radio[] = $form->createElement('radio',null,null,'Percent of linked account',	'P', 'class=radio');
$form->addGroup($depositamount_radio, 'depositamounttype', 'Deposit amount type','</td><td>');

//$form->freeze($client->frostdata);

if ($form->validate()) { 
 
  $form->removeElement('submit');
  $form->removeElement('reset');
  $form->removeElement('cancel');

  $form->freeze();
    
  $form->addElement('static', 	'view', 	'', $btn->imglink(sprintf("%s?scr_name=%s&ref=%s&id=%s",$PHP_SELF,'BS.SCR.viewClient',WEBPAGE::$scr_name,$lastID),$_LABELS['view']));
  $form->addElement('static', 	'edit', 	'', $btn->imglink(WEBPAGE::appendParam2URL('id',$lastID),$_LABELS['edit']));
  $form->addElement('static', 	'close', 	'', $btn->imglink($PHP_SELF,$_LABELS['close']));
  
  } 

//$form->setDefaults($client->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>