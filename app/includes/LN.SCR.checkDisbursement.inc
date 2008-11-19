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
require_once 'class/loan_master.php';

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 	'submit', 	'', WEBPAGE::$btn->imgsubmit('searchForm',$_LABELS['search']));
$form->addElement('static', 	'reset', 	'', WEBPAGE::$btn->imgreset('searchForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', WEBPAGE::$btn->imglink($PHP_SELF,$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);

$form->addElement('select',		'borrower_type',	$_LABELS['tblLoansMaster.borrower_type'],	($borrower_types 	= array('%%'=>'') + LOAN_TYPE::borrower_types()), 	'class=large');
$form->addElement('select',		'zone_id',			$_LABELS['tblUsers.zone_id'],				($zones 			= array('%%'=>'') + USER::zones()), 				'class=large');
$form->addElement('select',		'creator_id',		$_LABELS['requestedBy'],					($advisors 			= array('%%'=>'') + CLIENT::advisors()), 			'class=large');
$form->addElement('text',		'creator_date_1',	$_LABELS['date.before'], 					'class=large');
$form->addElement('text',		'creator_date_2',	$_LABELS['date.after'], 					'class=large');

$form->addRule('creator_date_1',	'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('creator_date_2',	'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');

if ($form->validate()) { 

  $values = $form->_submitValues;

  if ($values['creator_id'] 	== '%%') { $operator_creator_id 		= 'LIKE'; } else { $operator_creator_id 		= '='; }
  if ($values['borrower_type'] 	== '%%') { $operator_borrower_type 		= 'LIKE'; } else { $operator_borrower_type 		= '='; }
  if ($values['zone_id'] 		== '%%') { $operator_zone_id 			= 'LIKE'; } else { $operator_zone_id	 		= '='; }
  
  if ($values['creator_date_1'] == '')   { $creator_date_1 	= '9999-01-01'; } 
  if ($values['creator_date_2'] == '')   { $creator_date_2 	= '1000-01-01'; } 

//  $fields[] 						= 'sum(l.kp) as amount';
  $extra['amount']['header']		= 'tblLoansMaster.amount';
  $extra['amount']['pack']			= '%s';
  $extra['amount']['align']			= 'right';

//  $fields[] 						= 'CONCAT(u.first," ",u.last) as creator_id';
  $extra['creator_id']['header']	= 'requestedBy';
  $extra['creator_id']['pack']		= '%s';
  $extra['creator_id']['align']		= 'left';

//  $fields[] 						= 'z.zone as zone_id';
  $extra['zone_id']['header']		= 'tblUsers.zone_id';
  $extra['zone_id']['pack']			= '%s';
  $extra['zone_id']['align']		= 'left';

//  $fields[] 						= 'p.program as program_id';
  $extra['program_id']['header']	= 'tblPrograms.program';
  $extra['program_id']['pack']		= '%s';
  $extra['program_id']['align']		= 'left';

//  $fields[] 						= 'lm.id as view';
  $extra['view']['header']			= 'view';
  $extra['view']['pack']			= sprintf('<a href=\'javascript:openWin("index.popup.php?scr_name=LN.SCR.viewLoanMasterPrintable&id=%s","LoanHistory","menubar=no,scrollbars=yes,resizable=yes,width=700,height=380")\'>%s</a>' ,'%s',$_LABELS['view']); // sprintf("<a href='index.php?scr_name=LN.SCR.viewLoanMaster&id=%s'>%s</a>",'%s',$_LABELS['view']);
  $extra['view']['align']			= 'center';
  
//  $fields[] 						= 'lm.id as disbursement';
  $extra['disbursement']['header']	= 'doDisburse';
  $extra['disbursement']['pack']	= sprintf("<a href='index.php?scr_name=LN.SCR.doCheckDisbursement&id=%s'>%s</a>",'%s',$_LABELS['doDisburse']);
  $extra['disbursement']['align']	= 'center';

  $extra['param']['value']['borrower_type']					= $values['borrower_type'];
  $extra['param']['value']['zone_id']						= $values['zone_id'];
  $extra['param']['value']['creator_id']					= $values['creator_id'];
  $extra['param']['value']['creator_date_1']				= $values['creator_date_1'];
  $extra['param']['value']['creator_date_1']				= $values['creator_date_2'];

  $extra['param']['label']['tblLoanTypes.borrower_type']	= $borrower_types[$values['borrower_type']]		? $borrower_types[$values['borrower_type']]		: $_LABELS['all'];
  $extra['param']['label']['tblUsers.zone_id']				= $zones[$values['zone_id']]					? $zones[$values['zone_id']]					: $_LABELS['all'];
  $extra['param']['label']['requestedBy']					= $advisors[$values['creator_id']] 				? $advisors[$values['creator_id']] 				: $_LABELS['all'];
  $extra['param']['label']['date.before']					= $creator_date_1;
  $extra['param']['label']['date.after']					= $creator_date_2;
    
  $extra['param']['ref']									= WEBPAGE::$scr_name;
  
  //Este sql es para burlar un problemita con el engine, se hizo el sql en dos partes....
  $mrow = WEBPAGE::$dbh->getAssoc("select lmdO.master_id,1 from tblLoansMasterDetails as lmdO
                                                     where lmdO.master_id not in 
                                                       (select lmd.master_id from tblLoansMasterDetails as lmd where lmd.loan_id not in 
                                                          (select l.id from tblLoans as l where l.status = 'A')) group by lmdO.master_id");

  $mrow = WEBPAGE::$dbh->getAll(sprintf("
					    		select 
					    			lm.amount as amount,
					    			CONCAT(u.first,' ',u.last) as creator_id,z.zone as zone_id,p.program as program_id,lm.id as view,lm.id as disbursement 
		    						    from tblLoansMaster as lm, tblLoanTypes as lt, tblUsers as u, tblZones as z, tblPrograms as p, tblLoansMasterDetails as lmd   
                        			    where lm.check_status = 'A'
                        			      AND lm.creator_id %s '%s'
					    			      AND lt.borrower_type %s '%s'
		    						      AND lm.zone_id %s '%s'
		    						      AND lm.creator_date <= '%s'
					    			      AND lm.creator_date >= '%s'
		    						      AND lm.creator_id = u.id
		    						      AND lm.zone_id = z.id
					    			      AND lm.program_id = p.id 
		    						      AND lm.loan_type_id = lt.id
		    						      AND lmd.master_id = lm.id
		    						      AND lmd.master_id in (%s)
		    						    group by lmd.master_id",
										$operator_creator_id, $values['creator_id'],
										$operator_borrower_type, $values['borrower_type'],
										$operator_zone_id, $values['zone_id'],
										$creator_date_1, $creator_date_2, implode(',',array_keys($mrow))));

  $mrow = is_array($mrow) ? $mrow : array();
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

