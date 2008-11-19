<?
require_once 'class/uprofile.php';
//require_once 'ST.functions.inc';

switch ($_SESSION['_authsession']['data']['lang']) {
  case 'esp':
    $addClient 				= 'Agregar cliente';
    $addSociety 			= 'Agregar grupo';
	$newLoan 				= $_LABELS['LN.SCR.requestLoan'];
	$doCollectionSheet 		= $_LABELS['LN.SCR.doCollectionSheet'];
    $addMultPayment 		= 'Aplicar pago grupal';
    $applySurvey 			= 'Aplicar encuesta';
    $browseLoans 			= 'Revisar préstamos';
    $calculator 			= 'Calculadora Financiera';
    $transact 				= 'Ver transacciones';
    $delinquency 			= 'Préstamos en mora';
    $factsheet 				= 'Resumen de operaciones';
    $reports				= 'Reportes';
    $submittedPayments		= 'Ver pagos aplicados';
    $welcome 				= 'Bienvenido %s';
    $roles_caption 			= 'Tus niveles de acceso son:';
    $shortcuts 				= 'Enlaces directos';
    break;
  default :
    $addClient 				= 'Add client';
    $addSociety 			= 'Add group';
	$newLoan 				= $_LABELS['LN.SCR.requestLoan'];
	$doCollectionSheet 		= $_LABELS['LN.SCR.doCollectionSheet'];
    $addMultPayment 		= 'Do bulk entry';
    $applySurvey 			= 'Apply survey';
    $browseLoans 			= 'Review loans';
    $calculator 			= 'Financial calculator';
    $transact 				= 'View transactions';
    $delinquency 			= 'Delinquency';
    $factsheet 				= 'Facts sheet';
    $reports				= 'Reports';
    $submittedPayments		= 'View submitted payments';
    $welcome 				= 'Welcome %s';
    $roles_caption 			= 'Your access rights:';
    $shortcuts 				= 'Quick jumps';
  }
$roles = UPROFILE::getRoles($_SESSION['_authsession']['data']['access_code']);

$tpl->setVariable('javascript',"<SCRIPT type='text/javascript' src='CS.functions.js'></SCRIPT>");
$tpl->setVariable('welcome',sprintf($welcome, $_SESSION['_authsession']['data']['first']));
$tpl->setVariable('roles_caption',$roles_caption);
$tpl->setVariable('roles',implode(', ',$roles));
//$tpl->setVariable('roles',implode(', ',maskRoles(getRoles(),$_SESSION['authsession']['data']['access_code'])));
$tpl->setVariable('shortcuts',$shortcuts);
$tpl->setVariable('addClient',$addClient);
$tpl->setVariable('addSociety',$addSociety);
$tpl->setVariable('newLoan',$newLoan);
$tpl->setVariable('doCollectionSheet',$doCollectionSheet);
$tpl->setVariable('addMultPayment',$addMultPayment);
$tpl->setVariable('browseLoans',$browseLoans);
$tpl->setVariable('delinquency',$delinquency);
$tpl->setVariable('transact',$transact);
$tpl->setVariable('calculator',$calculator);
$tpl->setVariable('applySurvey',$applySurvey);
$tpl->setVariable('factsheet',$factsheet);
$tpl->setVariable('reports',$reports);
$tpl->setVariable('submittedPayments',$submittedPayments);
$tpl->setVariable('mktime', mktime());
$tpl->setVariable('lang', WEBPAGE::$lang);


