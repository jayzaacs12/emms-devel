<?php
require_once 'PEAR.php';
require_once 'DB.php';
include_once 'includes/trace.debugger.inc';

$dbh = DB::connect('mysql://emms_eird:eird3006@localhost/emms_eird');
$dbh->setFetchMode(DB_FETCHMODE_ASSOC);
$dbh->setFetchMode(DB_FETCHMODE_ASSOC);
$data = $dbh->getAssoc("select id,code from tblClients");

//nftrace('just erasing the log file',1,'checkcode');

$c = 0;
echo '<table>';
foreach ($data as $id => $code) {
  if (!file_exists(sprintf('img/clients/%s.jpg',$code))) {
    if(checkcode($code) == TRUE ) {
    $c++; echo sprintf('<tr><td align=right>%s</td><td width=30px></td><td>%s</td></tr>',$c,$code);
   }
  }
}
echo '</table>';

function checkcode($clientcode) {
//nftrace('just erasing the log file',1,'checkcode'); 
 if (preg_match('(^([0-9]{3}-[0-9]{7}-[0-9]{1})$)',$clientcode)) { 
  $_pattern_match=TRUE;} else {$_pattern_match=FALSE;}
    //{return sprintf('<font color=red>%s</font>',$clientcode);}
  $code = str_replace('-','',$clientcode);
//nftrace($code,0,'checkcode','$code line 28>');
  $code = chunk_split($code,1,',');
//nftrace($code,0,'checkcode','$code line 30>');
  $data = explode(',',$code);
//nftrace($data,0,'checkcode','$data line 32>');
  unset($data[11]);
//nftrace($data,0,'checkcode','$data line 34 after unset($data[11])>');
  $verdigit = array_pop($data);
//nftrace($verdigit,0,'checkcode','$verdigit line 36>');
  $base = 1;
  $total = 0;
//nftrace($data,0,'checkcode','$data line 40 after array_pop>'); 
 foreach ($data as $key=>$val) {
    $subtotal = $base*$val;
    if ($subtotal > 9) { $total += $subtotal - 9; } else { $total += $subtotal; }
    if ($base == 1) { $base = 2; } else { $base = 1; }
    }
  $resdigit = 10*floor($total/10) + 10 - $total;
  if (($resdigit == $verdigit)||($resdigit == $verdigit + 10)) {$_checksum_match=TRUE; }
    else {$_checksum_match=FALSE;}
  //return sprintf('<font size="2.5px" color=red>%s</font>',$clientcode);
  if ($_pattern_match && $_checksum_match) {return TRUE;} else {return FALSE;}

}



?>