<html>
<head>
<link href="themes/blue/style.css" rel="stylesheet" type="text/css">
<title>
Esperanza Internacional - Branch Office Performance
</title>
<style type="text/css">
a.hide {display:none;}
</style>
<body>
<form action="index.branch.performance1.php" method="get">
<table><tr><td>
<select id="report" name="report">
    <option id="00" value="">Seleccione Reporte</option>
    <option id="01" value="01"> *Riesgo</option>
    <option id="02" value="02"> *Cantidad de Prestamos</option>
    <option id="03" value="03"> *Cartera Vigente</option>
    <option id="04" value="04"> *Cartera Clientes</option>
    </select>
</td></td><td>&nbsp;</td><td><br /><br /><input type="submit" value="submit" /></td>
</tr>
<tr>
<td class="label">Fecha inicio:</td><td><input type="text" name="start"></td>
</tr>
<tr>
<td class="label">Fecha final:</td><td><input type="text" name="end"></td>
</tr>
</table>
</form>
<p></p>
<?php
//error_reporting(E_ERROR);
//error_reporting(E_ALL);
include_once 'includes/trace.debugger.php';
require_once 'HTML/Template/ITX.php';
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once './includes/ST.LIB.login.inc';
//require_once ("Mail.php");
//require_once ("Mail/mime.php"); 
print "<h1>Advisor Performance Report:</h1>";
print "<hr>";
WEBPAGE::START();
/*
eng - english
esp - spanish
fra - french
*/

if(isset($_GET['lang'])){
WEBPAGE::$lang   = $_GET['lang'];
} else {

WEBPAGE::$lang   = 'esp';

}


$_LABELS         = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_LABELS_FILE,WEBPAGE::$lang));
$_CONF           = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_CONF_FILE,WEBPAGE::$lang));

if (function_exists('trace')) {
   //trace($_CONF);
}

$start_date=$_GET['start'];
$end_date=$_GET['end'];


// Risk per Branch
if($_GET['report']=="01") {
$riesgo = WEBPAGE::$dbh->getAll(sprintf("SELECT  z.id                                ,
        z.zone                                 AS Zona         ,
        ROUND( 100 * ( SUM( rp.riskA ) / SUM( rp.balance ) ) , 2 ) AS PAR15_percent  ,
        ROUND( 100 * ( SUM( rp.riskB ) / SUM( rp.balance ) ) , 2 ) AS PAR30_percent  ,
        ROUND( 100 * ( SUM( rp.riskC ) / SUM( rp.balance ) ) , 2 ) AS PAR60_percent  ,
        ROUND( 100 * ( SUM( rp.riskD ) / SUM( rp.balance ) ) , 2 ) AS PAR90_percent  ,

        (ROUND( 100 * ( SUM( rp.riskA ) / SUM( rp.balance ) ) , 2 ) +
        ROUND( 100 * ( SUM( rp.riskB ) / SUM( rp.balance ) ) , 2 )  +
        ROUND( 100 * ( SUM( rp.riskC ) / SUM( rp.balance ) ) , 2 )  +
        ROUND( 100 * ( SUM( rp.riskD ) / SUM( rp.balance ) ) , 2 )) AS riesgo_suma,
        rp.date
FROM    (tblLoans l,
        tblZones z)

LEFT JOIN tblRiskPortfolio rp ON (l.advisor_id = rp.advisor_id) 

WHERE   z.id = l.zone_id
    AND rp.date = '%s'
GROUP BY z.id
ORDER BY riesgo_suma ASC;",$end_date));
$_html = "<h1>Riesgo [$start_date --- $end_date] </h1>";
$_html .= count($riesgo) ? WEBPAGE::printchart($riesgo,array_keys($riesgo[0])) : $_LABELS['noData'];
print $_html;
}

// New clients per advisor per date range
// if($_GET['report']=="extra3") {
// $new_clients = WEBPAGE::$dbh->getAll(sprintf("select count(c.id) as asociados_nuevos,u.id,concat(u.first,' ',u.last) as asesor from tblClients c,tblUsers u where u.id = c.advisor_id
// and c.creator_date >= '%s' and c.creator_date<='%s' group by u.id;",$start_date,$end_date));
// 
// $_html = "<h1>New clients</h1>";
// $_html .= count($new_clients) ? WEBPAGE::printchart($new_clients,array_keys($new_clients[0])) : $_LABELS['noData'];
// print $_html;
// }

if($_GET['report']=="04") {

$cartera_clientes = WEBPAGE::$dbh->getAll(sprintf("SELECT  tcp.zone_id   ,
        z.zone           AS zona,
        SUM(tcp.clients) AS cartera_clientes
FROM    tblClientPortfolio tcp,
        tblZones z
WHERE   z.id     = tcp.zone_id
    AND tcp.date = '%s'
GROUP BY tcp.zone_id
ORDER BY cartera_clientes DESC;",$end_date));

trace($cartera_clientes,1,'$cartera_clientes');

$_html = "<h1>Cartera Clientes</h1>";
$_html .= count($cartera_clientes) ? WEBPAGE::printchart($cartera_clientes,array_keys($cartera_clientes[0])) : $_LABELS['noData'];
print $_html;
}

// if($_GET['report']=="extra3") {
// $writeoff = WEBPAGE::$dbh->getAll(sprintf("SELECT u.id,  concat(u.first,' ',u.last) AS asesor,
//         SUM(lw.amount) as castigo_cartera,balance.cartera as cartera,round(100*(SUM(lw.amount)/balance.cartera),2) as castigo_percent
// FROM    (tblLoans l,
//         tblUsers u,(select rp.advisor_id as asesor,sum(rp.balance) as cartera from tblRiskPortfolio rp where rp.date>='%s' AND rp.date<='%s' group by rp.advisor_id) as balance)
// LEFT JOIN tblLoanWriteOff lw ON (l.id = lw.loan_id) 
// WHERE   
//     l.advisor_id = u.id
//     AND lw.date     >='%s'
//     AND lw.date     <='%s'
//     AND l.advisor_id = balance.asesor
// GROUP BY u.id ORDER BY castigo_percent asc;",$start_date,$end_date,$start_date,$end_date));
// 
// $_html = "<h1>% Castigo [$start_date --- $end_date]</h1>";
// $_html .= count($writeoff) ? WEBPAGE::printchart($writeoff,array_keys($writeoff[0])) : $_LABELS['noData'];
// print $_html;
// }

// if($_GET['report']=="05") {
// $avg_loan_amount = WEBPAGE::$dbh->getAll(sprintf("select u.id,concat(u.first,' ',u.last) as asesor,round(SUM(l.kp)/COUNT(l.id),2) as prestamo_promedio from tblLoans l,tblUsers u
//  where u.id = l.advisor_id and (l.status='G' or l.status='LI' or l.status='LO' or l.status='C') 
// and
//  l.delivered_date>='%s' and l.delivered_date<='%s' group by u.id;",$start_date,$end_date));
// 
// $_html = "<h1>Average Loan Amount [$start_date --- $end_date]</h1>";
// $_html .= count($avg_loan_amount) ? WEBPAGE::printchart($avg_loan_amount,array_keys($avg_loan_amount[0])) : $_LABELS['noData'];
// print $_html;
// }

// if($_GET['report']=="10") {
// $ingresos_operativos = WEBPAGE::$dbh->getAll(sprintf("SELECT  u.id,concat(u.first,' ',u.last) as asesor,
//         (SUM(tp.penalties) + SUM(tp.interest) + SUM(tp.insurances) + SUM(tp.fees)) as ingresos        
// FROM    (tblLoans l    ,
//         tblUsers u)    
//         LEFT JOIN tblPayments tp ON (l.id = tp.loan_id)
//         
// WHERE   u.id              = l.advisor_id
//     AND l.id              = tp.loan_id
//     AND l.delivered_date >= '%s'
//     AND l.delivered_date <= '%s'
// GROUP BY u.id order by ingresos desc;",$start_date,$end_date));
// 
// 
// $_html = "<h1>Ingresos Operativos [$start_date --- $end_date]</h1>";
// $_html .= count($ingresos_operativos) ? WEBPAGE::printchart($ingresos_operativos,array_keys($ingresos_operativos[0])) : $_LABELS['noData'];
// print $_html;
// }

// Snapshots al final de la ventana de tiempo.
// if($_GET['report']=="01") {
// $monto_desembolsado = WEBPAGE::$dbh->getAll(sprintf("select u.id,concat(u.first,' ',u.last) AS asesor,sum(l.kp) as monto_desembolsado from tblUsers u,tblLoans l where u.id = l.advisor_id and l.delivered_date >='%s' and l.delivered_date <='%s' group by l.advisor_id;",$start_date,$end_date));
// //round( 100 * ( sum( rp.riskB ) / sum( rp.balance ) ) , 2 )
// 
// $_html = "<h1>Monto Desembolsado [$start_date --- $end_date]</h1>";
// $_html .= count($monto_desembolsado) ? WEBPAGE::printchart($monto_desembolsado,array_keys($monto_desembolsado[0])) : $_LABELS['noData'];
// print $_html;
// }

if($_GET['report']=="02") {
$disbursed_loans = WEBPAGE::$dbh->getAll(sprintf("select z.id,z.zone as Zona,count(lsh.id) as cantidad_prestamos from tblLoanStatusHistory lsh,tblLoans l,tblZones as z
 where z.id = l.zone_id and l.id = lsh.loan_id and lsh.date >= '%s'
 and lsh.date <= '%s' and lsh.status='G' group by z.id order by cantidad_prestamos desc;",$start_date,$end_date));

$_html = "<h1>Cantidad de Prestamos [$start_date --- $end_date]</h1>";
$_html .= count($disbursed_loans) ? WEBPAGE::printchart($disbursed_loans,array_keys($disbursed_loans[0])) : $_LABELS['noData'];
print $_html;
}

if($_GET['report']=="08") {
$active_loans = WEBPAGE::$dbh->getAll(sprintf("SELECT u.id                      ,
        concat(u.first,' ',u.last) as asesor    ,
        SUM(IF(lsh.status='G',1,0))  AS entregado,
        SUM(IF(lsh.status='C',1,0))  AS cancelado,
        SUM(IF(lsh.status='LI',1,0)) AS legal_in ,
        SUM(IF(lsh.status='LO',1,0)) AS legal_out,
(
        SUM(IF(lsh.status='G',1,0))-
        SUM(IF(lsh.status='C',1,0))-
        SUM(IF(lsh.status='LI',1,0))
) AS activos
        FROM    tblUsers u,tblLoans l,tblLoanStatusHistory lsh
WHERE   /*lsh.date>='%s'*/
        lsh.date<='%s'
    AND u.id = l.advisor_id
    AND l.id = lsh.loan_id
GROUP BY u.id order by activos desc;",$start_date,$end_date));

$_html = "<h1>Clientes con Prestamos [$start_date --- $end_date]</h1>";
$_html .= count($active_loans) ? WEBPAGE::printchart($active_loans,array_keys($active_loans[0])) : $_LABELS['noData'];
print $_html;
}

if($_GET['report']=="03") {
$cartera_vigente = WEBPAGE::$dbh->getAll(sprintf("SELECT  z.id,
        z.zone     AS Zona,
        sum(rp.balance) as cartera_vigente,
        rp.date
FROM    tblRiskPortfolio rp,
        tblZones z
WHERE   z.id    = rp.zone_id
    AND rp.date = '%s'
GROUP BY z.id
ORDER BY cartera_vigente DESC;",$end_date));

$_html = "<h1>Cartera Vigente [$start_date --- $end_date]</h1>";
$_html .= count($cartera_vigente) ? WEBPAGE::printchart($cartera_vigente,array_keys($cartera_vigente[0])) : $_LABELS['noData'];
print $_html;
}

if($_GET['report']=="extra10") {
$group_bde_ratio = WEBPAGE::$dbh->getAll(sprintf("SELECT  u.id,concat(u.first,' ',u.last) as asesor,(sum(cp.group_b)/ sum(cp.group_bg)) as group_bde_ratio,cp.date
FROM    tblClientPortfolio cp,
        tblUsers u
WHERE   u.id      = cp.advisor_id
    AND cp.date ='%s'
    GROUP BY u.id;",$end_date));

$_html = "<h1>Ratio Grupo/BDE [$start_date --- $end_date]</h1>";
$_html .= count($group_bde_ratio) ? WEBPAGE::printchart($group_bde_ratio,array_keys($group_bde_ratio[0])) : $_LABELS['noData'];
print $_html;
}

if($_GET['report']=="11") {
$client_portfolio = WEBPAGE::$dbh->getAll(sprintf("SELECT  u.id,concat(u.first,' ',u.last) as asesor,sum(cp.clients) as cartera_clientes,cp.date
FROM    tblClientPortfolio cp,
        tblUsers u
WHERE   u.id      = cp.advisor_id
    AND cp.date ='%s'
    GROUP BY u.id ORDER BY cartera_clientes DESC;",$end_date));

$_html = "<h1>Cartera Clientes [$start_date --- $end_date]</h1>";
$_html .= count($client_portfolio) ? WEBPAGE::printchart($client_portfolio,array_keys($client_portfolio[0])) : $_LABELS['noData'];
print $_html;
}


// Assessing active loans within the date range
if($_GET['report']=="07") {
$women_percent = WEBPAGE::$dbh->getAll(sprintf("select u.id,cp.date,u.id,concat(u.first,' ',u.last) as asesor,sum(cp.female) as female,sum(cp.clients) as clients,
round((sum(cp.female)/sum(cp.clients))*100,2) as female_percent
 from tblClientPortfolio cp,tblUsers u where
 u.id = cp.advisor_id and
 cp.date='%s' group by u.id order by female desc;",$end_date));

$_html .= "<h1>% de Mujeres [$start_date --- $end_date]</h1>";
$_html .= count($women_percent) ? WEBPAGE::printchart($women_percent,array_keys($women_percent[0])) : $_LABELS['noData'];
print $_html;
}

// print "<pre>";
// // foreach ($loan_summary as $key=>$value) {
// // foreach ($value as $key_inner=>$value_inner) {
// // print "$key_inner => $value_inner" . "\n";
// // }
// // }
// print count($loan_summary);
// print "</pre>";
// trace(array_keys($women_percent[0]));
// $output.="<table>";
// $output.="<tr>";
// 
// foreach (array_keys($women_percent[0]) as $key1=>$value1) {
// $output.="<td>$value1</td>";
// }
// 
// $output.="</tr>";
// 
// foreach($women_percent as $key=>$value) {
// $output.="<tr>";
// foreach($value as $key_inner=>$value_inner) {
// $output.="<td bgcolor='yellow'>$value_inner</td>";
// }
// $output.="</tr>";
// }
// 
// $output.="</table>";
// 
// print $output;



?>
</body>
</html>