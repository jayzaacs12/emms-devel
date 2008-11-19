<?php
exit;
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/coolbutton.php';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';
require_once 'class/society.php';

$client = new CLIENT();

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('pickForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('pickForm',$_LABELS['submit']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('select',		'advisor_id',	$_LABELS['tblClients.advisor_id'],	(array('-'=>'') + $client->advisors()),		'class=large');

$form->addRule('advisor_id',	$_LABELS['selectOption'],'required');
$form->addRule('advisor_id',	$_LABELS['selectOption'],'numeric');

if ($form->validate()) {

  WEBPAGE::redirect(sprintf('index.popup.php?scr_name=MG.SCR.browseClients&advisor_id=%s',$_POST['advisor_id']));

  }

$form->setDefaults($client->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>