<?php
error_reporting(E_ERROR);
include_once 'includes/trace.debugger.php';
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'Auth.php';
//require_once 'ST.LIB.login.inc';

WEBPAGE::START();

$auth = new Auth('DB', WEBPAGE::$auth_options, 'closePopup');
$auth->setFailedLoginCallback('closePopup');
$auth->setLoginCallback('closePopup');
$auth->start();

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


// check permissions here ....  line below is just an example 
if (!(is_numeric(WEBPAGE::$userAccessCode))) { exit; }

require './includes/index.popup.inc';

function closePopup() {
  echo "<body onLoad='window.close()'>";
  exit;
  }

?>