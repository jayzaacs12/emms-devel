<?php
exit;
$cdata = WEBPAGE::$dbh->getAll(sprintf("
		 select c.id, c.code, u.id as advisor_id, concat(u.first,' ',u.last) as advisor from tblClients as c, tblUsers as u WHERE u.id = c.advisor_id"));

foreach ($cdata as $key=>$val) {
  $data[$val['advisor_id']]['name']  = $val['advisor'];
  $data[$val['advisor_id']]['hits'] += checkcode($val['code']);
  }

/*
$cdata = WEBPAGE::$dbh->getAssoc('select u.id, concat(u.first," ",u.last) as advisor, count(c.id) as ctotal from tblUsers as u, tblClients as c where u.id = c.advisor_id group by u.id');
$ldata = WEBPAGE::$dbh->getAssoc('select u.id, count(l.id) as ltotal from tblUsers as u, tblLoans as l where u.id = l.advisor_id group by u.id');
foreach ($cdata as $uid=>$val) {
  $data[$uid]['advisor'] = $val['advisor'];
  $data[$uid]['ctotal'] = $val['ctotal'];
  $data[$uid]['ltotal'] = $ldata[$uid]['ltotal'];
  }
*/
$head = array('advisor'=>'Asesor','ctotal'=>'Cedulas incorrectas');
$tpl->setVariable('chart', WEBPAGE::printchart($data,$head));


function checkcode($clientcode) {
  if (!preg_match('(^([0-9]{3}-[0-9]{7}-[0-9]{1})$)',$clientcode)) { return '1';}
  $code = str_replace('-','',$clientcode);
  $code = chunk_split($code,1,',');
  $data = explode(',',$code);
  unset($data[11]);
  $verdigit = array_pop($data);
  $base = 1;
  $total = 0;
  foreach ($data as $key=>$val) {
    $subtotal = $base*$val;
    if ($subtotal > 9) { $total += $subtotal - 9; } else { $total += $subtotal; }
    if ($base == 1) { $base = 2; } else { $base = 1; }
    }
  $resdigit = 10*floor($total/10) + 10 - $total;
  if (($resdigit == $verdigit)||($resdigit == $verdigit + 10)) { return '0'; }
  return '1';
  }

?>