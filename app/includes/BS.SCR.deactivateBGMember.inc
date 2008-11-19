<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/society.php';
require_once 'class/client.php';

// Check for active loans. If so then abort deactivation...
$checkloan = WEBPAGE::$dbh->getAll(sprintf("select lm.id from tblLoansMaster as lm where lm.borrower_id = %s and lm.check_status != 'R' and lm.borrower_type != 'I'",$_REQUEST['child_id']));
!count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewSociety&id=%s&ico=err&msg=moveClient.err.loan',$_REQUEST['child_id']));
$checkloan = WEBPAGE::$dbh->getAll(sprintf("select lm.id from tblLoansMaster as lm, tblLoansMasterDetails as lmd, tblLoansCurrentData as lcd where lmd.master_id = lm.id and lcd.loan_id = lmd.loan_id and lm.borrower_id = %s and lm.check_status = 'R' and lm.borrower_type != 'I'",$_REQUEST['child_id']));
!count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewSociety&id=%s&ico=err&msg=moveClient.err.loan',$_REQUEST['child_id']));

$client 	= new CLIENT();
$bgmember 	= SOCIETY::getTemplateData($_REQUEST['child_id']);
$btn  		= new TTFButton(WEBPAGE::$theme);
$form 		= new HTML_QuickForm('deactivateForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('deactivateForm',$_LABELS['submit']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink(sprintf('index.php?scr_name=BS.SCR.viewSociety&id=%s',$_REQUEST['child_id']),$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'parent_id',	$_REQUEST['parent_id']);
$form->addElement('hidden',		'child_id',		$_REQUEST['child_id']);
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
  $table1  = 'tblClientIOM';
  $fields1 = 'id,type,client_id,society_id,advisor_id,zone_id,date,cause,user_id,memo';
  $tables2 = 'tblClients';
  $params  = sprintf("tblClients.society_id = '%s'",$values['child_id']);
  $fields2 = sprintf("'Null','O',tblClients.id,'%s',tblClients.advisor_id,tblClients.zone_id,'%s','%s','%s','%s'", $values['child_id'],date('Y-m-d'),$values['cause'],WEBPAGE::$userID,$values['memo']);
  SQL::insert_from_select($table1,$fields1,$tables2,$fields2,$params);
  //deactivate clients
  WEBPAGE::$dbh->query(sprintf(SQL::_UPDATE_SQL,'tblClients','zone_id = 0, advisor_id = 0, society_id = 0', 'society_id', $values['child_id']));

  //deactivate group
  $params = sprintf("parent_id=%s AND child_id=%s", $values['parent_id'], $values['child_id']);
  SQL::delete('tblLinkSocieties',$params);
  SQL::insert('tblSocieties',array('id' => $values['child_id'], 'zone_id' => 0, 'advisor_id' => 0, 'editor_id' => WEBPAGE::$userID, 'editor_date' => date('Y-m-d')),'id');

  //update group names
  $soc = new SOCIETY($values['parent_id']);
  $soc->nameAutoUpdate();
  WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewSociety&id=%s&ico=scs&msg=scs',$values['child_id']));
  }

$tpl->setVariable('name',				$bgmember['name']);
$tpl->setVariable('code_label',			$_LABELS['tblSocieties.code']);
$tpl->setVariable('code',				$bgmember['code']);
$tpl->setVariable('bde_label',			$_LABELS['tblSocieties.category.BG']);
$tpl->setVariable('bde',				$bgmember['parent']);
$tpl->setVariable('advisor_label',		$_LABELS['tblSocieties.advisor_id']);
$tpl->setVariable('advisor',			$bgmember['advisor']);
$tpl->setVariable('zone_label',		    $_LABELS['tblSocieties.zone_id']);
$tpl->setVariable('zone',				$bgmember['zone']);

$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>