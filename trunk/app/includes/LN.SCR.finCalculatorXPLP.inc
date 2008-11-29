<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';

$data = WEBPAGE::$dbh->getAll('select id,borrower_type,description from tblLoanTypes order by borrower_type');
$loantypes = array();
foreach($data as $key=>$val) {
  $loantypes[$val['id']] = sprintf('%s : %s',$_LABELS[sprintf('tblLoanTypes.borrower_type.%s',$val['borrower_type'])],$val['description']);
  }

$form = new HTML_QuickForm('calForm','POST','index.popup.php','finCalculatorX');

$form->addElement('hidden',		'scr_name',		'LN.SCR.finCalculatorX');
$form->addElement('select',		'id',			$_LABELS['tblLoanTypes.description'],	$loantypes, 'onchange="document.forms[0].submit()" style="width:320px" size=4');

$form->setDefaults();
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>
