<?php
error_reporting(E_ERROR);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once 'Auth.php';
require_once './includes/ST.LIB.login.inc';
require_once 'HTML/Template/ITX.php';
require_once 'class/TTFButton.php';


WEBPAGE::START();
// check user auth. status
$auth = new Auth('DB', WEBPAGE::$auth_options, 'loginFunction');
$auth->setFailedLoginCallback('loginFailedCallback');
$auth->setLoginCallback('loginCallback');
$auth->start();

WEBPAGE::LOAD_SESSION();

if(isset($_GET['lang'])){  WEBPAGE::$lang   = $_GET['lang']; } else { WEBPAGE::$lang   = 'esp'; }

$_LABELS         = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_LABELS_FILE,WEBPAGE::$lang));
$_CONF           = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_CONF_FILE,WEBPAGE::$lang));


$data = WEBPAGE::$dbh->getAll(sprintf("select
    lmd.master_id,
    l.xp_cancel_date,
    l.xp_num_pmt,
    lt.payment_frequency,
    lcd.xp_pmt_date,
    lcd.loan_id,
    lcd.xp_pmt,
    lcd.cn_delay,
    lcd.cn_penalties,
    lm.program_id, prg.program,
    lm.zone_id, z.short_name,
    concat(c.first,' ',c.last) cliente,
    c.code govID,
    s.name grupo,
    u.username,
    count(p.id)+1 cuota,
    fa.p_pmt_receipt_flag_a pmt_receipt_flag_a,
    fb.p_pmt_receipt_flag_b pmt_receipt_flag_b
from
    (
    tblLoansCurrentData lcd,
    tblLoansMasterDetails lmd,
    tblLoansMaster lm,
    tblLoans l,
    tblLoanTypes lt,
    tblClients c,
    tblPrograms prg,
    tblZones z,
    tblUsers u
    )
left join
    tblSocieties s on s.id = c.society_id
left join
    tblPayments p on p.loan_id = lcd.loan_id and p.special = 0
left join
    (select fap.loan_id p_loan_id, count(far.flag_a) p_pmt_receipt_flag_a FROM tblPayments fap, tblReceipts far, tblLinkReceiptsPayments falrp, tblLoansCurrentData falcd where falcd.loan_id = fap.loan_id and falcd.xp_pmt_date <= date_add(now(), interval 7 day) and far.flag_a = '1' and falrp.payment_id = fap.id and falrp.receipt_id = far.id group by fap.loan_id) fa on fa.p_loan_id = lcd.loan_id
left join
    (select fbp.loan_id p_loan_id, count(fbr.flag_b) p_pmt_receipt_flag_b FROM tblPayments fbp, tblReceipts fbr, tblLinkReceiptsPayments fblrp, tblLoansCurrentData fblcd where fblcd.loan_id = fbp.loan_id and fblcd.xp_pmt_date <= date_add(now(), interval 7 day) and fbr.flag_b = '1' and fblrp.payment_id = fbp.id and fblrp.receipt_id = fbr.id group by fbp.loan_id) fb on fb.p_loan_id = lcd.loan_id
where
        lcd.xp_pmt_date <= date_add(now(), interval 1 month)
    and lmd.loan_id = lcd.loan_id
    and lm.id = lmd.master_id
    and l.id = lcd.loan_id
    and lt.id = l.loan_type_id
    and c.id = l.client_id
    and prg.id = lm.program_id
    and z.id = lm.zone_id
    and u.id = l.advisor_id
group by
    lcd.loan_id
order by
    lcd.loan_id"));

$head = array('id'=>'ID','client'=>'Cliente','num_pmt'=>'No cuota','xp_num_pmt'=>'Total de cuotas','payment_frequency'=>'Frec. Pagos','xp_pmt_date'=>'Fecha pago','xp_cancel_date'=>'Fecha vencimiento','xp_pmt'=>'Monto cuota','cn_penalties'=>'Punitorios','pmt_receipt_flag_a'=>$_CONF['pmt_receipt_flag_a'] ? $_CONF['pmt_receipt_flag_a'] : '-','pmt_receipt_flag_b'=>$_CONF['pmt_receipt_flag_b'] ? $_CONF['pmt_receipt_flag_b'] : '-','username'=>'Asesor','program'=>'Programa','branch'=>'Sucursal','print'=>'Imprimir');

foreach($data as $key => $val) {
  $master[$val['master_id']]['id']                 = $val['master_id'];
  $master[$val['master_id']]['client']             = $counter[$val['master_id']] ? $val['grupo'] : $val['cliente'].' : '.$val['govID'];
//  $master[$val['master_id']]['client']           = $val['grupo'] ? $val['grupo'] : $val['cliente'];
  $master[$val['master_id']]['num_pmt']            = $val['cuota'];
  $master[$val['master_id']]['xp_num_pmt']         = $val['xp_num_pmt'];
  $master[$val['master_id']]['payment_frequency']  = $val['payment_frequency'];
  $master[$val['master_id']]['xp_pmt_date']        = $val['xp_pmt_date'];
  $master[$val['master_id']]['xp_cancel_date']     = $val['xp_cancel_date'];
  $master[$val['master_id']]['xp_pmt']            += $val['xp_pmt'];
  $master[$val['master_id']]['cn_penalties']      += $val['cn_penalties'];
  $master[$val['master_id']]['pmt_receipt_flag_a'] = $_CONF['pmt_receipt_flag_a'] ? ( $val['pmt_receipt_flag_a'] ? $val['pmt_receipt_flag_a'] : 0 ) : '-';
  $master[$val['master_id']]['pmt_receipt_flag_b'] = $_CONF['pmt_receipt_flag_b'] ? ( $val['pmt_receipt_flag_b'] ? $val['pmt_receipt_flag_b'] : 0 ) : '-';
  $master[$val['master_id']]['username']           = $val['username'];
  $master[$val['master_id']]['program']            = $val['program'];
  $master[$val['master_id']]['branch']             = $val['short_name'];

  $master[$val['master_id']]['print']             = sprintf('<a href="index.receipt.php?advisor=%s&client=%s&xp_pmt_date=%s&num_pmt=%s&xp_num_pmt=%s&xp_pmt=%s">Imprimir</a>',$master[$val['master_id']]['username'],$master[$val['master_id']]['client'],$master[$val['master_id']]['xp_pmt_date'],$master[$val['master_id']]['num_pmt'],$master[$val['master_id']]['xp_num_pmt'],$master[$val['master_id']]['xp_pmt']);

  $counter[$val['master_id']]++;

  }


$tpl = new HTML_Template_ITX('.');
$tpl->loadTemplateFile('index.compactCS.tpl');
$tpl->setVariable('chart', count($master) ? WEBPAGE::printchart($master,$head) : $_LABELS['noData']);
$tpl->show();

?>
