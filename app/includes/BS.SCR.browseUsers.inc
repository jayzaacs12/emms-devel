<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/user.php';

$user = new USER();

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 	'submit', 	'', WEBPAGE::$btn->imgsubmit('searchForm',$_LABELS['search']));
$form->addElement('static', 	'reset', 	'', WEBPAGE::$btn->imgreset('searchForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', WEBPAGE::$btn->imglink($PHP_SELF,$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('select',		'zone_id',		$_LABELS['tblUsers.zone_id'],		($zones = $user->zones()));

$active_radio[] = $form->createElement('radio',null,null,$_LABELS['tblUsers.active.1'],'1', 'class=radio');
$active_radio[] = $form->createElement('radio',null,null,$_LABELS['tblUsers.active.0'],'0', 'class=radio');
$form->addGroup($active_radio, 'active', $_LABELS['tblUsers.active'],' ');

if ($form->validate()) { 

  $values = $form->_submitValues;

  if ( $values['active'] )   { $operator_active  = '='; } else { $operator_active  = '!=';  }

  $fields[] 					= "CONCAT(first, ' ', last) as name";
  $extra['name']['header']		= 'name';
  $extra['name']['pack']		= '%s';
  $extra['name']['align']		= 'left';
    
  $fields[] 					= 'username';
  $extra['username']['header']	= 'tblUsers.username';
  $extra['username']['pack']	= '%s';
  $extra['username']['align']	= 'center';

  $fields[] 					= 'id as view';
  $extra['view']['header']		= 'view';
  $extra['view']['pack']		= sprintf("<a href='%s?scr_name=BS.SCR.viewUser&ref=BS.SCR.browseUsers&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['view']);
  $extra['view']['align']		= 'center';

  $fields[] 					= 'id as edit';
  $extra['edit']['header']		= 'edit';
  $extra['edit']['pack']		= sprintf("<a href='%s?scr_name=BS.SCR.addUser&ref=BS.SCR.editUsers&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['edit']);
  $extra['edit']['align']		= 'center';

  $param = sprintf("zone_id = %s AND active %s 1", $values['zone_id'], $operator_active);
  
  $extra['param']['value']['zone_id']	    		= $values['zone_id'];
  $extra['param']['value']['active']				= $values['active'];

  $extra['param']['label']['tblUsers.zone_id']	    = $zones[$values['zone_id']];
  $extra['param']['label']['tblUsers.active']		= $_LABELS[sprintf("tblUsers.active.%s",$values['active'])];
    
  $extra['param']['ref']							= WEBPAGE::$scr_name;
  
  $mrow = array();
  $mrow = SQL::select('tblUsers',$fields,$param);
  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);
  require_once 'BS.SCR.pager.inc';
  
  } else {

  $form->setDefaults(array('active'=>1));
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $tpl->setVariable('content_title',$_LABELS[WEBPAGE::$scr_name]);
  $form->accept($renderer);
  
  }
?>