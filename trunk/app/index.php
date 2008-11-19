<?php

error_reporting(E_ERROR);
include_once 'includes/trace.debugger.php';

require_once 'class/webpage.php';

require_once 'class/sql.php';

require_once 'PEAR.php';

require_once 'Auth.php';

require_once './includes/ST.LIB.login.inc';



WEBPAGE::START();



// check user auth. status

$auth = new Auth('DB', WEBPAGE::$auth_options, 'loginFunction');

$auth->setFailedLoginCallback('loginFailedCallback');

$auth->setLoginCallback('loginCallback');

$auth->start();

//trace($_SESSION['_authsession']['data']['last'],0);

// blocks session hacking from emms modules

if ($_SESSION['_authsession']['data']['module'] != 'main' ) {

  $message = 'loggedOut';

  $auth->logout();

  loginFunction();

  exit;

  }



// blocks session hacking on shared hosting with multiple emms installations

if (WEBPAGE::$url != $_SESSION['_authsession']['data']['url']) {

    $message = 'loggedOut';

    $auth->logout();

    loginFunction();  

    exit;  	

  }



// to be here you need to provide valid username/password 

// or have a valid session open

WEBPAGE::LOAD_SESSION();

$_LABELS = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_LABELS_FILE,WEBPAGE::$lang));

$_CONF = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_CONF_FILE,WEBPAGE::$lang));



// request logout

if (!empty($_REQUEST['logout'])) {

  $message = 'loggedOut';

  $auth->logout();

  loginFunction();

  exit;

  }



// check run mode


switch (WEBPAGE::$runMode) {

  case WEBPAGE::_RUN_MODE_NORMAL:

    break;

  case WEBPAGE::_RUN_MODE_OUTDATED:

    require 'includes/index.update.inc';

    exit;

  default :

    $message = 'loggedOut.maintenance';

    $auth->logout();

    loginFunction();  

    exit;  	

  }



// check permissions here ....  first line below is just an example

//if (!(is_numeric(WEBPAGE::$userAccessCode))) { exit; }

if (!( intval(WEBPAGE::$userAccessCode) & intval($_CONF[WEBPAGE::$scr_name]) )) { 

  $message = 'loggedOut';

  $auth->logout();

  loginFunction();

  exit;

  }



//echo '<pre>';print_r($_CONF['url']);echo '</pre>';

//echo $_SESSION['_authsession']['data']['url'];



require './includes/index.inc';


?>  