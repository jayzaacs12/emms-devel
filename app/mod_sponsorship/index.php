<?php
//echo 'We are doing maintenance tasks... Please check again soon...';
error_reporting(E_ERROR);
set_include_path(get_include_path() . PATH_SEPARATOR . '../');
//include_once '../includes/trace.debugger.php';
require_once '../class/webpage.php';
require_once '../class/sql.php';
require_once 'PEAR.php';
require_once 'Auth.php';
require_once './includes/ST.LIB.login.inc';

//error_reporting(E_ALL);

// if (function_exists('trace')) {
//     trace(getcwd());
// }

WEBPAGE::START();

WEBPAGE::$auth_options['table'] = 'tblSponsors';
// check user auth. status
$auth = new Auth('DB', WEBPAGE::$auth_options, 'loginFunction');
$auth->setFailedLoginCallback('loginFailedCallback');
$auth->setLoginCallback('loginCallback');


$auth->start();


// blocks session hacking from other emms modules
if ($_SESSION['_authsession']['data']['module'] != 'mod_sponsorship' ) {
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
/*
switch (WEBPAGE::$runMode) {
  case 'normal':
    break;
  default :
    $message = 'loggedOut.maintenance';
    $auth->logout();
    loginFunction();
    exit;
  }
*/
// check permissions here ....
if (!$_SESSION['_authsession']['data']['active']) {
  $message = 'loggedOut';
  $auth->logout();
  loginFunction();
  exit;
  }

WEBPAGE::$dbh->query(sprintf("update tblSponsorsLog set last_hit_date = NOW() where id = '%s'",$_SESSION['_authsession']['data']['session_id']));

require './includes/index.inc';

?>