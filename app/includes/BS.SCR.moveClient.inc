<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/coolbutton.php';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';
require_once 'class/society.php';

// Check for active loans. If so then abort moving...
/*
 * este approach evita movimiento con al menos un asociado dentro
 * del grupo origen tiene un prestamo activo o en proceso
$checkloan = WEBPAGE::$dbh->getAll(sprintf("select lm.id from tblLoansMaster as lm, tblClients as c where lm.borrower_id = IF(c.society_id,c.society_id,c.id) and c.id = %s and lm.check_status != 'R'",$_REQUEST['id']));
!count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewClient&id=%s&ico=err&msg=moveClient.err.loan',$_REQUEST['id']));
$checkloan = WEBPAGE::$dbh->getAll(sprintf("select lm.id, lcd.loan_id from tblLoansMaster as lm, tblLoansMasterDetails as lmd, tblClients as c, tblLoansCurrentData as lcd where lm.id = lmd.master_id and lmd.loan_id = lcd.loan_id and lm.borrower_id = IF(c.society_id,c.society_id,c.id) and c.id = %s and lm.check_status = 'R'",$_REQUEST['id']));
!count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewClient&id=%s&ico=err&msg=moveClient.err.loan',$_REQUEST['id']));
*/
// este approach solo verifica si el asociado en particular
// tiene prestamos activos o en proceso
$checkloan = WEBPAGE::$dbh->getAll(sprintf("select lcd.loan_id from tblLoansCurrentData as lcd, tblClients as c, tblLoans as l where lcd.loan_id = l.id and c.id = %s and l.client_id = c.id",$_REQUEST['id']));
!count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewClient&id=%s&ico=err&msg=moveClient.err.loan',$_REQUEST['id']));
$checkloan = WEBPAGE::$dbh->getAll(sprintf("select lm.id from tblLoansMaster as lm, tblLoansMasterDetails as lmd, tblClients as c, tblLoans as l where lm.check_status != 'R' and lm.id = lmd.master_id and lmd.loan_id = l.id and c.id = %s and l.client_id = c.id",$_REQUEST['id']));
!count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewClient&id=%s&ico=err&msg=moveClient.err.loan',$_REQUEST['id']));


$client = new CLIENT($_REQUEST['id']);

if (!( $client->data[id] && $client->data[advisor_id] && $client->data[zone_id] )) {
  WEBPAGE::redirect("index.php?logout=1");
  }

$client->frostdata 			= array('nick','first','middle','last','birthdate','code','spouse','dependants','skills','education','cstatus','gender');

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('clientForm','POST');

$button[id]	    		= "pickSocietybt";
$button[href]			= 'javascript:setSociety()';
$button[alt]			= $_LABELS['pickSociety'];
$button[onClick]		= '';
$button[ico]			= "filter";
$picksocietybutton  	= New COOLBUTTON($button);

$form->addElement('static', 	'pickSociety', 	'', COOLBUTTON::getButton($picksocietybutton));

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('clientForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('clientForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);
$form->addElement('hidden',		'activate',		$_REQUEST['activate']);
$form->addElement('hidden',		'society_id',	$client->data['society_id']);

$form->addElement('text',		'nick',			$_LABELS['tblClients.nick'],		'class=large');
$form->addElement('text',		'first',		$_LABELS['tblClients.first'],		'class=large');
$form->addElement('text',		'middle',		$_LABELS['tblClients.middle'],		'class=large');
$form->addElement('text',		'last',			$_LABELS['tblClients.last'],		'class=large');
$form->addElement('text',		'birthdate',	$_LABELS['tblClients.birthdate'],	'class=large');
$form->addElement('text',		'code',			$_LABELS['tblClients.code'],		'class=large');
$form->addElement('text',		'spouse',		$_LABELS['tblClients.spouse'],		'class=large');
$form->addElement('text',		'dependants',	$_LABELS['tblClients.dependants'],	'class=large');
$form->addElement('text',		'skills',		$_LABELS['tblClients.skills'],		'class=large');
$form->addElement('text',		'society',		$_LABELS['tblClients.society_id'],	'disabled class=large');
$form->addElement('select',		'education',	$_LABELS['tblClients.education'],	(array('-'=>'') + $client->education()),	'class=large');
$form->addElement('select',		'cstatus',		$_LABELS['tblClients.cstatus'],		(array('-'=>'') + $client->cstatus()),		'class=large');
$form->addElement('select',		'zone_id',		$_LABELS['tblClients.zone_id'],		(array('-'=>'') + $client->zones()),		'class=large onChange="javascript:unsetSociety()"');
$form->addElement('select',		'advisor_id',	$_LABELS['tblClients.advisor_id'],	(array('-'=>'') + $client->advisors()),		'class=large onChange="javascript:unsetSociety()"');

$form->addElement('textarea',	'memo',			$_LABELS['tblClients.memo'], 		'class=large');

$gender_radio[] = $form->createElement('radio',null,null,$_LABELS['tblClients.gender.M'],'M', 'class=radio');
$gender_radio[] = $form->createElement('radio',null,null,$_LABELS['tblClients.gender.F'],'F', 'class=radio');
$form->addGroup($gender_radio, 'gender', $_LABELS['tblClients.gender'],' ');

//add rules
$form->addRule('first',			sprintf($_LABELS['rangelength'],1,16),'required');
$form->addRule('first',			sprintf($_LABELS['rangelength'],1,16),'rangelength', array(1,16));
$form->addRule('first',			$_LABELS['lettersonly'],'lettersonly');
$form->addRule('middle',		sprintf($_LABELS['rangelength'],1,16),'rangelength', array(1,16));
$form->addRule('middle',		$_LABELS['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('last',			sprintf($_LABELS['rangelength'],1,32),'required');
$form->addRule('last',			sprintf($_LABELS['rangelength'],1,32),'rangelength', array(1,32));
$form->addRule('last',			$_LABELS['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('birthdate',		'yyyy-mm-dd','required');
$form->addRule('birthdate',		'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('code',			$_LABELS['required'],'required');
$form->addRule('code',			$_CONF['regex_govID_tip'],'regex',$_CONF['regex_govID']);
$form->addRule('spouse',		sprintf($_LABELS['rangelength'],1,32),'rangelength', array(1,32));
$form->addRule('spouse',		$_LABELS['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('gender',		$_LABELS['required'],'required');
$form->addRule('memo',			sprintf($_LABELS['maxlength'],255),'maxlength', 255);
$form->addRule('education',		$_LABELS['selectOption'],'required');
$form->addRule('education',		$_LABELS['selectOption'],'lettersonly');
$form->addRule('cstatus',		$_LABELS['selectOption'],'required');
$form->addRule('cstatus',		$_LABELS['selectOption'],'lettersonly');
$form->addRule('advisor_id',	$_LABELS['selectOption'],'required');
$form->addRule('advisor_id',	$_LABELS['selectOption'],'numeric');
$form->addRule('zone_id',		$_LABELS['selectOption'],'required');
$form->addRule('zone_id',		$_LABELS['selectOption'],'numeric');

$form->freeze($client->frostdata);

if ($form->validate()) {

  $form->removeElement('pickSociety');
  $form->removeElement('submit');
  $form->removeElement('reset');
  $form->removeElement('cancel');

  $form->freeze();

  $values['id'] 			= $form->_submitValues[id];
  $values['advisor_id'] 	= $form->_submitValues[advisor_id];
  $values['zone_id'] 		= $form->_submitValues[zone_id];
  $values['society_id'] 	= $form->_submitValues[society_id];
  $values['editor_date']  	= date('Y-m-d');
  $values['editor_id']    	= WEBPAGE::$userID;

  $lastID = SQL::insert('tblClients',$values,'id');

  // record iom info
  $fields = array('id','internal','type','client_id','society_id','advisor_id','zone_id','date','user_id','memo');
  $data   = sprintf("
                    ('Null','1','O','%s','%s','%s','%s','%s','%s','%s'),
                    ('Null','1','I','%s','%s','%s','%s','%s','%s','%s')",
                     $client->data[id],$client->data[society_id],$client->data[advisor_id],$client->data[zone_id],date('Y-m-d'),WEBPAGE::$userID,$form->_submitValues['memo'],
                     $client->data[id],$values[society_id],$values[advisor_id],$values[zone_id],date('Y-m-d'),WEBPAGE::$userID,$form->_submitValues['memo']);

  SQL::insert_mult('tblClientIOM',$fields,$data);

  WEBPAGE::redirect(sprintf("index.php?scr_name=%s&id=%s",'BS.SCR.viewClient',$lastID));

  }

$form->setDefaults($client->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>