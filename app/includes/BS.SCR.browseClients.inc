<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';

$client = new CLIENT();

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 	'submit', 	'', WEBPAGE::$btn->imgsubmit('searchForm',$_LABELS['search']));
$form->addElement('static', 	'reset', 	'', WEBPAGE::$btn->imgreset('searchForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', WEBPAGE::$btn->imglink($PHP_SELF,$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'ref',			$_REQUEST['ref']);

$form->addElement('text',		'code',		    $_LABELS['tblClients.code'], 		'class=large');
$form->addElement('text',		'first',		$_LABELS['tblClients.first'], 		'class=large');
$form->addElement('text',		'last',		    $_LABELS['tblClients.last'], 		'class=large');
$form->addElement('select',		'zone_id',		$_LABELS['tblClients.zone_id'],		($zones = array('') +  $client->zones()), 		'class=large');
$form->addElement('select',		'advisor_id',	$_LABELS['tblClients.advisor_id'],	($advisors = array('') +  $client->advisors()), 'class=large');
$form->addElement('select',		'options',	$_LABELS['status'],	(array('') +  array('IC'=>$_LABELS['inactiveClient'],'AL'=>$_LABELS['activeWithLoan'],'NAL'=>$_LABELS['activeWithoutLoan'])), 'class=large');


if ($form->validate()) {

  $values = $form->_submitValues;

  $extra['param']['value']['code']	    				= $values['code'];
  $extra['param']['value']['zone_id']	    			= $values['zone_id'];
  $extra['param']['value']['advisor_id']				= $values['advisor_id'];
  $extra['param']['value']['first']	    				= $values['first'];
  $extra['param']['value']['last']	    				= $values['last'];

  $extra['param']['label']['tblClients.code']	    	= $values['code'] 		? $values['code'] 			       : $_LABELS['all'];
  $extra['param']['label']['tblClients.zone_id']	    = $values['zone_id'] 	? $zones[$values['zone_id']]       : $_LABELS['all'];
  $extra['param']['label']['tblClients.advisor_id']		= $values['advisor_id']	? $advisors[$values['advisor_id']] : $_LABELS['all'];
  $extra['param']['label']['tblClients.first']	    	= $values['first']		? $values['first'] 			       : $_LABELS['all'];
  $extra['param']['label']['tblClients.last']	    	= $values['last']		? $values['last'] 			       : $_LABELS['all'];

  $extra['param']['ref']								= WEBPAGE::$scr_name;

  $fields[] 					= "CONCAT(c.first,' ', c.last) AS name";
  $extra['name']['header']		= 'tblClients.id';
  $extra['name']['pack']		= '%s';
  $extra['name']['align']		= 'left';

  $fields[] 					= "c.code";
  $extra['code']['header']		= 'tblClients.code';
  $extra['code']['pack']		= '%s';
  $extra['code']['align']		= 'center';

  $fields[] 					= "s.name as society";
  $extra['society']['header']	= 'tblClients.society_id';
  $extra['society']['pack']		= '%s';
  $extra['society']['align']	= 'left';

  $fields[] 					= "lc.lc_date as lcdate";
  $extra['lcdate']['header']	= 'last_cancel_date';
  $extra['lcdate']['pack']		= '%s';
  $extra['lcdate']['align']		= 'center';

  $fields[] 					= 'c.id as view';
  $extra['view']['header']		= 'view';
  $extra['view']['pack']		= sprintf("<a href='%s?scr_name=BS.SCR.viewClient&ref=BS.SCR.browseClients&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['view']);
  $extra['view']['align']		= 'center';

  $fields[] 					= 'c.id as edit';
  $extra['edit']['header']		= 'edit';
  $extra['edit']['pack']		= sprintf("<a href='%s?scr_name=BS.SCR.addClient&ref=BS.SCR.editClient&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['edit']);
  $extra['edit']['align']		= 'center';

  if ($applySurvey) {
    $fields[] 							= 'c.id as applySurvey';
    $extra['applySurvey']['header']		= 'applySurvey';
    $extra['applySurvey']['pack']		= sprintf("<a href='%s?scr_name=SV.SCR.browseSurveys&ref=SV.SCR.applySurveyStepA&client_id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['applySurvey']);
    $extra['applySurvey']['align']		= 'center';
    }

  if ($_REQUEST[ref] == 'LN.SCR.requestILoan') {
    $fields[] 							= 'c.id as requestLoan';
    $extra['requestLoan']['header']		= 'requestLoan';
    $extra['requestLoan']['pack']		= sprintf("<a href='%s?scr_name=LN.SCR.requestILoan&client_id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['requestLoan']);
    $extra['requestLoan']['align']		= 'center';
    }

  // Constructing query operators

  if ( $values['zone_id'] )   	{ $operator_zone_id  	= '='; } else { $operator_zone_id  	  = 'LIKE'; $values['zone_id']    = '%%'; }
  if ( $values['code'] )   		{ $operator_code  	    = '='; } else { $operator_code  	  = 'LIKE'; $values['code']  	  = '%%'; }
  if ( $values['advisor_id'] )	{ $operator_advisor_id  = '='; } else { $operator_advisor_id  = 'LIKE'; $values['advisor_id'] = '%%'; }

  $params = sprintf("		c.code 			%s 		'%s'
							AND c.first 		LIKE 	'%%%s%%'
							AND c.last 			LIKE 	'%%%s%%'
							AND c.zone_id 		%s 		'%s'
							AND c.advisor_id 	%s 		'%s'",
							$operator_code, $values['code'],
							$values['first'],
							$values['last'],
							$operator_zone_id, $values['zone_id'],
							$operator_advisor_id, $values['advisor_id'], WEBPAGE::$userID);

  switch ($values['options']) {
    case 'AL':
      $mrow = WEBPAGE::$dbh->getAll(sprintf("select %s from tblClients c left join (SELECT lc_l.client_id lc_client_id,max(lc_lsh.date) lc_date FROM tblLoans lc_l left join tblLoanStatusHistory lc_lsh on lc_lsh.loan_id = lc_l.id and lc_lsh.status = 'C' group by lc_l.client_id) lc on lc.lc_client_id = c.id left join tblSocieties s on s.id = c.society_id where %s and c.id in (select l.client_id from tblLoans as l, tblLoansCurrentData as lcd where l.id = lcd.loan_id)",implode(',',$fields),$params));
      break;
    case 'NAL':
      $mrow = WEBPAGE::$dbh->getAll(sprintf("select %s from tblClients c left join (SELECT lc_l.client_id lc_client_id,max(lc_lsh.date) lc_date FROM tblLoans lc_l left join tblLoanStatusHistory lc_lsh on lc_lsh.loan_id = lc_l.id and lc_lsh.status = 'C' group by lc_l.client_id) lc on lc.lc_client_id = c.id left join tblSocieties s on s.id = c.society_id where %s and c.advisor_id > '0' and c.id not in (select l.client_id from tblLoans as l, tblLoansCurrentData as lcd where l.id = lcd.loan_id)",implode(',',$fields),$params));
      break;
    case 'IC':
      $mrow = WEBPAGE::$dbh->getAll(sprintf("select %s from tblClients c left join (SELECT lc_l.client_id lc_client_id,max(lc_lsh.date) lc_date FROM tblLoans lc_l left join tblLoanStatusHistory lc_lsh on lc_lsh.loan_id = lc_l.id and lc_lsh.status = 'C' group by lc_l.client_id) lc on lc.lc_client_id = c.id left join tblSocieties s on s.id = c.society_id where %s and c.id in (select nc.id from tblClients nc where nc.advisor_id = 0)",implode(',',$fields),$params));
      break;
    default:
      $mrow = WEBPAGE::$dbh->getAll(sprintf("select %s from tblClients c left join (SELECT lc_l.client_id lc_client_id,max(lc_lsh.date) lc_date FROM tblLoans lc_l left join tblLoanStatusHistory lc_lsh on lc_lsh.loan_id = lc_l.id and lc_lsh.status = 'C' group by lc_l.client_id) lc on lc.lc_client_id = c.id left join tblSocieties s on s.id = c.society_id where %s",implode(',',$fields),$params));
    }

  foreach($mrow as $key => $row) {
    $client->data['id'] = $row['view'];
    $client->checkConfidentiality();
    if ($client->data[confidential]) {
      $mrow[$key]['name'] = $client->data['name'];
      $mrow[$key]['code'] = $client->data['code'];
      }
    }
  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);
  require_once 'BS.SCR.pager.inc';

  } else {

  $form->setDefaults();
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $tpl->setVariable('content_title',$_LABELS[WEBPAGE::$scr_name]);
  $form->accept($renderer);

  }
?>
