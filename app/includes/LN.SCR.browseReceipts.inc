<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/roles.php';
require_once 'class/persons.php';
//require_once 'class/client.php';
require_once 'class/user.php';
//require_once 'class/loan_type.php';
//require_once 'class/loan.php';
//require_once 'class/loan_master.php';

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 	'submit', 	'', WEBPAGE::$btn->imgsubmit('searchForm',$_LABELS['search']));
$form->addElement('static', 	'reset', 	'', WEBPAGE::$btn->imgreset('searchForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', WEBPAGE::$btn->imglink($PHP_SELF,$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);

$form->addElement('text',		'id',				$_LABELS['code'], 	'class=large');
$form->addElement('select',		'zone_id',			$_LABELS['tblUsers.zone_id'],				($zones 			= array('%%'=>'') + USER::zones()), 				'class=large');
$form->addElement('text',		'creator_date_1',	$_LABELS['date.after'], 					'class=large');
$form->addElement('text',		'creator_date_2',	$_LABELS['date.before'], 					'class=large');

$form->addRule('creator_date_1',	'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('creator_date_2',	'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');

if ($form->validate()) {

  $values = $form->_submitValues;

  if ($values['zone_id'] 		== '%%') { $operator_zone_id 		= 'LIKE'; } else { $operator_zone_id	= '='; }
  if ($values['id'] 		    == '')   { $operator_id 			= 'LIKE'; } else { $operator_id	 		= '='; }

  if ($values['creator_date_1'] == '')   { $values['creator_date_1'] 	= '1000-01-01'; }
  if ($values['creator_date_2'] == '')   { $values['creator_date_2'] 	= '9999-01-01'; }

  $fields[] 						= 'r.id as id';
  $extra['id']['header'] 			= 'code';
  $extra['id']['pack']	 			= '%s';
  $extra['id']['align']	 			= 'center';

  $fields[] 						= 'sum(pmt.pmt) amount';
  $extra['amount']['header'] 		= 'PMT';
  $extra['amount']['pack']	 		= '%s';
  $extra['amount']['align']	 		= 'right';

  $fields[] 						= 'z.zone as zone_id';
  $extra['zone_id']['header']		= 'tblUsers.zone_id';
  $extra['zone_id']['pack']			= '%s';
  $extra['zone_id']['align']		= 'left';

  $fields[] 						= 'p.program as program_id';
  $extra['program_id']['header']	= 'tblPrograms.program';
  $extra['program_id']['pack']		= '%s';
  $extra['program_id']['align']		= 'left';

  $fields[] 						= 'r.date as date';
  $extra['date']['header']			= 'tblPrograms.program';
  $extra['date']['pack']			= '%s';
  $extra['date']['align']			= 'left';

  $fields[] 						= 'r.id as view';
  $extra['view']['header']			= 'view';
  $extra['view']['pack']			= sprintf('<a href=\'javascript:openWin("index.popup.php?scr_name=LN.SCR.viewReceipt&id=%s","ViewReceipt","menubar=no,scrollbars=yes,resizable=yes,width=700,height=380")\'>%s</a>' ,'%s',$_LABELS['view']);
  $extra['view']['align']			= 'center';

  $extra['param']['value']['id']							= $values['id'];
  $extra['param']['value']['zone_id']						= $values['zone_id'];
  $extra['param']['value']['creator_date_1']				= $values['creator_date_1'];
  $extra['param']['value']['creator_date_1']				= $values['creator_date_2'];

  $extra['param']['label']['code']							= $values['id'] 			  ? $values['id']    : '-';
  $extra['param']['label']['tblUsers.zone_id']				= $zones[$values['zone_id']]  ? $zones[$values['zone_id']] : $_LABELS['all'];
  $extra['param']['label']['date.before']					= $values['creator_date_2'];
  $extra['param']['label']['date.after']					= $values['creator_date_1'];

  $extra['param']['ref']									= WEBPAGE::$scr_name;

  $mrow = WEBPAGE::$dbh->getAll(sprintf(
  		"select
  		   r.id, sum(pmt.pmt) amount, z.zone zone_id, p.program program_id, r.date, r.id view
  		 from
  		   tblReceipts r, tblLoansMaster lm, tblZones z, tblPrograms p, tblLinkReceiptsPayments rp, tblPayments pmt
  		 where
  		   r.id %s '%s' and
  		   r.date >= '%s' and
  		   r.date <= '%s' and
  		   lm.zone_id %s '%s' and
  		   lm.id = r.loanmaster_id and
  		   z.id = lm.zone_id and
  		   p.id = lm.program_id and
  		   rp.receipt_id = r.id and
  		   pmt.id = rp.payment_id
  		 group by
  		   r.id",$operator_id,$values['id'] ? $values['id'] : '%%',$values['creator_date_1'],$values['creator_date_2'],$operator_zone_id, $values['zone_id']));

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

