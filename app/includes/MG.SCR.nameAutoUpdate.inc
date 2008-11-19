<?php
exit;
require_once 'class/roles.php';
require_once 'class/client.php';
require_once 'class/society.php';
$mrow = WEBPAGE::$dbh->getAll(sprintf("select id from tblSocieties where category = 'BG' and advisor_id != 0"));
foreach($mrow as $key=>$val) {
  $soc = new SOCIETY($val['id']);
  $soc->nameAutoUpdate();
  }
echo 'Todo ok';
?>