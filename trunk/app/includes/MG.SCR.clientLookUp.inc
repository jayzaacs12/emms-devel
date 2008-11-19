<?php
exit;
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 	'submit', 	'', WEBPAGE::$btn->imgsubmit('searchForm',$_LABELS['search']));
$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);

$form->addElement('text',		'code',		    'Cédula', 		'class=large');

if ($form->validate()) {

  $values = $form->_submitValues;

  if ( $id = current(WEBPAGE::$dbh->getAssoc(sprintf("select code,id from tblClients where code = '%s' and zone_id != 0 and advisor_id != 0", $values['code'])))) {
  	WEBPAGE::redirect(sprintf('index.popup.php?scr_name=MG.SCR.addLoan&client_id=%s',$id));
    } else {
    $tpl->setVariable('message','Cédula no válida o asociado no activo');
    }
  }

$form->setDefaults();
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$tpl->setVariable('content_title','Entrar cédula para verificar asociado');
$form->accept($renderer);

?>