<?php
exit;
$advisor = WEBPAGE::$dbh->getAll(sprintf("select concat(first,' ',last) as name from tblUsers where id = '%s'", $_GET['advisor_id']));

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select c.id, s.name as society, count(b.id) as business, concat(c.first,' ',c.last) as name, c.code, c.nick, l.id as loan_id, l.first_payment_date from (tblClients as c) LEFT JOIN tblLoans as l on l.client_id = c.id LEFT JOIN tblSocieties as s on s.id = c.society_id LEFT JOIN tblBusiness as b on b.client_list = c.id WHERE c.advisor_id = '%s' group by c.id ORDER BY society,name", $_GET['advisor_id']));

$c = 1;
foreach ($data as $key=>$val) {
  $data[$key]['id'] = $c++;
  $data[$key]['code'] = checkcode($val['code']);
  if (!$val['business']) {
    $data[$key]['business'] = sprintf("<a href='index.popup.php?scr_name=MG.SCR.addBusiness&owner_id=%s'><font color=red>Crear Negocio</font></a>",$val['id']);
    } else {
    $data[$key]['business'] = '<font color=green>OK</font>';
    }
  if ($val['first_payment_date'] <= date('Y-m-d')) {
  	$data[$key]['status'] = '<font color=red>Desactualizado</font>';
    } else {
  	$data[$key]['status'] = '<font color=green>En tiempo</font>';
    }
  if ( !$val['loan_id'] ) {
  	$data[$key]['status'] = '<font color=orange>No digitado</font>';
    $data[$key]['addLoan'] = sprintf("<a href='index.popup.php?scr_name=MG.SCR.addLoan&client_id=%s'>Entrar préstamo</a>", $val['id']);
    $data[$key]['viewLoan'] = '';
    $data[$key]['remLoan'] = '';
    } else {
    $data[$key]['addLoan'] = '';
    $data[$key]['viewLoan'] = sprintf("<a href='index.popup.php?scr_name=MG.SCR.viewILoan&id=%s&advisor_id=%s'>Ver préstamo</a>", $val['loan_id'], $_GET['advisor_id']);
    $data[$key]['remLoan'] = sprintf("<a href='index.popup.php?scr_name=MG.SCR.removeILoan&id=%s&advisor_id=%s'>Eliminar</a>", $val['loan_id'],$_GET['advisor_id']);
    }
  unset($data[$key]['loan_id']);
  unset($data[$key]['first_payment_date']);
  }

function checkcode($clientcode) {
  if (!preg_match('(^([0-9]{3}-[0-9]{7}-[0-9]{1})$)',$clientcode)) { return sprintf('<font color=red>%s</font>',$clientcode);}
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
  if (($resdigit == $verdigit)||($resdigit == $verdigit + 10)) { return $clientcode; }
  return sprintf('<font color=red>%s</font>',$clientcode);
  }

$head = array('id'=>'ID','society'=>'Grupo','business'=>'Negocio','name'=>'Nombre','code'=>'Cedula','nick'=>'Apodo','status'=>'Estado Prestamo','addLoan'=>'Accion','viewLoan'=>'Accion','remLoan'=>'Accion' );
$tpl->setVariable('advisor', $advisor[0]['name']);
$tpl->setVariable('chart', WEBPAGE::printchart($data,$head));
?>