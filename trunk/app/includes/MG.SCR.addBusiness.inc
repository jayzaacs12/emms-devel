<?php
exit;
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/client.php';
require_once 'class/business_type.php';
require_once 'class/business.php';

$bz = new BUSINESS($_REQUEST['id']);
$ownerID = $_GET['owner_id'] ? $_GET['owner_id'] : $_POST['client_id'];
if ($ownerID) {
  $cl = new CLIENT($ownerID);
  $bz->data[owner1] = $cl->data['code'];
  $bz->frostdata = array('owner1','owner2','owner3','owner4','owner5');
  }

$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('bzForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('bzForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('bzForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'ref',			$_REQUEST['ref']);
$form->addElement('hidden',		'loan_id',		$_REQUEST['loan_id']);
$form->addElement('hidden',		'client_id',	$_REQUEST['owner_id']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);

$form->addElement('text',		'name',			$_LABELS['tblBusiness.name'], 		'class=large');
$form->addElement('select',		'type_id',		$_LABELS['tblBusiness.type_id'],	(array('-'=>'') + $bz->types()), 'class=large');
$form->addElement('text',		'owner1',		$_LABELS['tblBusiness.client_list'], 'class=large');
$form->addElement('text',		'owner2',		'', 'class=large');
$form->addElement('text',		'owner3',		'', 'class=large');
$form->addElement('text',		'owner4',		'', 'class=large');
$form->addElement('text',		'owner5',		'', 'class=large');

$form->addElement('textarea',	'description',	$_LABELS['tblBusiness.description'], 'rows = 8');

$client_list = array();
function checkowner($code = '') {
  global $client_list;
  if (!($code)) { return true; }
  if ( $owner_id = current(SQL::getAssoc('tblClients','code,id',sprintf("code = '%s' AND advisor_id != 0",$code)))) {
    return ($client_list[] = $owner_id);
	} else {
    return false;
    }
  }
//add rules
$form->addRule('name',			sprintf($_LABELS['rangelength'],1,32),'required');
$form->addRule('name',			sprintf($_LABELS['rangelength'],1,32),'rangelength', array(1,32));

$govID = explode('::::::::::',$_CONF['govID']);
$form->addRule('owner1',		$govID[1],'regex',$govID[0]);
$form->addRule('owner1',		$_LABELS['required'].' - '.$_LABELS['tblClients.code'].': '.$govID[1],'required');

$form->addRule('owner1',		'Non valid client','callback','checkowner');
$form->addRule('owner2',		'Non valid client','callback','checkowner');
$form->addRule('owner3',		'Non valid client','callback','checkowner');
$form->addRule('owner4',		'Non valid client','callback','checkowner');
$form->addRule('owner5',		'Non valid client','callback','checkowner');
$form->addRule('name',			$_LABELS['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('type_id',		$_LABELS['selectOption'],'required');
$form->addRule('type_id',		$_LABELS['selectOption'],'alphanumeric');
$form->addRule('description',	sprintf($_LABELS['maxlength'],255),'maxlength', 255);

$form->freeze($bz->frostdata);

if ($form->validate()) {

  $form->freeze();
  $values = $form->_submitValues;

  $values['client_list']	= implode(',',array_unique($client_list));
  $values['editor_date'] 	= date('Y-m-d');
  $values['creator_date'] 	= $bz->data['creator_date'] ? $bz->data['creator_date'] : $values['editor_date'];
  $values['editor_id']   	= WEBPAGE::$userID;
  $values['creator_id']  	= $bz->data['creator_id']   ? $bz->data['creator_id']   : $values['editor_id'];

  $lastID = SQL::insert('tblBusiness',$values,'id');

  if (!($values['id'])) {
    $iom['id'] = 'Null';
    $iom['business_id'] = $lastID;
    $iom['status'] = 'N';
    $iom['creator_date'] = $values['editor_date'];
    $iom['creator_id'] = WEBPAGE::$userID;
    SQL::insert('tblBusinessIOM',$iom);
    }

  WEBPAGE::redirect(sprintf('index.popup.php?scr_name=MG.SCR.browseClients&advisor_id=%s',$cl->data['advisor_id']));

  }
$tpl->setVariable('client_name',$cl->data['name']);
$form->setDefaults($bz->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>