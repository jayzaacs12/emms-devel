<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/roles.php';
require_once 'class/client.php';

// Check for active loans. If so then abort moving...
$checkloan = WEBPAGE::$dbh->getAll(sprintf("select lm.id from tblLoansMaster as lm, tblClients as c where lm.borrower_id = IF(c.society_id,c.society_id,c.id) and c.id = %s and lm.check_status != 'R' and lm.borrower_type='I'",$_REQUEST['id']));
!count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewClient&id=%s&ico=err&msg=moveClient.err.loan',$_REQUEST['id']));
$checkloan = WEBPAGE::$dbh->getAll(sprintf("select lm.id, lcd.loan_id from tblLoansMaster as lm, tblLoansMasterDetails as lmd, tblClients as c, tblLoansCurrentData as lcd where lm.id = lmd.master_id and lmd.loan_id = lcd.loan_id and lm.borrower_id = IF(c.society_id,c.society_id,c.id) and c.id = %s and lm.check_status = 'R' and lm.borrower_type='I'",$_REQUEST['id']));
!count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewClient&id=%s&ico=err&msg=moveClient.err.loan',$_REQUEST['id']));

$client = new CLIENT();
$btn  	= new TTFButton(WEBPAGE::$theme);

$form 	= new HTML_QuickForm('deactivateForm','POST');

extract(CLIENT::getTemplateData($_REQUEST['id']));
$form->addElement('static', 	'name', 	'', $name);
$form->addElement('static', 	'img', 		'', $img_path);

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('deactivateForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('deactivateForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink(sprintf("%s?scr_name=%s&id=%s",$PHP_SELF,'BS.SCR.viewClient',$_REQUEST[id]),$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name', $_REQUEST['scr_name']);
$form->addElement('hidden',		'id',       $_REQUEST['id']);
$form->addElement('select',		'cause',  	$_LABELS['tblClientIOM.cause'],	(array('') + $client->iomcauses()), 'class=large');
$form->addElement('textarea',	'memo',		$_LABELS['tblClientIOM.memo'], 'rows=8');

//add rules
$form->addRule('cause',			$_LABELS['selectOption'],'required');
$form->addRule('cause',			$_LABELS['selectOption'],'lettersonly');
$form->addRule('memo',			sprintf($_LABELS['maxlength'],255),'maxlength', 255);

if ($form->validate()) { 

  $form->freeze();
  $values = $form->_submitValues;

  // save client movement into log
  WEBPAGE::$dbh->query(sprintf('insert into tblClientIOM (type,client_id,society_id,advisor_id,zone_id,date,cause,user_id,memo) select "O",c.id,c.society_id,c.advisor_id,c.zone_id,"%s","%s","%s","%s" from tblClients as c where c.id = "%s"',date('Y-m-d'),$values['cause'],WEBPAGE::$userID,$values['memo'],$values['id']));
  //deactivate clients
  WEBPAGE::$dbh->query(sprintf('update tblClients set zone_id = 0, advisor_id = 0, society_id = 0, editor_id = %s, editor_date = "%s" where id = %s', WEBPAGE::$userID, date('Y-m-d'), $values['id']));

  WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewClient&id=%s&ico=scs&msg=scs',$values['id']));

  } 

$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>