<?php
exit;


///////////////////////////////////////// valida client_id

if( !current(WEBPAGE::$dbh->getAssoc(sprintf("select code,id from tblClients where id = %s and zone_id != 0 and advisor_id != 0", $_REQUEST['client_id'])))) {
  WEBPAGE::redirect('index.popup.php?scr_name=MG.SCR.clientLookUp');
  }
if( $check_loan_id = current(WEBPAGE::$dbh->getAssoc(sprintf("select client_id,id from tblLoans where client_id = %s", $_REQUEST['client_id'])))) {
  WEBPAGE::redirect(sprintf('index.popup.php?scr_name=MG.SCR.viewILoan&id=%s',$check_loan_id));
  }


//////////////////////////////////////// datos del cliente

require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';

extract(CLIENT::getTemplateData($_REQUEST['client_id']));

$tpl->setVariable('title',				'Ventana de digitación del préstamo');

$tpl->setVariable('client_code',		'Identificación');
$tpl->setVariable('code_label',			'Cédula');
$tpl->setVariable('code',				$code);

$tpl->setVariable('client_data',		'Confirmación de Identificación');
$tpl->setVariable('name_label',			'Nombre');
$tpl->setVariable('name',				$name);
$tpl->setVariable('zone_label',			'Sucursal/Programa');
$tpl->setVariable('zone',				$zone);
$tpl->setVariable('advisor_label',		'Asesor');
$tpl->setVariable('advisor',			$advisor);
$tpl->setVariable('society_label',		'Grupo/BDE');
$tpl->setVariable('society',			$society ? $society : 'Individual');

//////////////////////////////////////// formulario de prestamo

require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';

$btn  		= new TTFButton(WEBPAGE::$theme);
$form 		= new HTML_QuickForm('iLoanForm','POST');

$loan_types = WEBPAGE::$dbh->getAssoc(sprintf("select id,description from tblLoanTypes where borrower_type = '%s'",$category));
$funds = WEBPAGE::$dbh->getAssoc(sprintf("select f.id,f.fund from tblFunds as f, tblLinkProgramsFunds as lpf, tblZones as z, tblClients as c where c.id = %s and z.id = c.zone_id and lpf.program_id = z.program_id and f.id = lpf.fund_id",$_REQUEST['client_id']));

$form->addElement('static', 	'submit', 	'', 			$btn->imgsubmit('iLoanForm',$_LABELS['submit']));
$form->addElement('static', 	'cancel', 	'', 			$btn->imglink($PHP_SELF,$_LABELS['cancel']));
$form->addElement('hidden',		'scr_name',					$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',						$_REQUEST['id']);
$form->addElement('hidden',		'client_id',				$_REQUEST['client_id']);
$form->addElement('select',		'fund_id',					'Fondo',					(array('-'=>'') + $funds));
$form->addElement('select',		'business_id',				'Negocio',					(array('-'=>'') + $business));
$form->addElement('select',		'loan_type_id',				'Modalidad préstamo',		(array('-'=>'') + $loan_types));
$form->addElement('text',		'number_of_payments',		'No. de cuotas pendientes');
//$form->addElement('text',		'original_kp',				'Monto original del préstamo');
$form->addElement('text',		'new_kp',					'Monto total pendiente');
$form->addElement('text',		'new_delivered_date',		'Fecha último pago');
$form->addElement('text',		'new_first_payment_date',	'Fecha próximo pago');
$form->addElement('text',		'savings_v',				'Ahorro voluntario');

//add rules
$form->addRule('fund_id',					'Seleccione una opción',				'required');
$form->addRule('fund_id',					'Seleccione una opción',				'alphanumeric');
//$form->addRule('business_id',				'Seleccione una opción',				'required');
//$form->addRule('business_id',				'Seleccione una opción',				'alphanumeric');
$form->addRule('loan_type_id',				'Seleccione una opción',				'required');
$form->addRule('loan_type_id',				'Seleccione una opción',				'alphanumeric');
$form->addRule('number_of_payments',		'Especifique no. de cuotas pendientes',	'required');
$form->addRule('number_of_payments',		'Especifique no. de cuotas pendientes',	'regex', '(^[0-9 ]*$)');
//$form->addRule('original_kp',				'Especifique un monto válido',			'required');
//$form->addRule('original_kp',				'Especifique un monto válido',			'numeric');
$form->addRule('new_kp',					'Especifique un monto válido',			'required');
$form->addRule('new_kp',					'Especifique un monto válido',			'numeric');
$form->addRule('new_delivered_date',		'Formato válido: yyyy-mm-dd',			'required');
$form->addRule('new_delivered_date',		'Formato válido: yyyy-mm-dd',			'regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('new_delivered_date',		'Fechas no válidas',					'callback','checkdates');
$form->addRule('new_first_payment_date',	'Formato válido: yyyy-mm-dd',			'required');
$form->addRule('new_first_payment_date',	'Formato válido: yyyy-mm-dd',			'regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('new_first_payment_date',	'Fechas no válidas',					'callback','checkdates');
//$form->addRule('savings_v',					'Especifique un monto válido',			'required');
//$form->addRule('savings_v',					'Especifique un monto válido',			'numeric');

function checkdates() {
  if ($_REQUEST['new_first_payment_date'] <= $_REQUEST['new_delivered_date']) { return false; }
  return true;
  }

//$form->freeze();

if ($form->validate()) {
  $values = $form->_submitValues;

  require_once 'Date.php';
  require_once './includes/LN.LIB.functions.inc';
  require_once 'class/society.php';
  require_once 'class/business_type.php';
  require_once 'class/business.php';
  require_once 'class/zone.php';
  require_once 'class/loan_type.php';
  require_once 'class/loan.php';
  require_once 'class/loan_master.php';

  $ltype 	= new LOAN_TYPE($values['loan_type_id']);
  $zone 	= new ZONE($zone_id);

  switch ($category) {
  	case 'I': $borrower_id = $values['client_id']; break;
  	case 'B': $borrower_id = $society_id; break;
    default  : exit;
    }

/*
  switch($ltype->data['payment_frequency']) {
    case  "W": $NumberOfPayments = max(1,floor($values['installment'] / 7));	break;
	case "BW": $NumberOfPayments = max(1,floor($values['installment'] / 14));	break;
	case  "M": $NumberOfPayments = max(1,floor($values['installment'] / 30));	break;
	case  "Q": $NumberOfPayments = max(1,floor($values['installment'] / 90));	break;
	case "SA": $NumberOfPayments = max(1,floor($values['installment'] / 180));	break;
	case  "A": $NumberOfPayments = max(1,floor($values['installment'] / 360));	break;
	default  : exit;
    }
*/
  $NumberOfPayments = $values['number_of_payments'];
  switch($ltype->data['payment_frequency']) {
    case  "W": $calc_installment = $values['number_of_payments'] * 7;	break;
	case "BW": $calc_installment = $values['number_of_payments'] * 14;	break;
	case  "M": $calc_installment = $values['number_of_payments'] * 30;	break;
	case  "Q": $calc_installment = $values['number_of_payments'] * 90;	break;
	case "SA": $calc_installment = $values['number_of_payments'] * 180;	break;
	case  "A": $calc_installment = $values['number_of_payments'] * 360;	break;
	default  : exit;
    }
  $values['installment'] = $calc_installment;

// load loan data - tblLoans
  $data['loan_code'] 				= (date(ym).'.'.str_pad($zone->zonedata['parent_id'], 3, '0', STR_PAD_LEFT).'.'.str_pad($values['loan_type_id'], 3, '0', STR_PAD_LEFT).'.'.str_pad($values['client_id'], 6, '0', STR_PAD_LEFT));
  $data['status'] 					= 'G';
  $data['client_id'] 				= $values['client_id'];
  $data['loan_type_id'] 			= $values['loan_type_id'];
  $data['business_id'] 				= $values['business_id'];
  $data['installment'] 				= $calc_installment;
  $data['fees_at'] 					= $ltype->data['fees_at'];
  $data['fees_af'] 					= $ltype->data['fees_af'];
  $data['rates_r'] 					= $ltype->data['rates_r'];
  $data['rates_d'] 					= $ltype->data['rates_d'];
  $data['rates_e'] 					= round(CalEffRate($data['rates_r'],$data['fees_at'],$data['fees_af'],$data['installment'],$ltype->data['calendar_type'],$ltype->data['payment_frequency']),2);
  $data['margin_d'] 				= $ltype->data['margin_d'];
  $data['kp'] 						= $values['new_kp'];
  $data['kat'] 						= $values['new_kp'] * $ltype->data['fees_at'] / 100;
  $data['kaf'] 						= $values['new_kp'] * $ltype->data['fees_af'] / 100;
  $data['pmt'] 						= ceil(CalMinPMT($data['kp'],$data['kat'],$data['kaf'],$data['rates_r'],$data['installment'],$ltype->data['calendar_type'],$ltype->data['payment_frequency']));
  $data['savings_p'] 				= ceil((0.01 * $values['new_kp'] * $ltype->data['savings_p'])/$NumberOfPayments);
  $data['savings_v'] 				= $values['savings_v'];
  $data['pg_value'] 				= 0;
  $data['pg_memo'] 					= '';
  $data['re_value'] 				= 0;
  $data['re_memo'] 					= '';
  $data['fgd_value'] 				= 0;
  $data['fgd_memo'] 				= '';
  $data['fgt_value'] 				= $values['new_kp']*(1+0.01*$ltype->data['margin_c']);
  $data['fgt_memo'] 				= '';
  $data['zone_id'] 					= $zone->zonedata['parent_id'];
  $data['client_zone_id'] 			= $zone->zonedata['id'];
  $data['program_id'] 				= $zone->zonedata['program_id'];
  $data['advisor_id'] 				= $advisor_id;
  $data['creator_date'] 			= $values['new_delivered_date'];
  $data['creator_id'] 				= WEBPAGE::$userID;
  $data['editor_date'] 				= $values['new_delivered_date'];
  $data['editor_id'] 				= WEBPAGE::$userID;
  $data['delivered_date'] 			= $values['new_delivered_date'];
  $data['first_payment_date'] 		= $values['new_first_payment_date'];

  $loan_id 							= SQL::insert('tblLoans',$data,'id');

// load loans master data - tblLoansMaster
  $master 					= current(WEBPAGE::$dbh->getAssoc(sprintf("select id as inx,id from tblLoansMaster where borrower_id = '%s' and borrower_type = '%s'", $borrower_id, $category)));
  $mdata['borrower_id'] 	= $borrower_id;
  $mdata['borrower_type'] 	= $category;
  $mdata['loan_type_id'] 	= $values['loan_type_id'];
  $mdata['zone_id']			= $zone->zonedata['parent_id'];
  $mdata['program_id']		= $zone->zonedata['program_id'];
  $mdata['check_number'] 	= sprintf("%s%s",$category,$borrower_id);
  $mdata['check_status'] 	= 'R';
  $mdata['creator_id'] 		= WEBPAGE::$userID;
  $mdata['creator_date'] 	= $values['new_delivered_date'];
  $mdata['editor_id'] 		= WEBPAGE::$userID;
  $mdata['editor_date'] 	= $values['new_delivered_date'];
  $master_id 				= $master ? $master : SQL::insert('tblLoansMaster',$mdata, 'id');
  WEBPAGE::$dbh->query(sprintf("insert into tblLoansMasterDetails (master_id,loan_id) values ('%s','%s')",$master_id,$loan_id));
  WEBPAGE::$dbh->query(sprintf("update tblLoansMaster as lm set lm.amount = (select sum(l.kp) from tblLoans as l, tblLoansMasterDetails as lmd where lmd.master_id = %s and lmd.loan_id = l.id) where lm.id = %s",$master_id,$master_id));

// load fund loans master pct data - tblFundsLoansMasterPct
  $fundslmpct 				= current(WEBPAGE::$dbh->getAssoc(sprintf("select id as inx,id from tblFundsLoansMasterPct where master_id = '%s'", $master_id)));
  $fundslmpct ? '' : WEBPAGE::$dbh->query(sprintf("insert into tblFundsLoansMasterPct (id,master_id,fund_id,pct) values ('null',%s,%s,'100')",$master_id,$values['fund_id']));

// load loan status history - tblLoanStatusHistory
  $lshdata['loan_id']				= $loan_id;
  $lshdata['date']					= $data['delivered_date'];
  $lshdata['user_id']				= WEBPAGE::$userID;
  $lshdata['memo'] 	     	     	= '';
  $lshdata['p_status']				= 'N';
  $lshdata['status']				= 'O';
  SQL::insert('tblLoanStatusHistory',$lshdata,'id');
  $lshdata['p_status']				= 'O';
  $lshdata['status']				= 'S';
  SQL::insert('tblLoanStatusHistory',$lshdata,'id');
  $lshdata['p_status']				= 'S';
  $lshdata['status']				= 'A';
  SQL::insert('tblLoanStatusHistory',$lshdata,'id');
  $lshdata['p_status']				= 'A';
  $lshdata['status']				= 'D';
  SQL::insert('tblLoanStatusHistory',$lshdata,'id');
  $lshdata['p_status']				= 'D';
  $lshdata['status']				= 'G';
  SQL::insert('tblLoanStatusHistory',$lshdata,'id');

// load loan current data - tblLoansCurrentData
  $lcdata['loan_id']				= $loan_id;
  $lcdata['balance_kp']				= $data['kp'];
  $lcdata['balance_kaf']			= $data['kaf'];
  $lcdata['balance_kat']			= $data['kat'];
  $lcdata['r_from_date']			= $data['delivered_date'];
  $lcdata['xp_pmt_date']			= $data['first_payment_date'];
  $lcdata['xp_pmt']					= $data['pmt'];
  $lcdata['cn_date']				= $data['delivered_date'];
  $lcdata['cn_delay']				= 0;
  $lcdata['cn_penalties']			= 0;
  SQL::insert('tblLoansCurrentData',$lcdata,'id');

  $loan = new LOAN($loan_id);
  $loan->load_xp_pmt_plan();
  $xp_num_pmt = count($loan->data['xp_pmt_plan']);
  $xp_cancel_date = $loan->data['xp_pmt_plan'][$xp_num_pmt-1]['pmt_date'];
  WEBPAGE::$dbh->query(sprintf("update tblLoans as l set l.xp_num_pmt = '%s',l.xp_cancel_date = '%s' where l.id = %s",$xp_num_pmt,$xp_cancel_date,$loan_id));


  WEBPAGE::redirect(sprintf('index.popup.php?scr_name=MG.SCR.viewILoan&id=%s',$loan_id));
  }

$tpl->setVariable('loan_data',		'Información del préstamo');
$form->setDefaults();
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>