<?php
require_once 'HTML/Template/ITX.php';
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once '../class/TTFButton.php';

$btn = new TTFButton() ;

$form = new HTML_QuickForm('loginForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('loginForm',$_LABELS['submit']));
$form->addElement('text',		  'username',		  $_LABELS['tblUsers.username'], 'class=qform');
$form->addElement('password',	'password',		  $_LABELS['tblUsers.password'], 'class=qform');
$form->addElement('hidden',		'screenWidth',	'');
$form->addElement('hidden',		'lang',			    WEBPAGE::$lang);


$form->addRule('username',$_LABELS['MSG.WRN.078'],'required');

$tpl = new HTML_Template_ITX('./templates');
$tpl->loadTemplateFile('login.tpl');

$tpl->setVariable('html_title_client_name',	    strip_tags($_CONF['client_name']));
$tpl->setVariable('client_name',	              $_CONF['client_name']);
$tpl->setVariable('sponsorship_program_name',		$_CONF['sponsorship_program_name']);
$tpl->setVariable('message',		                $_LABELS[$message]);
$tpl->setVariable('copyright',		              $_CONF['copyright']);
$tpl->setVariable('contact',		                $_CONF['contact_info']);

$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(_REQUIRED_FIELD);
$renderer->setErrorTemplate(_FIELD_ERROR);

$form->accept($renderer);
$tpl->show();
?>