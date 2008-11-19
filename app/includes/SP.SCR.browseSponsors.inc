<?php
$mrow = WEBPAGE::$dbh->getAll("select s.username,s.sponsor,s.active,s.id as donations,s.id as add_donation,s.id as view,s.id as edit,s.id as fsummary,s.id as csummary from tblSponsors as s");


$extra['username']['header']	    = 'tblUsers.username';
$extra['username']['pack']		    = '%s';
$extra['username']['align']		    = 'left';

$extra['sponsor']['header']	      = 'tblSponsors.sponsor';
$extra['sponsor']['pack']		      = '%s';
$extra['sponsor']['align']	      = 'left';

$extra['active']['header']	      = 'tblUsers.active';
$extra['active']['pack']		      = 'tblUsers.active.%s';
$extra['active']['align']	        = 'right';

$extra['donations']['header']	    = 'donations';
$extra['donations']['pack']		    = sprintf("<a href='javascript:openWin(\"index.popup.php?scr_name=SP.SCR.donationHistoryBySponsor&id=%s\",\"donationHistoryBySponsor\",\"menubar=no,scrollbars=yes,resizable=yes,width=700,height=320\")'>%s</a>",'%s',$_LABELS['view']);
$extra['donations']['align']	    = 'center';

$extra['add_donation']['header']  = 'addDonation';
$extra['add_donation']['pack']	  = sprintf("<a href='%s?scr_name=SP.SCR.addDonation&ref=SP.SCR.addDonation&sponsor_id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['addDonation']);
$extra['add_donation']['align']	  = 'center';

$extra['view']['header']		      = 'view';
$extra['view']['pack']		        = sprintf("<a href='%s?scr_name=SP.SCR.viewSponsor&ref=SP.SCR.browseSponsors&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['view']);
$extra['view']['align']		        = 'center';

$extra['edit']['header']		      = 'edit';
$extra['edit']['pack']		        = sprintf("<a href='%s?scr_name=SP.SCR.addSponsor&ref=SP.SCR.editSponsor&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['edit']);
$extra['edit']['align']		        = 'center';

$extra['fsummary']['header']              = 'SP.SCR.financialSummary';
$extra['fsummary']['pack']              = sprintf("<a href='%s?scr_name=SP.SCR.financialSummary&ref=SP.SCR.browseSponsor&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['SP.SCR.financialSummary']);

$extra['fsummary']['align']             = 'center';

$extra['csummary']['header']              = 'SP.SCR.clientSummary';
$extra['csummary']['pack']              = sprintf("<a href='%s?scr_name=SP.SCR.clientSummary&ref=SP.SCR.browseSponsor&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['SP.SCR.clientSummary']);

$extra['fsummary']['align']             = 'center';

$extra['param']['ref']		        = WEBPAGE::$scr_name;

array_unshift($mrow,$extra);


WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);
require_once 'BS.SCR.pager.inc';

?>