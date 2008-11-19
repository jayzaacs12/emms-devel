<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/survey.php';
require_once 'class/survey_item.php';

$sitem = new SURVEY_ITEM($_REQUEST['id']);
$_REQUEST['process'] = $_POST[process]; $_POST[process] = 1;
$_REQUEST['NOA'] = $_POST['NOA'] ? $_POST['NOA'] : $sitem->data[NOA]; 

$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('sitemForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('sitemForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('sitemForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);
$form->addElement('hidden',		'process',		1);

for ($i=0; $i<$_REQUEST[NOA]; $i++) {
  $name_txt = "answer_txt_".$i;
  $value_txt = $$name_txt;
  $name_num = "answer_num_".$i;
  $value_num = $$name_num;
  $form->addElement('text',		$name_txt,		$_LABELS['tblSurveyItems.answer_txt'], 'class=large');
  $form->addElement('text',		$name_num,		$_LABELS['tblSurveyItems.answer_num'], 'style="width: 60px; text-align:right"');
  //add answer's rules
  $form->addRule($name_txt,		sprintf($_LABELS['rangelength'],1,255),'required');
  $form->addRule($name_txt,		sprintf($_LABELS['rangelength'],1,255),'rangelength', array(1,255));
  $form->addRule($name_num,		'0 - 10','required');
  $form->addRule($name_num,		'0 - 10','numeric');
  }

$form->addElement('text',		'question',		$_LABELS['tblSurveyItems.question'], 'class=large');
$form->addElement('select',		'category',		$_LABELS['tblSurveyItems.category'], (array('-'=>'') + $sitem->item_categories()), 'class=large');
$form->addElement('select',		'NOA',			$_LABELS['tblSurveyItems.answer_txt'], array('2'=>'2','3'=>'3','4'=>'4','5'=>'5','6'=>'6','7'=>'7','8'=>'8','9'=>'9','10'=>'10'),  "onChange='javascript:refresh()'");

//add rules
$form->addRule('category',		$_LABELS['selectOption'],'required');
$form->addRule('category',		$_LABELS['selectOption'],'alphanumeric');
$form->addRule('question',		sprintf($_LABELS['rangelength'],1,255),'required');
$form->addRule('question',		sprintf($_LABELS['rangelength'],1,255),'rangelength', array(1,255));

$form->freeze($sitem->frostdata);

if ($_REQUEST['process']) {
  if ($form->validate()) { 
  
    $form->removeElement('submit');
    $form->removeElement('reset');
    $form->removeElement('cancel');

    $form->freeze();
  
    $values = $form->_submitValues;
  
    for($i=0;$i<$values[NOA];$i++) {
      $values[answer_txt][] = $values[sprintf("answer_txt_%s",$i)];
      $values[answer_num][] = $values[sprintf("answer_num_%s",$i)];
	  }
    $values[answer_txt] = implode('|',$values[answer_txt]);
    $values[answer_num] = implode('|',$values[answer_num]);
    
    $values['date'] = date('Y-m-d');
    $values['creator_id']   = WEBPAGE::$userID;
  
    $lastID = SQL::insert('tblSurveyItems',$values,'id');
  
    $form->addElement('static', 	'edit', 	'', $btn->imglink(WEBPAGE::appendParam2URL('id',$lastID),$_LABELS['edit']));
    $form->addElement('static', 	'close', 	'', $btn->imglink($PHP_SELF,$_LABELS['close']));
  
    } 
  }
  
$form->setDefaults($sitem->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>