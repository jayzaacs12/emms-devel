<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/business_type.php';

$btype = new BUSINESS_TYPE();

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 	'submit', 	'', WEBPAGE::$btn->imgsubmit('searchForm',$_LABELS['search']));
$form->addElement('static', 	'reset', 	'', WEBPAGE::$btn->imgreset('searchForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', WEBPAGE::$btn->imglink($PHP_SELF,$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('select',		'activity',		$_LABELS['tblBusinessTypes.activity'],		($activities = $btype->activities()));

if ($form->validate()) { 

  $values = $form->_submitValues;

  $fields[] 					= "type";
  $extra['type']['header']		= 'tblBusinessTypes.type';
  $extra['type']['pack']		= '%s';
  $extra['type']['align']		= 'left';

//  $fields[] 					= 'id as view';
//  $extra['view']['header']		= 'view';
//  $extra['view']['pack']		= sprintf("<a href='%s?scr_name=BS.SCR.viewBusinessType&ref=BS.SCR.browseBusinessTypes&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['view']);
//  $extra['view']['align']		= 'center';

  $fields[] 					= 'id as edit';
  $extra['edit']['header']		= 'edit';
  $extra['edit']['pack']		= sprintf("<a href='%s?scr_name=BS.SCR.addBusinessType&ref=BS.SCR.editBusinessType&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['edit']);
  $extra['edit']['align']		= 'center';

  $param = sprintf("activity = '%s'", $values['activity']);
  
  $extra['param']['value']['activity']					= $values['activity'];
  $extra['param']['label']['tblBusinessTypes.activity']	= $_LABELS[sprintf("tblBusinessTypes.activity.%s",$values['activity'])];
    
  $extra['param']['ref']		= WEBPAGE::$scr_name;
  
  $mrow = SQL::select('tblBusinessTypes',$fields,$param);
  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);
  require_once 'BS.SCR.pager.inc';
  
  } else {

  $form->setDefaults();
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $tpl->setVariable('content_title',$_LABELS[WEBPAGE::$scr_name]);
  $form->accept($renderer);
  
  }
?>