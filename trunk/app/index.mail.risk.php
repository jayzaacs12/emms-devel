<?php
//error_reporting(E_ERROR);
error_reporting(E_ALL);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once './includes/ST.LIB.login.inc';
//include_once 'includes/trace.debugger.php';
require_once ("Mail.php");
require_once ("Mail/mime.php"); 

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


$cdata = WEBPAGE::$dbh->getAll(sprintf("SELECT z.id, rp.date AS now_date, z.zone, trp.id AS trp_zone_id, (
round( 100 * ( sum( rp.riskB ) / sum( rp.balance ) ) , 2 )
) AS risk_B, trp.fmrRiskB AS fmr_riskB, trp.date AS fmrdate, lfrp.id as lfrp_zone_id,lfrp.lfRiskB, lfrp.date AS lfdate
FROM (

SELECT z.id, rp2.date, z.zone, (
round( 100 * ( sum( rp2.riskB ) / sum( rp2.balance ) ) , 2 )
) AS fmrRiskB
FROM `tblRiskPortfolio` AS rp2, tblZones AS z
WHERE rp2.date = date_sub( curdate( ) , INTERVAL 1
DAY )
AND rp2.zone_id = z.id
GROUP BY zone_id
) AS trp, (

SELECT z2.id, rp3.date, z2.zone, (
round( 100 * ( sum( rp3.riskB ) / sum( rp3.balance ) ) , 2 )
) AS lfRiskB
FROM `tblRiskPortfolio` AS rp3, tblZones AS z2
WHERE rp3.date = date_add( (
date_sub( curdate( ) , interval( 1 ) week ) ) , interval( 6 - dayofweek( date_sub( curdate( ) , interval( 1 ) week ) ) )
DAY
)
AND rp3.zone_id = z2.id
GROUP BY zone_id
) AS lfrp, tblRiskPortfolio AS rp, tblZones AS z
WHERE rp.date = curdate( )
AND rp.zone_id = z.id
AND rp.zone_id = trp.id
AND rp.zone_id = lfrp.id
GROUP BY zone_id
ORDER BY risk_B;
"));

$total_cdata = WEBPAGE::$dbh->getAll(sprintf("SELECT rp.date AS now_date, (
round( 100 * ( sum( rp.riskB ) / sum( rp.balance ) ) , 2 )
) AS total_risk_B, trp.fmrRiskB AS total_fmr_riskB, trp.date AS fmrdate,lfrp.lfRiskB as total_lfriskB,lfrp.date as lfdate
FROM (

SELECT rp2.date, (
round( 100 * ( sum( rp2.riskB ) / sum( rp2.balance ) ) , 2 )
) AS fmrRiskB
FROM `tblRiskPortfolio` AS rp2
WHERE rp2.date = date_sub( curdate( ) , INTERVAL 1
DAY )
GROUP BY rp2.date
) AS trp,
(

SELECT rp3.date,(
round( 100 * ( sum( rp3.riskB ) / sum( rp3.balance ) ) , 2 )
) AS lfRiskB
FROM `tblRiskPortfolio` AS rp3
WHERE rp3.date = date_add( (
date_sub( curdate( ) , interval( 1 ) week ) ) , interval( 6 - dayofweek( date_sub( curdate( ) , interval( 1 ) week ) ) )
DAY
)

GROUP BY rp3.date
) AS lfrp,
tblRiskPortfolio AS rp
WHERE rp.date = curdate( )
GROUP BY rp.date
ORDER BY total_risk_B;
"));



$_html .= <<<CHTML

<head>
<title> Mail tblRiskPortfolio </title>
<style type="text/css">

table { font-size:10px; border-width:0; font-size:11px; margin-left:0px; background-color:#E5EBFB; }
td, th { padding:0; font-size:10px;}
td.header { background-color:#68A0BC; font-weight:bold; color:white; padding: 2px 10px 2px 10px; text-align:center; }
td.label { background-color:white; font-weight:bold; color:#336699; padding: 4px 20px 4px 0px; text-align:left; vertical-align:top; }
td.chart { background-color:white; padding:2px 5px 2px 5px; }
td.totals { padding:2px 5px 2px 5px; font-weight: bold; text-align: right;}
td.legend { background-color:white;padding:1px 5px 1px 5px;}



td.data {background-color:white;font-size:20px;text-align:center;text-color:red;border-style:inset;border-color:#68A0BC; padding:0px 0px 0px 0px;}
th.alert {text-align:center;padding-left:2cm;}
td.alert {text-align:center;padding-left:2cm;}

#legend {
    position: absolute;
    top: 249;
    left: 200;
    width: 10em;
}
</style>
</head>
<body>

CHTML;

$_html .= '<table>';

$_html .= sprintf('<tr class="header"><td>&nbsp;</td><td class="header" colspan="2">%s%s</td></tr>','PAR 30 - ',$_LABELS['SP.SCR.portfolioAtRisk']);

$_html .= sprintf('<tr class="header"><td class="header">%s</td><td class="header">%s</td><td class="header">%s</td><td class="header">%s</td><td class="header">&nbsp;Status</td></tr>',$_LABELS['tblZones.zone'],$cdata[0]['lfdate'],$cdata[0]['fmrdate'],$cdata[0]['now_date']);

$_template = '<tr><td class="label">%2$s</td><td class="chart">%5$s%%</td><td class="chart">%3$s%%</td><td class="chart">%4$s%%</td><td class="chart" style="background-color:%1$s">&nbsp;</td></tr>';

foreach ($cdata as $value) {

    $rank=$value['risk_B'];
    
    switch ($rank) {
    
        case ($rank < 3):
            $_html .= sprintf($_template,'green',$value['zone'],$value['fmr_riskB'],$value['risk_B'],$value['lfRiskB']);
        break;
        
        case ($rank >= 3 && $rank < 4):
            $_html .= sprintf($_template,'yellow',$value['zone'],$value['fmr_riskB'],$value['risk_B'],$value['lfRiskB']);
        break;
        
        case ($rank >= 4 && $rank < 5 ):
            $_html .= sprintf($_template,'orange',$value['zone'],$value['fmr_riskB'],$value['risk_B'],$value['lfRiskB']);
        break;
        
        case ($rank >= 5):
            $_html .= sprintf($_template,'red',$value['zone'],$value['fmr_riskB'],$value['risk_B'],$value['lfRiskB']);
        break;        

        default:
            $_html .= sprintf($_template,'white',$value['zone'],$value['fmr_riskB'],$value['risk_B'],$value['lfRiskB']);
        break;
        
    
    }


if ($rank >= 5) {

$bAlertFlag+=1;

}

}

// Adding totals to table
$_html .= sprintf('<tr><td class="totals">%s</td><td class="totals">%s%%</td><td class="totals">%s%%</td><td class="totals">%s%%</td><td>&nbsp;</td></tr>',$_LABELS['total'],$total_cdata[0]['total_lfriskB'],$total_cdata[0]['total_fmr_riskB'],$total_cdata[0]['total_risk_B']);

$_html .= '</table>';

$_html .= sprintf('
<table>
<tr>
<td bgcolor="green">&nbsp;</td><td class="label">PAR30<sub>*</sub>&nbsp;&lt;&nbsp;3%%</td>
</tr>
<tr>
<td bgcolor="yellow">&nbsp;</td><td class="label">PAR30<sub>*</sub>&nbsp;&lt;&nbsp;4%%</td>
</tr>
<tr>
<td bgcolor="orange">&nbsp;</td><td class="label">PAR30<sub>*</sub>&nbsp;&lt;&nbsp;5%%</td>
</tr>
<tr>
<td bgcolor="red">&nbsp;</td><td class="label">PAR30<sub>*</sub>&nbsp;&gt;&nbsp;5%%</td>
</tr>
<tr><td><br></br></td></tr>
<tr><td>&nbsp;</td><td>*&nbsp;%s</td></tr>
</table>
',$_LABELS['par30']);


if ($bAlertFlag) {
$subject = sprintf('Accion inmediata - PAR 30 Report: %s',$_CONF['client_name']);}
else {$subject = sprintf('Normal - PAR 30 Report: %s',$_CONF['client_name']);}

// finishing message

$subject = strip_tags($subject);

$message = $subject;

$attachment = $_html .'</body></html>';
 
$recipients = explode(';',$_CONF['par30rpt_emailto']); 


// Additional headers
$headers["From"] = 'cgomez <cgomez@esperanza.org>';
// $headers["To"]    = 'carlosgomezsilva@gmail.com'; 
$headers["Subject"] = $subject;
$crlf = "\n";


$mime = new Mail_mime($crlf);

$mime->setTxtBody($message);
$mime->setHTMLBody($attachment);


//do not ever try to call these lines in reverse order
$body = $mime->get();
$hdr = $mime->headers($headers,true); 

$params["username"] = 'cgomez@esperanza.org';
$params["password"] = 'cg1606';
$params["auth"] = true;

$mail =& Mail::factory('mail',$params);

$res = $mail->send($recipients, $hdr, $body);

if (PEAR::isError($res)) echo 'error enviando el email';

 
?>
