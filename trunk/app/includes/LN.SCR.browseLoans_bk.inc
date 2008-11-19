<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';
require_once 'class/user.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';

if ($_GET[loan_code]) {
  $_POST[loan_code] 		= $_GET[loan_code];
  $_POST[status] 			= '%%';
  $_POST[zone_id] 			= '%%';
  $_POST[borrower_type] 	= '%%';
  $_POST[advisor_id] 		= '%%';
  $_POST[submit] 			= true;
  }

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 	'submit', 	'', WEBPAGE::$btn->imgsubmit('searchForm',$_LABELS['search']));
$form->addElement('static', 	'reset', 	'', WEBPAGE::$btn->imgreset('searchForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', WEBPAGE::$btn->imglink($PHP_SELF,$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);

$form->addElement('text',		'loan_code',	$_LABELS['tblLoans.loan_code'], 		'class=large');
$form->addElement('text',		'first',		$_LABELS['tblClients.first'], 			'class=large');
$form->addElement('text',		'last',		    $_LABELS['tblClients.last'], 			'class=large');
$form->addElement('select',		'status',		$_LABELS['tblLoans.status'],			($status 			= array('%%'=>'') + LOAN::status()), 				'class=large');
$form->addElement('select',		'zone_id',		$_LABELS['tblUsers.zone_id'],			($zones 			= array('%%'=>'') + USER::zones()), 				'class=large');
// PATCH: 2008.07.19
$form->addElement('select',		'advisor_id',	$_LABELS['tblLoans.advisor_id'],		($advisors 			= array('%%'=>'') + CLIENT::advisors()), 			'class=large');
$form->addElement('select',		'borrower_type',$_LABELS['tblLoanTypes.borrower_type'],	($borrower_types 	= array('%%'=>'') + LOAN_TYPE::borrower_types()), 	'class=large');



if ($form->validate()) {

  $values = $form->_submitValues;

  if ($values[status] 			== '%%') { $operator_status 		= 'LIKE'; } else { $operator_status 		= '='; }
  if ($values[zone_id] 			== '%%') { $operator_zone_id 		= 'LIKE'; } else { $operator_zone_id 		= '='; }
  if ($values[borrower_type] 	== '%%') { $operator_borrower_type 	= 'LIKE'; } else { $operator_borrower_type 	= '='; }
  if ($values[advisor_id] 		== '%%') { $operator_advisor_id 	= 'LIKE'; } else { $operator_advisor_id 	= '='; }

  $fields[] 					= 'l.loan_code';
  $extra['loan_code']['header']	= 'tblLoans.loan_code';
  $extra['loan_code']['pack']	= '%s';
  $extra['loan_code']['align']	= 'center';

  $fields[] 					= 'l.status';
  $extra['status']['header']	= 'status';
  $extra['status']['pack']		= 'tblLoans.status.%s';
  $extra['status']['align']		= 'left';

  $fields[] 					= 'concat(c.first, " ", c.last) as client';
  $extra['client']['header']	= 'tblClients.id';
  $extra['client']['pack']		= '%s';
  $extra['client']['align']		= 'left';

  $fields[] 					= 'if((lt.borrower_type != "I"),s.name,"-") society'; // 's.name as society';
  $extra['society']['header']	= 'tblClients.society_id';
  $extra['society']['pack']		= '%s';
  $extra['society']['align']	= 'left';

 // PATCH: 2008.07.19
  if ($values['status'] == 'G' || $values['status'] == 'LI' || $values['status'] == 'LO' || $values['status'] == 'C') {

  $fields[] 					= 'l.kp as amount';
  $extra['amount']['header']	= 'tblLoans.kp';
  $extra['amount']['pack']		= '%s';
  $extra['amount']['align']		= 'left';

  $fields[] 					    = 'u.username as username';
  $extra['username']['header']	    = 'tblLoans.advisor_id';
  $extra['username']['pack']		= '%s';
  $extra['username']['align']		= 'left';

  $fields[] 					    = 'p.program as program';
  $extra['program']['header']	    = 'tblPrograms.program';
  $extra['program']['pack']		    = '%s';
  $extra['program']['align']		= 'left';

  $fields[] 					= 'l.xp_cancel_date as date';
  $extra['date']['header']	    = 'tblLoans.xp_cancel_date';
  $extra['date']['pack']		= '%s';
  $extra['date']['align']		= 'center';

  }

  $fields[] 					= 'l.id as view';
  $extra['view']['header']		= 'view';
  $extra['view']['pack']		= sprintf("<a href='%s?scr_name=LN.SCR.viewILoan&ref=LN.SCR.browseLoans&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['view']);
  $extra['view']['align']		= 'center';

  $fields[] 					= 'l.id as edit';
  $extra['edit']['header']		= 'edit';
  $extra['edit']['pack']		= sprintf("<a href='%s?scr_name=LN.SCR.addILoan&ref=LN.SCR.requestILoan&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['edit']);
  $extra['edit']['align']		= 'center';


  $extra['param']['value']['loan_code']						= $values['loan_code'];
  $extra['param']['value']['first']	    					= $values['first'];
  $extra['param']['value']['last']	    					= $values['last'];
  $extra['param']['value']['status']						= $values['status'];
  $extra['param']['value']['zone_id']	    				= $values['zone_id'];
  $extra['param']['value']['advisor_id']					= $values['advisor_id'];
  $extra['param']['value']['borrower_type']					= $values['borrower_type'];

  $extra['param']['label']['tblLoans.loan_code']			= $values['loan_code'] 										? $values['loan_code'] 										: '-';
  $extra['param']['label']['tblClients.first']	    		= $values['first']		? $values['first'] 			       : $_LABELS['all'];
  $extra['param']['label']['tblClients.last']	    		= $values['last']		? $values['last'] 			       : $_LABELS['all'];
  $extra['param']['label']['tblLoans.status']				= $_LABELS[sprintf("tblLoans.status.%s",$values['status'])] ? $_LABELS[sprintf("tblLoans.status.%s",$values['status'])]	: $_LABELS['all'];
  $extra['param']['label']['tblClients.zone_id']			= $zones[$values['zone_id']]								? $zones[$values['zone_id']]						      	: $_LABELS['all'];
  $extra['param']['label']['tblClients.advisor_id']			= $advisors[$values['advisor_id']] 							? $advisors[$values['advisor_id']] 							: $_LABELS['all'];
  $extra['param']['label']['tblLoanTypes.borrower_type']	= $borrower_types[$values['borrower_type']]					? $borrower_types[$values['borrower_type']]					: $_LABELS['all'];

  $extra['param']['ref']									= WEBPAGE::$scr_name;

  $tables = "tblLoans AS l, tblUsers u, tblPrograms p, tblZones AS z, tblLoanTypes AS lt, tblClients as c left join tblSocieties s on s.id = c.society_id";
  $params = sprintf("l.loan_code LIKE '%%%s%%'
						AND c.first LIKE 	'%%%s%%'
						AND c.last 	LIKE 	'%%%s%%'
		    			AND l.status %s '%s'
		    			AND l.zone_id %s '%s'
		    			AND lt.borrower_type %s '%s'
		    			AND ( (l.editor_id %s '%s') OR (l.creator_id %s '%s'))
		    			AND l.advisor_id = u.id
		    			AND l.program_id = p.id
		    			AND l.zone_id = z.id
		    			AND l.loan_type_id = lt.id
		    			AND c.id = l.client_id",
							$values[loan_code],
							$values['first'],
							$values['last'],
							$operator_status, $values[status],
							$operator_zone_id, $values[zone_id],
							$operator_borrower_type, $values[borrower_type],
							$operator_advisor_id, $values[advisor_id], $operator_advisor_id, $values[advisor_id]);

  $mrow = SQL::select($tables,$fields,$params);


//  foreach($mrow as $key => $val) {
//    $mrow[$key][status] 		= $_LABELS[sprintf("tblLoans.status.%s",$val[status])];
//	}
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