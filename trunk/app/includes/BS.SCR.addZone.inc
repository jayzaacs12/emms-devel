<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/zone.php';

$zone = new ZONE($_REQUEST['id']);
$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('zoneForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('zoneForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('zoneForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden',		'scr_name',			$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',				$_REQUEST['id']);

$form->addElement('text',		'zone',				$_LABELS['tblZones.zone'], 			'class=large');
$form->addElement('text',		'short_name',		$_LABELS['tblZones.short_name'], 	'class=large');
$form->addElement('text',		'zA',				$_LABELS['tblZones.zA'], 			'class=large');
$form->addElement('text',		'zB',				$_LABELS['tblZones.zB'], 			'class=large');
$form->addElement('text',		'zC',				$_LABELS['tblZones.zC'], 			'class=large');

$form->addElement('select',		'program_id',  		$_LABELS['tblPrograms.id'],	$zone->programs(),			 	'multiple size="5" class=large');
$form->addElement('static',		'current_programs',	'', 								implode('<br>',$zone->getPrograms()));

$form->addElement('textarea',	'memo',		$_LABELS['tblZones.memo']);

//add rules
$form->addRule('zone',			sprintf($_LABELS['rangelength'],1,32),		'required');
$form->addRule('zone',			sprintf($_LABELS['rangelength'],1,32),		'rangelength', array(1,32));
$form->addRule('zone',			$_LABELS['lettersonly'],					'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('short_name',	sprintf($_LABELS['rangelength'],1,3),		'required');
$form->addRule('short_name',	$_LABELS['lettersonly'],					'lettersonly');
$form->addRule('short_name',	sprintf($_LABELS['rangelength'],1,3),		'rangelength', array(1,3));
$form->addRule('zA',			sprintf($_LABELS['rangelength'],1,32),		'rangelength', array(1,32));
$form->addRule('zA',			$_LABELS['lettersonly'],					'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('zB',			sprintf($_LABELS['maxlength'],255),			'maxlength', 32);
$form->addRule('zB',			$_LABELS['lettersonly'],					'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('zC',			sprintf($_LABELS['maxlength'],255),			'maxlength', 32);
$form->addRule('zC',			$_LABELS['lettersonly'],					'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('memo',			sprintf($_LABELS['maxlength'],255),			'maxlength', 255);

if ($form->validate()) { 
  
  $form->freeze();
  
  $values = $form->_submitValues;
  
  $values['editor_date']  = date('Y-m-d');
  $values['creator_date'] = $zone->zonedata['creator_date'] ? $zone->zonedata['creator_date'] : $values['editor_date'];
  $values['editor_id']    = WEBPAGE::$userID;
  $values['creator_id']   = $zone->zonedata['creator_id']   ? $zone->zonedata['creator_id']   : $values['editor_id'];
  
  $program_id = $values['program_id'];
  unset($values['program_id']);

  $lastID = SQL::insert('tblZones',$values,'id');
  
  if (is_array($program_id)) {
    $comma = ' ';
    $data = '';
	for ($i=0;$i<count($program_id);$i++) {
	  $data .= sprintf("%s(%s,%s,%s,%s,'%s')", $comma, 'Null',$lastID,$program_id[$i],$values['editor_id'],$values['editor_date']);
	  $comma = ',';
	  }
	$fields = 'id,parent_id,program_id,creator_id,creator_date';
    SQL::insert_mult('tblZones',$fields, $data);
    }

  WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewZone&id=%s&ico=scs&msg=scs',$lastID));

  } 

$form->setDefaults($zone->zonedata);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>