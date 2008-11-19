<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/society.php';
require_once 'class/client.php';

$bg		 	= SOCIETY::getTemplateData($_REQUEST['id']);

// Check for active loans. If so then abort deactivation...
$bgmemberstocheckforloans = array_keys($bg['members']);
foreach ($bgmemberstocheckforloans as $key=>$val) {
  $checkloan = WEBPAGE::$dbh->getAll(sprintf("select lm.id from tblLoansMaster as lm where lm.borrower_id = %s and lm.check_status != 'R' and lm.borrower_type != 'I'",$val));
  !count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewSociety&id=%s&ico=err&msg=moveClient.err.loan',$_REQUEST['id']));
  $checkloan = WEBPAGE::$dbh->getAll(sprintf("select lm.id from tblLoansMaster as lm, tblLoansMasterDetails as lmd, tblLoansCurrentData as lcd where lmd.master_id = lm.id and lcd.loan_id = lmd.loan_id and lm.borrower_id = %s and lm.check_status = 'R' and lm.borrower_type != 'I'",$val));
  !count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewSociety&id=%s&ico=err&msg=moveClient.err.loan',$_REQUEST['id']));
  }

$client 	= new CLIENT();
$btn  		= new TTFButton(WEBPAGE::$theme);
$form 		= new HTML_QuickForm('deactivateForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('deactivateForm',$_LABELS['submit']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink(sprintf('index.php?scr_name=BS.SCR.viewSociety&id=%s',$_REQUEST['child_id']),$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);
$form->addElement('hidden',		'ico',			$_REQUEST['ico']);
$form->addElement('hidden',		'msg',			$_REQUEST['msg']);
$form->addElement('select',		'cause',  		$_LABELS['tblClientIOM.cause'],	(array('') + $client->iomcauses()), 'class=large');
$form->addElement('textarea',	'memo',			$_LABELS['tblClientIOM.memo'], 'rows=8');

//add rules
$form->addRule('cause',			$_LABELS['selectOption'],'required');
$form->addRule('cause',			$_LABELS['selectOption'],'lettersonly');
$form->addRule('memo',			sprintf($_LABELS['maxlength'],255),'maxlength', 255);

if ($form->validate()) { 

  $values = $form->_submitValues;
  
  // save client movement into log
  WEBPAGE::$dbh->query(sprintf('insert into tblClientIOM (type,client_id,society_id,advisor_id,zone_id,date,cause,user_id,memo)  select "O",c.id,c.society_id,c.advisor_id,c.zone_id,"%s","%s","%s","%s" from tblClients as c where c.society_id in (select ls.child_id from tblLinkSocieties as ls where ls.parent_id = %s)', date('Y-m-d'),$values['cause'],WEBPAGE::$userID,$values['memo'],$values['id']));
  //deactivate clients, groups
  WEBPAGE::$dbh->query(sprintf('update tblClients as c set c.zone_id = 0, c.advisor_id = 0, c.society_id = 0, c.editor_id = %s, c.editor_date = "%s" where c.society_id in (select ls.child_id from tblLinkSocieties as ls where ls.parent_id = %s)',WEBPAGE::$userID, date('Y-m-d'), $values['id']));
  WEBPAGE::$dbh->query(sprintf('update tblSocieties as s set s.zone_id = 0, s.advisor_id = 0, s.editor_id = %s, s.editor_date = "%s" where s.id in (select ls.child_id from tblLinkSocieties as ls where ls.parent_id = %s)',WEBPAGE::$userID, date('Y-m-d'), $values['id']));
  // un-link parent/child societies
  $param = implode(',',WEBPAGE::$dbh->getAssoc(sprintf('select child_id, child_id as id from tblLinkSocieties where parent_id = %s', $values['id'])));
  WEBPAGE::$dbh->query(sprintf('delete from tblLinkSocieties WHERE FIND_IN_SET(parent_id,"%s")', $param));
 
  WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewSociety&id=%s&ico=scs&msg=scs',$values['id']));
  }

$tpl->setVariable('name',				$bg['name']);
$tpl->setVariable('code_label',			$_LABELS['tblSocieties.code']);
$tpl->setVariable('code',				$bg['code']);
$tpl->setVariable('advisor_label',		$_LABELS['tblSocieties.advisor_id']);
$tpl->setVariable('advisor',			$bg['advisor']);
$tpl->setVariable('zone_label',		    $_LABELS['tblSocieties.zone_id']);
$tpl->setVariable('zone',				$bg['zone']);

$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>