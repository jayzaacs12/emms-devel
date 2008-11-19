<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/coolbutton.php';
require_once 'class/roles.php';
require_once 'class/client.php';
require_once 'class/society.php';

$soc = new SOCIETY($_REQUEST[id]);
($soc->data[category] == 'B') ? '' : WEBPAGE::redirect('index.php?logout=1');
 
$parent_soc = new SOCIETY(($_REQUEST[parent_id] ? $_REQUEST[parent_id] :$soc->data[parent_id]));
$soc->data[parent] = $parent_soc->data[name];

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('societyForm','POST');

// Creating BG picking button for new B type societies
$button[id]	    		= "pickSocietybt";
$button[href]			= 'javascript:setParent()';
$button[alt]			= $_LABELS['pickSociety'];
$button[onClick]		= '';
$button[ico]			= "filter";
$picksocietybutton  	= New COOLBUTTON($button);

// create form elements
$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('societyForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('societyForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));
$form->addElement('static', 	'pickParent', 	'', COOLBUTTON::getButton($picksocietybutton));
  
$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);
$form->addElement('hidden',		'parent_id',	$_REQUEST['parent_id']);

$form->addElement('text',		'parent',		$_LABELS['tblSocieties.category.BG'],		'disabled class=large');
$form->addElement('text',		'name',			$_LABELS['tblSocieties.name'], 'class=large');

$form->addElement('select',		'category',		$_LABELS['tblSocieties.category'], 		$soc->categories(), 					'class=large onChange="javascript:unsetParent()"');
$form->addElement('select',		'advisor_id', 	$_LABELS['tblSocieties.advisor_id'], 	(array('-'=>'') + $soc->advisors()), 	'class=large onChange="javascript:unsetParent()"');
$form->addElement('select',		'zone_id', 		$_LABELS['tblSocieties.zone_id'], 		(array('-'=>'') + $soc->zones()), 		'class=large onChange="javascript:unsetParent()"');

$_REQUEST[parent_id] ? '' : $form->addRule('parent', $_LABELS['selectOption'],'required');
  
$form->addElement('textarea',	'memo',			$_LABELS['tblSocieties.memo']);

//add rules
$form->addRule('name',			sprintf($_LABELS['rangelength'],1,32),'required');
$form->addRule('name',			sprintf($_LABELS['rangelength'],1,32),'rangelength', array(1,32));
$form->addRule('name',			$_LABELS['lettersonly'],'regex', '(^[0-9a-zA-Z\.: ]*$)');
$form->addRule('category',		$_LABELS['selectOption'],'required');
$form->addRule('advisor_id',	$_LABELS['selectOption'],'required');
$form->addRule('advisor_id',	$_LABELS['selectOption'],'numeric');
$form->addRule('zone_id',		$_LABELS['selectOption'],'required');
$form->addRule('zone_id',		$_LABELS['selectOption'],'numeric');
$form->addRule('description',	sprintf($_LABELS['maxlength'],255),'maxlength', 255);

$soc->frostdata[] = 'name';
$form->freeze($soc->frostdata);

if ($form->validate()) { 
  
  $form->freeze();
  
  $values = $form->_submitValues;
  
  $values['editor_date']  = date('Y-m-d');
  $values['editor_id']    = WEBPAGE::$userID;
  
  $lastID = SQL::insert('tblSocieties',$values,'id');
  
  SQL::delete('tblLinkSocieties',sprintf("child_id = '%s' AND parent_id != child_id", $values['id']));
  SQL::insert('tblLinkSocieties',array('parent_id'=>$values['parent_id'],'child_id'=>$values['id']));
  SQL::insert('tblClients',array('zone_id'=>$values['zone_id'],'advisor_id'=>$values['advisor_id'],'editor_id'=>$values['editor_id'],'editor_date'=>$values['editor_date'],'society_id'=>$values['id']),'society_id');

  $soc = new SOCIETY($lastID);
  $soc->nameAutoUpdate();

  WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewSociety&id=%s&ico=scs&msg=scs',$values['id']));
  } 

$form->addRule('parent', $_LABELS['selectOption'],'required');
$form->setDefaults($soc->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>