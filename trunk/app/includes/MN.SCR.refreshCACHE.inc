<?php
if (file_exists(sprintf(WEBPAGE::_APP_LABELS_FILE,WEBPAGE::$lang))) {
  unlink(sprintf(WEBPAGE::_APP_LABELS_FILE,WEBPAGE::$lang));
  }
if (file_exists(sprintf(WEBPAGE::_APP_CONF_FILE,WEBPAGE::$lang))) {
  unlink(sprintf(WEBPAGE::_APP_CONF_FILE,WEBPAGE::$lang));
  }
WEBPAGE::checkcachefiles();
$tpl->setVariable('msg',WEBPAGE::printmessage('wrn',$_LABELS['refreshCACHE.scs']));
?>