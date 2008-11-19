<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/coolbutton.php';
require_once 'class/roles.php';
require_once 'class/client.php';
require_once 'class/society.php';


$_id = $_GET['parent_id'] ? '' : $_REQUEST['id'];
$soc = new SOCIETY($_id,$category);

// Society category needs to be specified in advanced. 
// If not the browser redirected to pick one
$_cat = $soc->data['category'] ? $soc->data['category'] : $_REQUEST['cat'];
$_cat = $_GET['parent_id']   ? 'B' : $_cat;
$_cat = in_array($_cat,array_keys(SOCIETY::categories())) ? $_cat : '';

$soc->data['category'] = $_cat ? $_cat : WEBPAGE::redirect('index.php?scr_name=BS.SCR.pickSocietyCat');
$soc->frostdata[] = 'category';


$members = $soc->getMembers();
$soc->data['confidential'] ? WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewSociety&id=%s&ico=stp&msg=stp',$soc->data['id'])) : '';

$_parent = $_GET['parent_id'] ? $_GET['parent_id'] :  $soc->data['parent_id'];
 
// Creating BG picking button for new B type societies
if ($_cat == 'B') {
  $button['id']	    	= "pickSocietybt";
  $button['href']		= 'javascript:setParent()';
  $button['alt']		= $_LABELS['pickSociety'];
  $button['onClick']	= '';
  $button['ico']		= "filter";
  $picksocietybutton  	= New COOLBUTTON($button);

  $parent_soc = new SOCIETY(($_REQUEST['parent_id'] ? $_REQUEST['parent_id'] : $_parent));
  $soc->data['parent'] = $parent_soc->data['name'];
  }

// Preload significative(inheritable) fields if parent_id provided  
if ($_parent) {
  $parent_soc = new SOCIETY($_parent);
  $soc->frostdata[] 		= 'advisor_id';
  $soc->frostdata[] 		= 'zone_id';  
  $soc->frostdata[] 		= 'parent';  
  $soc->data['advisor_id'] 	= $parent_soc->data['advisor_id'];
  $soc->data['zone_id'] 	= $parent_soc->data['zone_id'];  
  $soc->data['parent'] 		= $parent_soc->data['name'];  
  }

$btn  = new TTFButton(WEBPAGE::$theme);
// create form elements
$form = new HTML_QuickForm('societyForm','POST');
$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('societyForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('societyForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));
  
$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);
$form->addElement('hidden',		'parent_id',	$_GET['parent_id']);
$form->addElement('hidden',		'cat',	$_cat);

$form->addElement('text',		'name',			$_LABELS['tblSocieties.name'], 'class=large');

$form->addElement('select',		'category',		$_LABELS['tblSocieties.category'], 		$soc->categories(), 					'class=large onChange="javascript:unsetParent()"');
$form->addElement('select',		'advisor_id', 	$_LABELS['tblSocieties.advisor_id'], 	(array('-'=>'') + $soc->advisors()), 	'class=large onChange="javascript:unsetParent()"');
$form->addElement('select',		'zone_id', 		$_LABELS['tblSocieties.zone_id'], 		(array('-'=>'') + $soc->zones()), 		'class=large onChange="javascript:unsetParent()"');

if ($_cat == 'B') { // only valid for B type Societies
  $form->addElement('static', 	'pickParent', 	'', COOLBUTTON::getButton($picksocietybutton));
  $form->addElement('text',		'parent',		$_LABELS['tblSocieties.category.BG'],			'disabled class=large');
  $_REQUEST['parent_id'] ? '' : $form->addRule('parent', $_LABELS['selectOption'],'required');
  if ($_REQUEST['id']) { $soc->frostdata[] = 'name'; } else { $form->removeElement('name'); }
  }
$_parent ? $form->removeElement('pickParent') : '';

if (($_cat == 'BG')&&(!$_id)) { // only valid for new BG type Societies
  $form->addElement('checkbox',  'groups_auto',		$_LABELS['tblSocieties.category.B'],	$_LABELS[groups_auto], 				'checked class=checkbox');
  $form->addElement('select',		'groups_num', 		$_LABELS['qty'], 					array(1=>1,2=>2,3=>3,4=>4,5=>5),     'class=large');
  $form->freeze(array('groups_auto'));
  }

if (is_array($members)) {
  $form->addElement('select',		'president_id', $_LABELS['tblSocieties.president_id'], 	(array('') + $members), 'class=large');
  $form->addElement('select',		'treasurer_id', $_LABELS['tblSocieties.treasurer_id'], 	(array('') + $members), 'class=large');
  $form->addElement('select',		'secretary_id', $_LABELS['tblSocieties.secretary_id'], 	(array('') + $members), 'class=large');
  }
  
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
$form->addRule('president_id',	$_LABELS['selectOption'],'numeric');
$form->addRule('treasurer_id',	$_LABELS['selectOption'],'numeric');
$form->addRule('secretary_id',	$_LABELS['selectOption'],'numeric');
$form->addRule('description',	sprintf($_LABELS['maxlength'],255),'maxlength', 255);

$form->freeze($soc->frostdata);

if ($form->validate()) { 
  
  $form->freeze();
  
  $values = $form->_submitValues;
  
  $values['editor_date']  = date('Y-m-d');
  $values['creator_date'] = $soc->data['creator_date'] ? $soc->data['creator_date'] : $values['editor_date'];
  $values['editor_id']    = WEBPAGE::$userID;
  $values['creator_id']   = $soc->data['creator_id']   ? $soc->data['creator_id']   : $values['editor_id'];
  
  $lastID = SQL::insert('tblSocieties',$values,'id');
  
  if(!($values['id'])) {
    SQL::insert('tblLinkSocieties',array('parent_id'=>$lastID,'child_id'=>$lastID));
    }
  
  // creates a parent-child relashionship for new societies  
  if ($values['parent_id']) {
    SQL::insert('tblLinkSocieties',array('parent_id'=>$values['parent_id'],'child_id'=>$lastID));
    }
    
  // Create groups automatically... if propperly requested
  if ( $values['groups_auto'] && $values['groups_num'] && (!$values['id']) ) {  
  	$parent_name = $values['name'];
    for ($i=0;$i<$values['groups_num'];$i++) {
      $values['category'] = 'B';
      $values['name'] = sprintf($_CONF['group.name.pattern'], $parent_name, $i+1);
      $lastChildID  = SQL::insert('tblSocieties',$values,'id');      
      SQL::insert('tblLinkSocieties',array('parent_id'=>$lastChildID,'child_id'=>$lastChildID));
      SQL::insert('tblLinkSocieties',array('parent_id'=>$lastID,'child_id'=>$lastChildID));
	  }
	}
  
  // In case of BG advisor_id or zone_id change, updates group and clients advisor/zone
  if (($values['category'] == 'BG')&&($values['id'])&&(($soc->data['advisor_id'] != $values['advisor_id'])||($soc->data['zone_id'] != $values['zone_id'])) ) {
    WEBPAGE::$dbh->query(sprintf('update tblSocieties as s set s.zone_id = %s, s.advisor_id = %s where s.id in (select ls.child_id from tblLinkSocieties as ls where ls.parent_id = %s)', $values['zone_id'], $values['advisor_id'], $values['id']));
    WEBPAGE::$dbh->query(sprintf('update tblClients as c set c.zone_id = %s, c.advisor_id = %s where c.society_id in (select ls.child_id from tblLinkSocieties as ls where ls.parent_id = %s)', $values['zone_id'], $values['advisor_id'], $values['id']));
    }  
  
  //update group names
  $soc = new SOCIETY($lastID);
  $soc->nameAutoUpdate();

  WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewSociety&id=%s&ico=scs&msg=scs',$lastID));
  } 

$form->addRule('parent', $_LABELS['selectOption'],'required');

$form->setDefaults($soc->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>