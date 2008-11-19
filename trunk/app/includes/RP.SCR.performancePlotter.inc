<?
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'Date.php';
require_once 'class/coolbutton.php';
require_once 'class/roles.php';
require_once 'class/imf.php';

$cycle 	= array(''=>'','DAY'=>$_LABELS['byDay'],'WEEK'=>$_LABELS['byWeek'],'MONTH'=>$_LABELS['byMonth'],'YEAR'=>$_LABELS['byYear']);
$mode 	= array(''=>'','zone_id'=>$_LABELS['byBranch'],'program_id'=>$_LABELS['byProgram'],'advisor_id'=>$_LABELS['byLoanOfficer'],'total'=>$_LABELS['total']);
$fact 	= array(
			''=>'',
			'RP.GRP.Performance.newLoans'=>$_LABELS['RP.GRP.Performance.newLoans'],
			'RP.GRP.Performance.kpDisbursment'=>$_LABELS['RP.GRP.Performance.kpDisbursment'],
			'RP.GRP.Performance.kpRecovery'=>$_LABELS['RP.GRP.Performance.kpRecovery'],
			'RP.GRP.Performance.interestRecovery'=>$_LABELS['RP.GRP.Performance.interestRecovery'],
			'RP.GRP.Performance.penaltiesRecovery'=>$_LABELS['RP.GRP.Performance.penaltiesRecovery'],
			'RP.GRP.Performance.feesRecovery'=>$_LABELS['RP.GRP.Performance.feesRecovery'],
			'RP.GRP.Performance.insurancesRecovery'=>$_LABELS['RP.GRP.Performance.insurancesRecovery'],
			'RP.GRP.Performance.operationalIncome'=>$_LABELS['RP.GRP.Performance.operationalIncome'],
			'RP.GRP.Performance.riskPortfolio'=>$_LABELS['RP.GRP.Performance.riskPortfolio'],
			'RP.GRP.Performance.clientPortfolio'=>$_LABELS['RP.GRP.Performance.clientPortfolio'],
//			'RP.GRP.clientIOM'=>$_LABELS['clientIOM'],
			'RP.GRP.Performance.loansOnDelinquency'=>$_LABELS['RP.GRP.Performance.loansOnDelinquency']);
  
$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('plotterForm','POST','index.popup.php','','onSubmit="popupform(this)"');

//$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('plotterForm',$_LABELS['submit']));
$form->addElement('image', 	'submit', 	sprintf("%s?txt=%s",$btn->webface,$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('plotterForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);

$form->addElement('text',		'dateFrom',		$_LABELS['dateFrom'],														'class=large');
$form->addElement('text',		'dateTo',		$_LABELS['dateTo'],															'class=large');
$form->addElement('select',		'fact',			$_LABELS['fact'],					$fact,									'class=large');
$form->addElement('select',		'cycle',		$_LABELS['cycle'],					$cycle,									'class=large');
$form->addElement('select',		'mode',			$_LABELS['mode'],					$mode,									'onChange="updateModeFields()" class=large');
$form->addElement('select',		'program_id',  	$_LABELS['tblPrograms.id'],			array('-'=>'') + IMF::getPrograms('A') + IMF::getPrograms('I'),		sprintf('%s class=large', ($_REQUEST['mode'] == 'program_id') ? '' : 'disabled'));
$form->addElement('select',		'zone_id',		$_LABELS['tblUsers.zone_id'],		array('-'=>'') + IMF::getBranches(),								sprintf('%s class=large', ($_REQUEST['mode'] == 'zone_id') ? '' : 'disabled'));
$form->addElement('select',		'advisor_id',	$_LABELS['tblClients.advisor_id'],	array('-'=>'') + IMF::getLoanOfficers(1) + IMF::getLoanOfficers(0),	sprintf('%s class=large', ($_REQUEST['mode'] == 'advisor_id') ? '' : 'disabled'));

//add rules
$form->addRule('fact',			$_LABELS['selectOption'],	'required');
$form->addRule('dateFrom',		'yyyy-mm-dd',				'required');
$form->addRule('dateFrom',		'yyyy-mm-dd',				'regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('dateTo',		'yyyy-mm-dd',				'required');
$form->addRule('dateTo',		'yyyy-mm-dd',				'regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('cycle',			$_LABELS['selectOption'],	'required');
$form->addRule('mode',			$_LABELS['selectOption'],	'required');

$form->registerRule('checkdateTo', 		'callback', 'checkdateTo');
$form->addRule('dateTo', 				$_LABELS['nonValidEntry'], 'checkdateTo', true);
$form->registerRule('checkdateFrom', 	'callback', 'checkdateFrom');
$form->addRule('dateFrom', 				$_LABELS['nonValidEntry'], 'checkdateFrom', true);
$form->registerRule('checkcycle', 		'callback', 'checkcycle');
$form->addRule('cycle', 				$_LABELS['nonValidOption'], 'checkcycle', true);
$form->registerRule('checkprogram', 	'callback', 'checkprogram');
$form->addRule('program_id', 			$_LABELS['selectOption'], 'checkprogram', true);
$form->registerRule('checkzone', 		'callback', 'checkzone');
$form->addRule('zone_id', 				$_LABELS['selectOption'], 'checkzone', true);
$form->registerRule('checkadvisor', 	'callback', 'checkadvisor');
$form->addRule('advisor_id', 			$_LABELS['selectOption'], 'checkadvisor', true);

//$form->freeze($client->frostdata);

if ($form->validate()) { 

  $form->freeze();
  $values = $form->_submitValues;
  
  $form->removeElement('submit');
  $form->removeElement('cancel');
  $form->removeElement('reset');
  $values['program_id'] ? '' : $form->removeElement('program_id');
  $values['zone_id'] ? '' : $form->removeElement('zone_id');
  $values['advisor_id'] ? '' : $form->removeElement('advisor_id');
  
  $tpl->setVariable('graph_title', $_LABELS[$fact[$values['fact']]]);
  foreach($values as $key=>$val) {
    $str .= sprintf('&%s=%s',$key,$val); 
    }
  $tpl->setVariable('graph', sprintf("<img src='%s.php?lang=%s%s' alt=''>",$values['fact'],WEBPAGE::$lang,$str));
  $tpl->setVariable('print', $_LABELS['printSheet']);
  }

if (count($form->_errors)) { echo '<script>window.close();</script>'; }

$form->setDefaults();
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
  
function checkdateTo($dateTo) {
  $dateTo_check = explode('-',$dateTo); 
  if (!checkdate($dateTo_check[1],$dateTo_check[2],$dateTo_check[0])) { return false; } //verifica que es una fecha real
  if (date('Y-m-d') < $dateTo) { return false; } else { return true; }  
  }    
  
function checkdateFrom($dateFrom) {
  $dateFrom_check = explode('-',$dateFrom); 
  if (!checkdate($dateFrom_check[1],$dateFrom_check[2],$dateFrom_check[0])) { return false; } //verifica que es una fecha real
  if ($dateFrom > $_REQUEST['dateTo']) { return false; } else { return true; }  
  }    

function checkprogram($program_id) {
  if ($_REQUEST['mode'] != 'program_id') { return true; }
  if ($program_id == '-') {return false;}  
  return true;
  } 
     
function checkzone($zone_id) {
  if ($_REQUEST['mode'] != 'zone_id') { return true; }
  if ($zone_id == '-') {return false;}  
  return true;
  }    
  
function checkadvisor($advisor_id) {
  if ($_REQUEST['mode'] != 'advisor_id') { return true; }
  if ($advisor_id == '-') {return false;}  
  return true;
  }    

function checkcycle($cycle) {
  $dateFrom = explode('-',$_REQUEST['dateFrom']);
  $dateTo 	= explode('-',$_REQUEST['dateTo']); 
  switch($cycle) {
  	case 'DAY': 	
  	  if ($dateFrom['0'] != $dateTo['0']) { return false; }
  	  if ($dateFrom['1'] != $dateTo['1']) { return false; }
  	case 'WEEK': 	
  	  if ($dateFrom['0'] != $dateTo['0']) { return false; }
    }
  return current(current(WEBPAGE::$dbh->getAll(sprintf('select if(TO_DAYS(DATE_ADD("%s",INTERVAL %s DAY))>TO_DAYS("%s"),0,1)',$_REQUEST['dateFrom'],2*$cycle,$_REQUEST['dateTo']))));
  }    
?>
