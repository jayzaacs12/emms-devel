<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/roles.php';
require_once 'class/society.php';
require_once 'class/loan_type.php';

$ltype = new LOAN_TYPE();
$society = new SOCIETY();

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 	'submit', 	'', WEBPAGE::$btn->imgsubmit('searchForm',$_LABELS['search']));
$form->addElement('static', 	'reset', 	'', WEBPAGE::$btn->imgreset('searchForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', WEBPAGE::$btn->imglink($PHP_SELF,$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);


$form->addElement('text',		'code',		    $_LABELS['tblSocieties.code'], 			'class=large');
$form->addElement('text',		'name',			$_LABELS['tblSocieties.name'],		    'class=large');

$form->addElement('select',		'zone_id',		$_LABELS['tblSocieties.zone_id'],		($zones = (array(''=>'') + $society->zones())), 		'class=large');
$form->addElement('select',		'advisor_id',	$_LABELS['tblSocieties.advisor_id'],	($advisors = (array(''=>'') + $society->advisors())),	'class=large');

$form->addRule('zone_id',		$_LABELS['selectOption'],'required');
$form->addRule('zone_id',		$_LABELS['selectOption'],'numeric');
$form->addRule('advisor_id',	$_LABELS['selectOption'],'required');
$form->addRule('advisor_id',	$_LABELS['selectOption'],'numeric');

if ($form->validate()) { 

  $values = $form->_submitValues;
  
  $extra['param']['value']['name']	    					= $values['name'];
  $extra['param']['value']['code']	    					= $values['code'];
  $extra['param']['value']['zone_id']	    				= $values['zone_id'];
  $extra['param']['value']['advisor_id']					= $values['advisor_id'];

  $extra['param']['label']['tblSocieties.name']	    		= $values['name'] 		? $values['name'] 			       : $_LABELS['all'];
  $extra['param']['label']['tblSocieties.code']	    		= $values['code'] 		? $values['code'] 			       : $_LABELS['all'];
  $extra['param']['label']['tblSocieties.zone_id']	    	= $values['zone_id'] 	? $zones[$values['zone_id']]       : $_LABELS['all'];
  $extra['param']['label']['tblSocieties.advisor_id']		= $values['advisor_id']	? $advisors[$values['advisor_id']] : $_LABELS['all'];
    
  $extra['param']['ref']								= 'LN.SCR.doCollectionSheet';

  $fields[] 						= "name";
  $extra['name']['header']			= 'tblSocieties.name';
  $extra['name']['pack']			= '%s';
  $extra['name']['align']			= 'left';
    
  $fields[] 						= "code";
  $extra['code']['header']			= 'tblSocieties.code';
  $extra['code']['pack']			= '%s';
  $extra['code']['align']			= 'center';

  
  $fields[] 						= 'id as view';
  $extra['view']['header']			= 'view';
  $extra['view']['pack']			= sprintf("<a href='%s?scr_name=BS.SCR.viewSociety&ref=BS.SCR.browseSocieties&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['view']);
  $extra['view']['align']			= 'center';

  $fields[] 						= 'id as doCSheet';
  $extra['doCSheet']['header']		= 'select';
  $extra['doCSheet']['pack']		= sprintf('<a alt="" onClick="" href=javascript:openWin("index.popup.php?scr_name=LN.SCR.viewCollectionSheet&id=%s","viewCollectionSheet","menubar=no,scrollbars=yes,resizable=yes,width=800,height=320")>%s</a>', '%s',$_LABELS['select']);
  $extra['doCSheet']['align']		= 'center';

  $param = sprintf("code like '%%%s%%'", $values['code']);
  $table = sprintf("(SELECT ss.id, ss.name, concat(LPAD(ss.zone_id,3,'0'),'.',ss.category,'.',LPAD(ss.id,5,'0')) as code from tblSocieties AS ss, tblLoansMaster AS lm where ss.category = 'B' and ss.zone_id = '%s' AND ss.advisor_id='%s' AND ss.name like '%%%s%%' AND lm.borrower_id = ss.id AND lm.borrower_type = 'B' AND lm.check_status = 'R' group by ss.id) as view", $values['zone_id'], $values['advisor_id'], $values['name']);
 
  $mrow = SQL::select($table,$fields,$param);
  
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
