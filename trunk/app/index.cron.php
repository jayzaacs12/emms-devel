<?php
error_reporting(E_ERROR);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once './includes/ST.LIB.login.inc';
WEBPAGE::START();
WEBPAGE::$lang   = 'eng';
$_LABELS         = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_LABELS_FILE,WEBPAGE::$lang));
$_CONF           = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_CONF_FILE,WEBPAGE::$lang));

// check run mode
switch (WEBPAGE::$runMode) {
  case WEBPAGE::_RUN_MODE_OUTDATED:
    break;
  default :
    if ($_CONF['auto_cron'] == '1') { exit; }
    WEBPAGE::redirect(sprintf('index.php?lang=%s',WEBPAGE::$lang));
    exit;
  }


// Chequea que no se ha hecho bk en el dia
//count(WEBPAGE::$dbh->getAll(sprintf("select id from tblDataLog where date = '%s' and script = '%s'",date('Y-m-d'),'index.cron.php'))) ? exit : '';
$rdate = explode('-',WEBPAGE::$runDate);
$p_date = date('Y-m-d',mktime(0,0,0,$rdate[1],$rdate[2],$rdate[0],0) + 24*60*60);

// Pone sistema en modo 'mantenimiento'. Nadie puede entrar en este modo.
// Usuarios 'logeados' previamente seran desocnectados en la primera accion que ejecuten
WEBPAGE::$dbh->query(sprintf("insert into tblDataLog values ('null','%s',CURTIME(),'maintenance','index.cron.php','','')",$p_date));

// correr procesos

//require 'index.cron.clientPortfolio.inc';
//actualiza tblClientPortfolio
$cdata = WEBPAGE::$dbh->getAll(sprintf("select count(c.id) as hits, z.parent_id, z.program_id, c.advisor_id, c.gender, IF(s.category,s.category,'I') as category from (tblClients as c, tblZones as z) left join tblSocieties as s on s.id = c.society_id where z.id = c.zone_id and c.advisor_id != 0 group by z.parent_id, z.program_id, c.advisor_id, c.gender, category"));
$sdata = WEBPAGE::$dbh->getAll(sprintf("select count(s.id) as hits, z.parent_id, z.program_id, s.advisor_id, s.category from tblSocieties as s, tblZones as z where z.id = s.zone_id and s.advisor_id != 0 group by z.parent_id, z.program_id, s.advisor_id, s.category"));
// PATCH: 2008.07.18
$acdata = WEBPAGE::$dbh->getAll(sprintf("select count(c.id) AS hits, z.parent_id, z.program_id, c.advisor_id from tblClients c, tblZones z where c.zone_id = z.id and c.advisor_id != 0 and c.id in (select l.client_id from tblLoans as l, tblLoansCurrentData as lcd where l.id = lcd.loan_id ) group by z.parent_id, z.program_id, c.advisor_id;"));

$token = array();
$c_data = array();
foreach ($cdata as $key=>$val) {
  $c_data[$val['parent_id']][$val['program_id']][$val['advisor_id']]['clients'] 			+= $val['hits'];
  $c_data[$val['parent_id']][$val['program_id']][$val['advisor_id']][$val['gender']] 		+= $val['hits'];
  $c_data[$val['parent_id']][$val['program_id']][$val['advisor_id']][$val['category']] 		+= $val['hits'];
  $token[sprintf('%s.%s.%s',$val['parent_id'],$val['program_id'],$val['advisor_id'])]	 	= 1;
  }
$s_data = array();
foreach ($sdata as $key=>$val) {
  $s_data[$val['parent_id']][$val['program_id']][$val['advisor_id']][$val['category']] 		+= $val['hits'];
  $token[sprintf('%s.%s.%s',$val['parent_id'],$val['program_id'],$val['advisor_id'])]	 	= 1;
  }

// PATCH: 2008.07.18
$ac_data = array();
foreach ($acdata as $key=>$val) {
  $ac_data[$val['parent_id']][$val['program_id']][$val['advisor_id']] ['client_al'] 		+= $val['hits'];
  $token[sprintf('%s.%s.%s',$val['parent_id'],$val['program_id'],$val['advisor_id'])]	 	= 1;
  }

$inx = array();
foreach($token as $key=>$val) {
  $inx = explode('.',$key);
// PATCH: 2008.07.18
  WEBPAGE::$dbh->query(sprintf("insert into tblClientPortfolio (date,zone_id,program_id,advisor_id,clients,female,male,client_i,client_g,client_b,group_g,group_b,group_bg,client_al) values ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",$p_date,$inx[0],$inx[1],$inx[2],$c_data[$inx[0]][$inx[1]][$inx[2]]['clients'],$c_data[$inx[0]][$inx[1]][$inx[2]]['F'],$c_data[$inx[0]][$inx[1]][$inx[2]]['M'],$c_data[$inx[0]][$inx[1]][$inx[2]]['I'],$c_data[$inx[0]][$inx[1]][$inx[2]]['G'],$c_data[$inx[0]][$inx[1]][$inx[2]]['B'],$s_data[$inx[0]][$inx[1]][$inx[2]]['G'],$s_data[$inx[0]][$inx[1]][$inx[2]]['B'],$s_data[$inx[0]][$inx[1]][$inx[2]]['BG'],$ac_data[$inx[0]][$inx[1]][$inx[2]]['client_al']));
  }

//require 'index.cron.loansUpdate.inc';
//actualiza la tabla tblLoansCurrentData con los valores de interes, mora y penalidad acumulados por cada prestamo
//nota: solo del proximo pago esperado. Prestamos con mas de una cuota vencida, solo muestra la primera.
WEBPAGE::$dbh->query(sprintf("update tblLoansCurrentData as lcd, tblLoans as l, tblLoanTypes AS lt
                         set
                           lcd.cn_date = '%s',
                           lcd.cn_delay = GREATEST(0,(TO_DAYS('%s')-TO_DAYS(xp_pmt_date))),
                           lcd.cn_penalties = IF(lcd.cn_delay<=l.margin_d,0,0.01*l.rates_d*GREATEST(0,(TO_DAYS('%s')-TO_DAYS(xp_pmt_date)))*lcd.xp_pmt)
                         where
                           l.id = lcd.loan_id AND
                           lt.id = l.loan_type_id",$p_date,$p_date,$p_date));

//require 'index.cron.risk.inc';
//actualiza tblRiskPortfolio
$dataA = array();
$dataB = array();
$dataC = array();
$dataD = array();
$riskA 		= WEBPAGE::$dbh->getAll(sprintf("select sum(lcd.balance_kp) as kp, l.zone_id, l.program_id, advisor_id from tblLoansCurrentData as lcd, tblLoans as l where l.id = lcd.loan_id and lcd.cn_delay >= '%s' group by l.zone_id, l.program_id, l.program_id, l.advisor_id", $_CONF['risk.days.A']));
foreach($riskA as $key=>$val) { $dataA[$val['zone_id']][$val['program_id']][$val['advisor_id']] = $val['kp']; }
$riskB 		= WEBPAGE::$dbh->getAll(sprintf("select sum(lcd.balance_kp) as kp, l.zone_id, l.program_id, advisor_id from tblLoansCurrentData as lcd, tblLoans as l where l.id = lcd.loan_id and lcd.cn_delay >= '%s' group by l.zone_id, l.program_id, l.program_id, l.advisor_id", $_CONF['risk.days.B']));
foreach($riskB as $key=>$val) { $dataB[$val['zone_id']][$val['program_id']][$val['advisor_id']] = $val['kp']; }
$riskC 		= WEBPAGE::$dbh->getAll(sprintf("select sum(lcd.balance_kp) as kp, l.zone_id, l.program_id, advisor_id from tblLoansCurrentData as lcd, tblLoans as l where l.id = lcd.loan_id and lcd.cn_delay >= '%s' group by l.zone_id, l.program_id, l.program_id, l.advisor_id", $_CONF['risk.days.C']));
foreach($riskC as $key=>$val) { $dataC[$val['zone_id']][$val['program_id']][$val['advisor_id']] = $val['kp']; }
$riskD 		= WEBPAGE::$dbh->getAll(sprintf("select sum(lcd.balance_kp) as kp, l.zone_id, l.program_id, advisor_id from tblLoansCurrentData as lcd, tblLoans as l where l.id = lcd.loan_id and lcd.cn_delay >= '%s' group by l.zone_id, l.program_id, l.program_id, l.advisor_id", $_CONF['risk.days.D']));
foreach($riskD as $key=>$val) { $dataD[$val['zone_id']][$val['program_id']][$val['advisor_id']] = $val['kp']; }
$balance 	= WEBPAGE::$dbh->getAll(sprintf("select sum(lcd.balance_kp) as kp, l.zone_id, l.program_id, advisor_id from tblLoansCurrentData as lcd, tblLoans as l where l.id = lcd.loan_id group by l.zone_id, l.program_id, l.program_id, l.advisor_id"));
$c = 0;
$data  = array();
foreach($balance as $key=>$val) {
  $data['riskA'] = max(0,$dataA[$val['zone_id']][$val['program_id']][$val['advisor_id']]);
  $data['riskB'] = max(0,$dataB[$val['zone_id']][$val['program_id']][$val['advisor_id']]);
  $data['riskC'] = max(0,$dataC[$val['zone_id']][$val['program_id']][$val['advisor_id']]);
  $data['riskD'] = max(0,$dataD[$val['zone_id']][$val['program_id']][$val['advisor_id']]);
  WEBPAGE::$dbh->query(sprintf("insert into tblRiskPortfolio (date,zone_id,program_id,advisor_id,balance,riskA,riskB,riskC,riskD) values ('%s','%s','%s','%s','%s','%s','%s','%s','%s')",$p_date,$val['zone_id'],$val['program_id'],$val['advisor_id'],$val['kp'],$data['riskA'],$data['riskB'],$data['riskC'],$data['riskD']));
  }

//require 'index.cron.delinquency.inc';
//actualiza cartera en atraso
$dloans = WEBPAGE::$dbh->getAll("select loan_id from tblLoansCurrentData where cn_delay > 0");
require_once 'Date.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_type.php';
require_once 'class/loan.php';

$c = 0;
$ldata = array();
foreach ($dloans as $key=>$val) {
  $loan = new LOAN($val['loan_id']);
  $balance_kp = $loan->data['balance_kp'];
  $hits = 0;
  while(($loan->data['xp_pmt_date'] <= $loan->data['cn_date'])&&($balance_kp > 0)) {
    $data				                  = $loan->getNextPaymentData();
    $loan->data['xp_pmt_date']		= $data['xp_pmt_date'];
    $loan->data['xp_pmt']		      = $data['xp_pmt'];
    $loan->data['balance_kp']		  = $data['balance_kp'];
    $loan->data['balance_kaf']		= $data['balance_kaf'];
    $loan->data['balance_kat']		= $data['balance_kat'];
    $loan->data['r_from_date']		= $data['r_from_date'];
    $ldata[$c]['fees']			     += $data['fees'];
    $ldata[$c]['insurances']		 += $data['insurances'];
    $ldata[$c]['principal']		   += $data['principal'];
    $ldata[$c]['interest']		   += $data['interest'];
    $ldata[$c]['delay']			     += $data['delay'];
    $ldata[$c]['penalties']		   += $data['penalties'];
    $ldata[$c]['pmt']			       += $data['pmt'];
    $hits++;
    $balance_kp 			            =  $data['balance_kp'];
    }
  $ldata[$c]['loan_id']			      = $val['loan_id'];
  $ldata[$c]['date']			        = $loan->data['cn_date'];
  $ldata[$c]['hits']			        = $hits;
  $c++;
  }
foreach($ldata as $key=>$val) {
  WEBPAGE::$dbh->query(sprintf("insert into tblLoansOnDelinquency values ('null','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",$val['loan_id'],$val['date'],$val['hits'],$val['delay'],$val['pmt'],$val['penalties'],$val['interest'],$val['fees'],$val['insurances'],$val['principal']));
  }

//require 'index.cron.transactionsUpdate.inc';
//actualiza tblTCredits y fija pagos del dia anterior en tblPayments
//WEBPAGE::$dbh->query("insert into tblTCredits (code,date,branch_id,program_id,amount,principal,fees,insurances,interest,penalties) select CONCAT('C',p.date+0,LPAD(l.zone_id,3,'0'),LPAD(l.program_id,3,'0')) as transaction,p.date,l.zone_id,l.program_id,sum(p.pmt),sum(p.principal),sum(p.fees),sum(p.insurances),sum(p.interest),sum(p.penalties) from tblPayments as p, tblLoans as l where p.transaction_id = '0' and l.id = p.loan_id group by transaction");
WEBPAGE::$dbh->query("insert into tblTCredits (code,date,branch_id,program_id,fund_id,amount,principal,fees,insurances,interest,penalties) select CONCAT('C',p.date+0,LPAD(l.zone_id,3,'0'),LPAD(l.program_id,3,'0'),LPAD(flmp.fund_id,3,'0')) as transaction, p.date, l.zone_id, l.program_id, flmp.fund_id, sum(p.pmt),sum(p.principal),sum(p.fees),sum(p.insurances),sum(p.interest),sum(p.penalties) from tblPayments as p, tblLoans as l, tblFundsLoansMasterPct flmp, tblLoansMasterDetails lmd where lmd.loan_id = l.id and flmp.master_id = lmd.master_id and p.transaction_id = '0' and l.id = p.loan_id group by transaction");
WEBPAGE::$dbh->query("update tblPayments as p, tblLoans as l, tblFundsLoansMasterPct flmp, tblLoansMasterDetails lmd  set p.transaction_id = CONCAT('C',p.date+0,LPAD(l.zone_id,3,'0'),LPAD(l.program_id,3,'0'),LPAD(flmp.fund_id,3,'0')) where p.transaction_id = 0 and l.id = p.loan_id and lmd.loan_id = l.id and flmp.master_id = lmd.master_id");
//tblTDebits se actualiza desde LN.SCR.doCheckRelease.inc

WEBPAGE::$dbh->query("delete from tblLoansCurrentDataBackup");
WEBPAGE::$dbh->query("insert into tblLoansCurrentDataBackup select * from tblLoansCurrentData");

//Automatic loan write off
if ($_CONF['auto_write_off']) {
  $lod_id = WEBPAGE::$dbh->getAll(sprintf("select max(lod.id) id,lcd.loan_id,lcd.balance_kp,lcd.balance_kaf,lcd.balance_kat,lcd.cn_date from tblLoansOnDelinquency lod, tblLoansCurrentData lcd where lcd.loan_id = lod.loan_id and lcd.cn_delay > '%s' group by lod.loan_id", $_CONF['auto_write_off_margin']));
  foreach($lod_id as $key=>$val) {
    $lod_info = current(WEBPAGE::$dbh->getAll(sprintf("select lod.interest,lod.penalties from tblLoansOnDelinquency lod where lod.id = '%s'",$val['id'])));
    $amount = $val['balance_kp'] + $val['balance_kaf'] + $val['balance_kat'] + $lod_info['interest'] + $lod_info['penalties'];
    WEBPAGE::$dbh->query(sprintf("insert into tblLoanWriteOff (id,loan_id,amount,principal,insurance,fees,interest,penalties,date,user_id) values ('Null','%s','%s','%s','%s','%s','%s','%s','%s','%s')",$val['loan_id'],$amount,$val['balance_kp'],$val['balance_kaf'],$val['balance_kat'],$lod_info['interest'],$lod_info['penalties'],$val['cn_date'],'1'));
    WEBPAGE::$dbh->query(sprintf("delete from tblLoansCurrentData where loan_id = %s", $val['loan_id']));
    WEBPAGE::$dbh->query(sprintf("insert into tblLoanStatusHistory (id,loan_id,p_status,status,date,user_id,memo) values ('Null','%s','G','LI','%s','1','%s')",$val['loan_id'],$val['cn_date'],$_LABELS['loanWriteOff']));
    WEBPAGE::$dbh->query(sprintf("update tblLoans set status = 'LI', editor_id = '1', editor_date = '%s' where id = '%s'",$val['cn_date'],$val['loan_id']));
    }
  }

// Pone sistema en el modo anterior.
WEBPAGE::$dbh->query(sprintf("insert into tblDataLog values ('null','%s',CURTIME(),'normal','index.cron.php','','')",$p_date));

if ($_CONF['auto_cron'] != '1') { WEBPAGE::redirect('index.cron.php'); exit; }

?>