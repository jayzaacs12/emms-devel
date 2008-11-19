<?php
$show_gohome = true;

  $extra['src_amount']['header']		     = 'tblSponsorsDonations.src_amount';
  $extra['src_amount']['pack']		       = '%s';
  $extra['src_amount']['align']		       = 'right';

  $extra['conv_amount']['header']	       = 'tblSponsorsDonations.conv_currency_id';
  $extra['conv_amount']['pack']	         = '%s';
  $extra['conv_amount']['align']	       = 'right';

  $extra['rate']['header']	             = 'tblSponsorsDonations.rate';
  $extra['rate']['pack']	               = '%s';
  $extra['rate']['align']	               = 'right';

  $extra['creator_date']['header']		   = 'tblSponsorsDonations.creator_date';
  $extra['creator_date']['pack']		     = '%s';
  $extra['creator_date']['align']		     = 'right';

  $extra['param']['ref']		             = WEBPAGE::$scr_name;

  $mrow = WEBPAGE::$dbh->getAll(sprintf("select concat(cs.symbol,' ',sd.src_amount) as src_amount,concat(cc.symbol,' ',sd.conv_amount) as conv_amount,round(sd.conv_amount/sd.src_amount,2) as rate,sd.creator_date from tblSponsorsDonations as sd, tblCurrencys as cs, tblCurrencys as cc where sd.sponsor_id = '%s' and cs.id = sd.src_currency_id and cc.id = sd.conv_currency_id order by sd.id desc",WEBPAGE::$userID));

  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);
  require_once '../includes/BS.SCR.pager.inc';

?>