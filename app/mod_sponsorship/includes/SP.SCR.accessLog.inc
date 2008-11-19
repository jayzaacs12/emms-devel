<?php
$show_gohome = true;

  $extra['ip_address']['header']		 = 'tblSponsorsLog.ip_address';
  $extra['ip_address']['pack']		   = '%s';
  $extra['ip_address']['align']		   = 'left';

  $extra['login_date']['header']		 = 'tblSponsorsLog.login_date';
  $extra['login_date']['pack']		   = '%s';
  $extra['login_date']['align']		   = 'left';

  $extra['last_hit_date']['header']	 = 'tblSponsorsLog.last_hit_date';
  $extra['last_hit_date']['pack']	   = '%s';
  $extra['last_hit_date']['align']	 = 'left';

  $extra['param']['ref']		= WEBPAGE::$scr_name;

  $mrow = WEBPAGE::$dbh->getAll(sprintf("select ip_address,login_date,last_hit_date from tblSponsorsLog where sponsor_id = '%s' order by id desc",WEBPAGE::$userID));
  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);
  require_once '../includes/BS.SCR.pager.inc';

?>