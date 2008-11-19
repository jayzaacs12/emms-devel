<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/roles.php';
require_once 'class/society.php';

$society = new SOCIETY();

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 	'submit', 	'', WEBPAGE::$btn->imgsubmit('searchForm',$_LABELS['search']));
$form->addElement('static', 	'reset', 	'', WEBPAGE::$btn->imgreset('searchForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', WEBPAGE::$btn->imglink($PHP_SELF,$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);

$form->addElement('text',		'code',		    $_LABELS['tblSocieties.code'], 			'class=large');
// PATCH: 2008.07.18
$form->addElement('text',		'name',		    $_LABELS['tblSocieties.name'], 			'class=large');
$form->addElement('select',		'zone_id',		$_LABELS['tblSocieties.zone_id'],		(array('') + $society->zones()), 		'class=large');
$form->addElement('select',		'advisor_id',	$_LABELS['tblSocieties.advisor_id'],	(array('') + $society->advisors()),		'class=large');
$form->addElement('select',		'category',		$_LABELS['tblSocieties.category'],		$society->categories(),					'class=large');

if ($form->validate()) { 

  $values = $form->_submitValues;
 
  $extra['param']['value']['code']	    				= $values['code'];
  $extra['param']['value']['zone_id']	    			= $values['zone_id'];
  $extra['param']['value']['advisor_id']				= $values['advisor_id'];
  $extra['param']['value']['category']	    			= $values['category'];

  $extra['param']['label']['tblSocieties.code']	    	= $values['code'] 		? $values['code'] 			       : $_LABELS['all'];
  $extra['param']['label']['tblSocieties.zone_id']	    = $values['zone_id'] 	? $zones[$values['zone_id']]       : $_LABELS['all'];
  $extra['param']['label']['tblSocieties.advisor_id']	= $values['advisor_id']	? $advisors[$values['advisor_id']] : $_LABELS['all'];
  $extra['param']['label']['tblSocieties.category']	    = $_LABELS[sprintf('tblSocieties.category.%s',$values['category'])];
    
  $extra['param']['ref']								= WEBPAGE::$scr_name;

  $fields[] 					= "s.name";
  $extra['name']['header']		= 'tblSocieties.name';
  $extra['name']['pack']		= '%s';
  $extra['name']['align']		= 'left';
    
  $fields[] 					= "CONCAT((LPAD(z.id,3,'0')),'.',s.category,'.',(LPAD(s.id,5,'0'))) AS code";
  $extra['code']['header']		= 'tblSocieties.code';
  $extra['code']['pack']		= '%s';
  $extra['code']['align']		= 'center';

  $fields[] 					= 's.id as view';
  $extra['view']['header']		= 'view';
  $extra['view']['pack']		= sprintf("<a href='%s?scr_name=BS.SCR.viewSociety&ref=BS.SCR.browseSocieties&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['view']);
  $extra['view']['align']		= 'center';

  $fields[] 					= 's.id as edit';
  $extra['edit']['header']		= 'edit';
  $extra['edit']['pack']		= sprintf("<a href='%s?scr_name=BS.SCR.addSociety&ref=BS.SCR.editSociety&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['edit']);
  $extra['edit']['align']		= 'center';

  if ( $values['zone_id'] )   	{ $operator_zone_id  	= '='; } else { $operator_zone_id  	  = 'LIKE'; $values['zone_id']    = '%%'; }
  if ( $values['advisor_id'] )	{ $operator_advisor_id  = '='; } else { $operator_advisor_id  = 'LIKE'; $values['advisor_id'] = '%%'; }
// PATCH: 2008.07.18
  if ( $values['name'] )	{ $operator_name  = 'LIKE'; $values['name'] = sprintf("%%%s%%",$values['name']); } else { $operator_name  = 'LIKE'; $values['name'] = '%%'; }
// PATCH: 2008.07.18 $param sql string change  
  $param = sprintf("CONCAT((LPAD(z.id,3,'0')),'.',s.category,'.',(LPAD(s.id,5,'0'))) LIKE '%%%s%%'
		    			AND s.name %s '%s'
                     AND s.category = '%s'
		    			AND s.zone_id %s '%s'
		    			AND s.advisor_id %s '%s'
		    			AND z.id = s.zone_id",
		    			$values['code'],
                        $operator_name,$values['name'],
						$values['category'], 
						$operator_zone_id, $values['zone_id'], 
						$operator_advisor_id, $values['advisor_id']);
   
  
  $mrow = SQL::select('tblSocieties AS s, tblZones AS z',$fields,$param);
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