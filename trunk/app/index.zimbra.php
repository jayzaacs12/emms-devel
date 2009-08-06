<?php
session_start();
session_unset();
session_destroy();

include_once 'includes/trace.debugger.php';
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'Auth.php';
require_once './includes/ST.LIB.login.inc';

WEBPAGE::START();

// check user auth. status

function zLoginFailedCallback()
{
  header("Location: http://www.esperanza.org/us");
  exit;
}

$auth = new Auth('DB', WEBPAGE::$auth_options, 'loginFunction');
$auth->setFailedLoginCallback('zLoginFailedCallback');
$auth->setLoginCallback('loginCallback');
$auth->start();

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

// check permissions here ....  first line below is just an example
//if (!(is_numeric(WEBPAGE::$userAccessCode))) { exit; }

//Globals. Can be stored in external config.inc.php or retreived from a DB.

$PREAUTH_KEY="9ea4b22a9571fe1b91a16993875730e81784d9a7cb9452ee091d500103836846";
$WEB_MAIL_PREAUTH_URL="http://zimbra.esperanza.org/service/preauth";

/**
* User's email address and domain. In this example obtained from a GET query parameter.
* i.e. preauthExample.php?email=user@domain.com&domain=domain.com
* You could also parse the email instead of passing domain as a separate parameter
*/
$user = $_POST["username"];
$domain='esperanza.org';

$email = "{$user}@{$domain}";

if(empty($PREAUTH_KEY)) {
  die("Need preauth key for domain ".$domain);
  }

/**
* Create preauth token and preauth URL
*/
$timestamp=1000*(time()-60*60);
$preauthToken=hash_hmac("sha1",$email."|name|0|".$timestamp,$PREAUTH_KEY);
$preauthURL = $WEB_MAIL_PREAUTH_URL."?account=".$email."&by=name&timestamp=".$timestamp."&expires=0&preauth=".$preauthToken;

/**
* Redirect to Zimbra preauth URL
*/
header("Location: $preauthURL");
?>
