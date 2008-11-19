<?
function loginFunction()
{
  global $message;
  if (!$message) { $message = 'pleaseLogin'; }
  $_LABELS = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_LABELS_FILE,WEBPAGE::$lang));
  $_CONF = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_CONF_FILE,WEBPAGE::$lang));
  require './includes/login.inc';
  exit;
}

function loginFailedCallback()
{
  global $_REQUEST;
  global $message;
  $message = 'invalidLogin';
  loginFunction();
}

function loginCallback()
{
  global $_CONF;
  $_SESSION['_authsession']['data']['module']      = 'mod_sponsorship';
  $_SESSION['_authsession']['data']['url']         = current(SQL::getAssoc('tblConfiguration',sprintf('var,%s',WEBPAGE::$lang),'var="url"'));
  $_SESSION['_authsession']['data']['lang']        = WEBPAGE::$lang;
  $_SESSION['_authsession']['data']['theme']       = WEBPAGE::$theme;
  $_SESSION['_authsession']['data']['screenWidth'] = WEBPAGE::$screenWidth;
  WEBPAGE::$dbh->query(sprintf("insert into tblSponsorsLog (sponsor_id,ip_address,login_date,last_hit_date) values ('%s','%s',NOW(),NOW())",$_SESSION['_authsession']['data']['id'],$_SERVER['REMOTE_ADDR']));
  $_SESSION['_authsession']['data']['session_id']  = mysql_insert_id(WEBPAGE::$dbh->connection);
  $_GET['scr_name'] = WEBPAGE::_DEFAULT_FRONTPAGE;
}

function encryptPass()
{
  $salt = md5($_POST['password']);
  $EncryptPass = crypt($_POST['password'], $salt);
  return $EncryptPass;
}


?>