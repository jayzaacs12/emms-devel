<?
require_once './mod_sponsorship/class/sponsor.php';

$sponsor = new SPONSOR($_REQUEST['id']);
$sponsor->loadDonations();

$extra['param']['label']['tblSponsors.sponsor']	    = $sponsor->data['sponsor'];

$extra['src_amount']['header']		= 'tblSponsorsDonations.src_amount';
$extra['src_amount']['pack']		  = '%s';
$extra['src_amount']['align']		  = 'right';

$extra['conv_amount']['header']		= 'tblSponsorsDonations.conv_amount';
$extra['conv_amount']['pack']		  = '%s';
$extra['conv_amount']['align']		= 'right';

$extra['creator_date']['header']	= 'date';
$extra['creator_date']['pack']		= '%s';
$extra['creator_date']['align']		= 'center';

$extra['param']['ref']		= WEBPAGE::$scr_name;

$mrow = array();
foreach($sponsor->data['donations'] as $key=>$val) {
  $mrow[$key]['src_amount']   = sprintf('%s %s',$val['src_currency'],$val['src_amount']);
  $mrow[$key]['conv_amount']  = sprintf('%s %s',$val['conv_currency'],$val['conv_amount']);
  $mrow[$key]['creator_date'] = $val['creator_date'];
  }

array_unshift($mrow,$extra);

WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);

require_once 'BS.SCR.pager.inc';

?>