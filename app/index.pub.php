<?php
error_reporting(E_ERROR);
include_once 'includes/trace.debugger.php';
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'Auth.php';
//require_once 'ST.LIB.login.inc';
WEBPAGE::START();

// check user auth. status
$auth = new Auth('DB', WEBPAGE::$auth_options, 'checkAccess');
//$auth->setFailedLoginCallback('closePopup');
//$auth->setLoginCallback('closePopup');
$auth->start();

// to be here you need to provide valid username/password 
// or have a valid session open
WEBPAGE::LOAD_SESSION();

WEBPAGE::$lang = $_GET['lang'] ? $_GET['lang'] : WEBPAGE::_DEFAULT_LANG;
$_LABELS 	= WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_LABELS_FILE, WEBPAGE::$lang));
$_CONF 		= WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_CONF_FILE,WEBPAGE::$lang));

// check permissions here ....  line below is just an example 
//if (!(is_numeric(WEBPAGE::$userAccessCode))) { exit; }

require './includes/index.pub.inc';

function checkAccess() {
  // scripts with public access here
  $_PUB['RP.SCR.performance'] = true;
  $_PUB['RP.SCR.clientData'] = true;
  
  //check if public access
  $_PUB[$_GET['scr_name']] ? '' : exit;
  }

?>
