<!--<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">-->
<?php
include_once 'includes/trace.debugger.php';
error_reporting(E_ALL);
$today=getdate();
$today_month=date('m',$today[0]);
$today_year=date('Y',$today[0]);

$month=$_GET['month'];
$year=$_GET['year'];


if((!isset($_GET['month'])) || (!isset($_GET['year'])))
{
//     $today=getdate();
//     $month=$today['mon'];
//     $year=$today['year'];

    $month="00";
    $year="0000";
    $alert_message='Need to select both Month and Year';
    $checkDateFail=1;
}

elseif (($month=="") || ($year=="")) {

// $today=getdate();
//     $month=$today['mon'];
//     $year=$today['year'];

    $month="00";
    $year="0000";
    $alert_message='Need to select both Month and Year';
    $checkDateFail=1;
}

elseif ((!eregi('(^[0-9]{2,2})',$month)) || (!eregi('(^[0-9]{4,4})',$year)) ) {

$month="00";
$year="0000";
$alert_message='Need to select Month and Year properly ie: 01 2007';
$checkDateFail=1;
}

elseif (($month>$today['mon']) || ($year>$today['year'])) {
$month="00";
$year="0000";
$alert_message='Selected Month or Year out of range';
$checkDateFail=1;
} 

else {

$month=$month;
$year=$year;
}


?>
<html>
<head>
<link href="themes/blue/style.css" rel="stylesheet" type="text/css">
<title>
Esperanza Internacional - Dominican Republic - PMR
</title>
<style type="text/css">
a.hide {display:none;}
</style>

<script type="text/javascript">

function preselect() {

document.getElementById('month<?php echo $month; ?>').selected="selected";
document.getElementById('year<?php echo $year; ?>').selected="selected";
return true;
}

function checkDate() {

alert("<?php echo $alert_message; ?>");
document.getElementById('month00').selected="selected";
document.getElementById('year0000').selected="selected";
return true;    

}


</script>

</head>

<body onload="<?php if($checkDateFail==1) {echo 'checkDate()';} else {echo 'preselect()';}; ?>">

<form action="index.hopestats.php" method="get">
<table><tr><td>
<select id="month" name="month">
    <option id="month00" value="">Select Month</option>
    <option id="month01" value="01">January</option>
    <option id="month02" value="02">February</option>
    <option id="month03" value="03">March</option>
    <option id="month04" value="04">April</option>
    <option id="month05" value="05">May</option>
    <option id="month06" value="06">June</option>
    <option id="month07" value="07">July</option>
    <option id="month08" value="08">August</option>
    <option id="month09" value="09">September</option>
    <option id="month10" value="10">October</option>
    <option id="month11" value="11">November</option>  
    <option id="month12" value="12">December</option>
</select></td><td>&nbsp;</td><td><br /><br /><input type="submit" value="submit" /></td>
</tr>
<tr>
<td>
<select id="year" name="year">
    <option id="year0000" value="">Select Year</option>
    <option id="year2007" value="2007">2007</option>
    <option id="year2008" value="2008">2008</option>
    <option id="year2009" value="2009">2009</option>
    <option id="year2010" value="2010">2010</option>
</select>
</td>
</tr>
</table>

</form>
<p></p>
<?php
//error_reporting(E_ERROR);
require_once 'includes/trace.debugger.php';
include_once 'includes/index.hopestats.data.inc';
require_once 'HTML/Template/ITX.php';
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once './includes/ST.LIB.login.inc';
require_once ("Mail.php");
require_once ("Mail/mime.php"); 
include_once ("Spreadsheet/Excel/Writer.php"); 

WEBPAGE::START();
/*
eng - english
esp - spanish
fra - french
*/

$link=WEBPAGE::$dbh;

if(isset($_GET['lang'])){
WEBPAGE::$lang   = $_GET['lang'];
} else {

WEBPAGE::$lang   = 'eng';

}

$_LABELS         = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_LABELS_FILE,WEBPAGE::$lang));
$_CONF           = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_CONF_FILE,WEBPAGE::$lang));

/*
Breadth of Outreach
===================
 */
// tblClientPortfolio
// Number of Active Male Clients:A1

// $data['male_count'] += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT count(id) from tblClients as c where c.gender = 'M' and c.advisor_id > 0 AND c.zone_id > 0;"))));

$data['Breadth_Of_Outreach']['Number_of_Active_Male_Clients'] += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT sum(cp.male) FROM tblClientPortfolio as cp WHERE cp.date=(select max(date) from tblClientPortfolio where month(date)=%s AND year(date)=%s)",$month,$year))));

// Number of Active Female Clients:A2

// $data['month_female_count'] += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT count(id) from tblClients as c where c.gender = 'F' and c.advisor_id > 0 AND c.zone_id > 0;"))));

$data['Breadth_Of_Outreach']['Number_of_Active_Female_Clients'] += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT sum(cp.female) FROM tblClientPortfolio as cp WHERE cp.date=(select max(date) from tblClientPortfolio where month(date)=%s AND year(date)=%s)",$month,$year))));


// Total Active Clients:A3

// $data['month_total_active'] += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT count(id) from tblClients as c where c.advisor_id > 0 AND c.zone_id > 0;"))));

$data['Breadth_Of_Outreach']['Total_Active_Clients'] += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT sum(cp.clients) FROM tblClientPortfolio as cp WHERE cp.date=(select max(date) from tblClientPortfolio where month(date)=%s AND year(date)=%s)",$month,$year))));



// Total number of loans disbursed this month:A4

$data['Breadth_Of_Outreach']['Total_Number_of_Loans_Disbursed_this_Month']  += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT count(lsh.id) from tblLoanStatusHistory as lsh where MONTH(lsh.date)=%s AND YEAR(lsh.date)=%s AND lsh.status='G';",$month,$year))));

// Total number of loans disbursed, inception to present:A5

$data['Breadth_Of_Outreach']['Total_Number_of_Loans_Disbursed_inception_to_present']  += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT count(lsh.id) + %s from tblLoanStatusHistory as lsh where lsh.date <= LAST_DAY('%s-%s-01') AND lsh.status = 'G' ;",$_CONF['ini.number_of_loans'],$year,$month))));


// Number of branch offices:A6

$data['Breadth_Of_Outreach']['Number_of_Branch_Offices'] += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT count(*)-1 from tblZones as z where z.parent_id = 0 and z.creator_date <= LAST_DAY('%s-%s-01');",$year,$month))));

// Net (Gross) portfolio outstanding:A7

// $data['gross_portfolio'] += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT sum(lcd.balance_kp) from tblLoansCurrentData as lcd,tblLoanStatusHistory as lsh,tblLoans as l where lcd.loan_id = lsh.loan_id AND l.id = lcd.loan_id AND MONTH(l.delivered_date)<=%s AND YEAR(l.delivered_date)<=%s AND lsh.status = 'G';",$month,$year))));

$data['Breadth_Of_Outreach']['Gross_Portfolio_Outstanding'] += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT sum(rp.balance) FROM tblRiskPortfolio as rp WHERE rp.date=(select max(date) from tblRiskPortfolio where month(date)=%s AND year(date)=%s);",$month,$year))));

// Total Amount of Loans Disbursed this Month:A8

$data['Breadth_Of_Outreach']['Total_Amount_of_Loans_Disbursed_this_Month']  += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT sum(l.kp) from tblLoans as l,tblLoanStatusHistory as lsh where l.id = lsh.loan_id AND MONTH(l.delivered_date)=%s AND YEAR(l.delivered_date)=%s AND lsh.status='G';",$month,$year))));

// Total Amount of Loans Disbursed, inception to present:A9

$data['Breadth_Of_Outreach']['Total_Amount_of_Loans_Disbursed_inception_to_present']  += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT sum(l.kp) + %s from tblLoans as l,tblLoanStatusHistory as lsh where l.id = lsh.loan_id AND l.delivered_date <= LAST_DAY('%s-%s-01') AND lsh.status = 'G' ;",$_CONF['ini.amount_disbursed'],$year,$month))));

/*
Depth of Outreach
===================
 */

// Value of Loans Disbursed for Active Clients before Repayment (first loaners).

// $data['loans_amount_first_loaners']  += current(current(WEBPAGE::$dbh->getAll(sprintf("select l.kp as amount,l.id as loan_id,l.client_id from tblLoans as l,
// (select count(id) as count_loans,if(count(id)=1,'no','yes') as reloan,
// client_id from
//  tblLoans group by client_id) as newloans,
// tblClients as c
//  where c.id = l.client_id AND c.advisor_id > 0 and c.zone_id > 0 and l.client_id = newloans.client_id and newloans.reloan='no' and MONTH(l.delivered_date) = %s AND YEAR(l.delivered_date) = %s and l.status = 'G';",$month,$year))));

/*
 SELECT SUM(l.kp) as amount,COUNT(l.id) as hits,round(SUM(l.kp)/COUNT(l.id),2) as lavg FROM tblLoans l, (SELECT t.client_id,COUNT(t.id),t.id FROM tblLoans t GROUP BY t.client_id) fst WHERE l.id = fst.id AND MONTH(l.delivered_date) = '08' AND YEAR(l.delivered_date) = '2007' and (l.status='G' or l.status='LI' or l.status='LO' or l.status='C') ;

*/

$data_aux = (WEBPAGE::$dbh->getAll(sprintf("SELECT SUM(l.kp) as amount,COUNT(l.id) as hits,round(SUM(l.kp)/COUNT(l.id),2) as lavg FROM tblLoans l, (SELECT t.client_id,COUNT(t.id),t.id FROM tblLoans t GROUP BY t.client_id) fst WHERE l.id = fst.id AND MONTH(l.delivered_date) = %s AND YEAR(l.delivered_date) = %s and (l.status='G' or l.status='LI' or l.status='LO' or l.status='C');",$month,$year)));




// $data['month_loans_amount_first_loaners']  += current(current(WEBPAGE::$dbh->getAll(sprintf("select sum(tblLoans.kp) from tblLoans,(select count(l.id) as hits,max(l.id) as max_id,l.client_id from tblLoans as l,tblClients as c where c.id=l.client_id and c.advisor_id>0 and month(l.delivered_date)=%s and year(l.delivered_date)=%s group by l.client_id) as loan_first where tblLoans.client_id = loan_first.client_id and tblLoans.id=loan_first.max_id and loan_first.hits =1;",$month,$year))));

$data['Depth_Of_Outreach']['Value_of_Loans_Disbursed_for_Active_Clients_before_repayment']=$data_aux[0]['amount'];

// Average Loan Size - Cycle 1

// $data['average_loan_size_cycle1']  += current(current(WEBPAGE::$dbh->getAll(sprintf("select count(l.id) as loan_count,sum(l.kp) as sum_amount,(sum(l.kp)/count(l.id)) as loan1_avg from tblLoans as l,
// (select count(id) as count_loans,if(count(id)=1,'no','yes') as reloan,
// client_id from
//  tblLoans group by client_id) as newloans,
// tblClients as c
//  where c.id = l.client_id AND c.advisor_id > 0 and c.zone_id > 0 and l.client_id = newloans.client_id and newloans.reloan='no' and MONTH(l.delivered_date) = %s AND YEAR(l.delivered_date) = %s and l.status = 'G';",$month,$year))));

$data['Depth_Of_Outreach']['Average_Loan_Size_Cycle_1']=$data_aux[0]['lavg'];

// Average Loan Size

$data_aux=WEBPAGE::$dbh->getAll(sprintf("SELECT SUM(l.kp) as amount,COUNT(l.id) as hits,round(SUM(l.kp)/COUNT(l.id),2) as loan_avg FROM tblLoans l WHERE MONTH(l.delivered_date) = %s AND YEAR(l.delivered_date) = %s and (l.status='G' or l.status='LI' or l.status='LO' or l.status='C');",$month,$year));


$data['Depth_Of_Outreach']['Average_Loan_Size']  = $data_aux[0]['loan_avg'];

// % of Clients who are Women

$data['Depth_Of_Outreach']['%_of_Clients_who_are_Women']=($data['Breadth_Of_Outreach']['Number_of_Active_Female_Clients'] / $data['Breadth_Of_Outreach']['Total_Active_Clients'])*100;

// % of Client business that are startups


$data_aux['total_business_count']+=current(current(WEBPAGE::$dbh->getAll(sprintf("select count(b.id) from tblClients as c,tblLoans as l,tblLoanStatusHistory as lsh,tblBusiness as b where 
c.advisor_id > 0 and c.zone_id > 0 and c.id = l.client_id and l.id = lsh.loan_id and lsh.status = 'G' and l.business_id = b.id and b.creator_date<=LAST_DAY('%s-%s-01')",$year,$month))));

$data_aux['month_startup_client_business_count']+= current(current(WEBPAGE::$dbh->getAll(sprintf("select count(b.id) from tblClients as c,tblLoans as l,tblLoanStatusHistory as lsh,tblBusiness as b where 
c.advisor_id > 0 and c.zone_id > 0 and c.id = l.client_id and l.id = lsh.loan_id and lsh.status = 'G' and l.business_id = b.id and MONTH(b.creator_date)= %s and YEAR(b.creator_date)= %s",$month,$year))));


$data['Depth_Of_Outreach']['%_of_Client_businesses_that_are_startups'] = ($data_aux['month_startup_client_business_count']/$data_aux['total_business_count'])*100;



// $data['month_startup_client_business_count']+= current(current(WEBPAGE::$dbh->getAll(sprintf("select count(b.id) from tblClients as c,tblLoans as l,tblLoanStatusHistory as lsh,tblBusiness as b where 
// c.advisor_id > 0 and c.zone_id > 0 and c.id = l.client_id and l.id = lsh.loan_id and lsh.status = 'G' and l.business_id = b.id and MONTH(b.creator_date)= %s and YEAR(b.creator_date)= %s",$month,$year))));



/*
Institutional Sustainability
============================
*/

// Earned income (interest + fees)

// $data['earned_income']  += current(current(WEBPAGE::$dbh->getAll(sprintf("select sum(tp.interest + tp.fees) as interest_fees from tblPayments as tp where month(tp.date) = %s and year(tp.date) = %s;",$month,$year))));

$data['Institutional_Sustainability']['Earned_income_(interest_+_fees)']  += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT (sum(tc.interest)+sum(tc.penalties)+sum(tc.fees)) FROM tblTCredits as tc WHERE MONTH(date)=%s AND YEAR(date)=%s;",$month,$year))));

$data['Institutional_Sustainability']['Total Expenses']=$accounting[$year][$month]['total_expenses'];
$data['Institutional_Sustainability']['Annual Rate of Inflation %']=$accounting[$year][$month]['annual_inflation_rate'];
$data['Institutional_Sustainability']['Adjustment: Inflation Portfolio Devaluation']=$accounting[$year][$month]['adj_inflation_portfolio_devaluation'];
$data['Institutional_Sustainability']['Cash Estimate']=$accounting[$year][$month]['cash_estimate'];
$data['Institutional_Sustainability']['Adjustment: Inflation Cash Devaluation']=$accounting[$year][$month]['adj_inflation_cash_devaluation'];
$data['Institutional_Sustainability']['Total Adjustment']=$accounting[$year][$month]['total_adjustment'];
$data['Institutional_Sustainability']['Operational Self-Sufficiency %']=$accounting[$year][$month]['operational_self_sufficiency'];
$data['Institutional_Sustainability']['Financial Self-Sufficiency %']=$accounting[$year][$month]['finantial_self_sufficiency'];



/*
Institutional Effectiveness
============================
 */

// Amount of payments more than 30 days late

// $data['month_late_payments_more_than_30_days']  += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT sum(lod.pmt - lod.penalties) as sum_pmt
// FROM tblLoans AS l, tblLoansOnDelinquency AS lod, (
// SELECT max( id ) AS id, loan_id
// FROM tblLoansOnDelinquency
// WHERE month( date ) = %s
// AND year( date ) = %s
// GROUP BY loan_id
// ) AS lodmax
// WHERE l.id = lodmax.loan_id
// AND lodmax.loan_id = lod.loan_id
// AND lodmax.id = lod.id
// AND lod.delay >30;",$month,$year))));


$data['Institutional_Effectiveness']['Amount of payments more than 30 days late']  += current(current(WEBPAGE::$dbh->getAll(sprintf("select sum(lod.pmt) from tblLoansOnDelinquency as lod,tblLoanTypes as lt,tblLoans as l where lod.date=(select max(date) from tblLoansOnDelinquency where month(date) = %s and year(date)=%s) and lod.loan_id = l.id and l.loan_type_id = lt.id and ((lod.delay/lod.hits)+((if(lt.payment_frequency='W',7,if(lt.payment_frequency='BW',14,if(lt.payment_frequency='M',28,if(lt.payment_frequency='Q',84,if(lt.payment_frequency='SA',168,if(lt.payment_frequency='A',336,14)))))))*(lod.hits-1))/2)>30;;",$month,$year))));


// Value of all loans outstanding that have one or more installments of principal past 30 days
// Valor de todos los prestamos que tienen 1 o mas coutas atrasadas con mas de 30 dias de atraso.
// Value = tblLoans.kp @todo

$data['Institutional_Effectiveness']['Value of all loans outstanding that have one or more installments of principal past 30 days']+=current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT round(sum(rp.riskB),2) as amount_par30
 from tblRiskPortfolio as rp,(select max(date) as date from tblRiskPortfolio where month(tblRiskPortfolio.date)=%s and year(tblRiskPortfolio.date)=%s) as rp_month
 where rp.date=rp_month.date;",$month,$year))));



// Arrears Rate
$data['Institutional_Effectiveness']['Arrears'] += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT sum(lod.principal) as sum_principal
FROM tblLoans AS l, tblLoansOnDelinquency AS lod, (
SELECT max( id ) AS id, loan_id
FROM tblLoansOnDelinquency
WHERE month( date ) = %s
AND year( date ) = %s
GROUP BY loan_id
) AS lodmax
WHERE l.id = lodmax.loan_id
AND lodmax.loan_id = lod.loan_id
AND lodmax.id = lod.id
AND lod.hits >=2;",$month,$year))));



// Portfolio-At-Risk

// $data['portfolio_risk'] += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT sum(lcd.balance_kp) as sum_balance_kp
// FROM tblLoansCurrentData AS lcd, tblLoansOnDelinquency AS lod, (
// SELECT max( id ) AS id, loan_id
// FROM tblLoansOnDelinquency
// WHERE month( date ) = %s
// AND year( date ) = %s
// GROUP BY loan_id
// ) AS lodmax
// WHERE lcd.loan_id = lodmax.loan_id
// AND lodmax.loan_id = lod.loan_id
// AND lodmax.id = lod.id
// AND lod.hits >=2;",$month,$year))));


$data['Institutional_Effectiveness']['Portfolio-At-Risk_PAR30'] += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT round(100*(sum(rp.riskB)/sum(rp.balance)),2) as total_risk_B from tblRiskPortfolio as rp where rp.date=(select max(tblRiskPortfolio.date) from tblRiskPortfolio where month(tblRiskPortfolio.date)=%s and year(tblRiskPortfolio.date)=%s);",$month,$year))));



// SELECT round(100*(sum(rp.riskB)/sum(rp.balance)),2) as total_risk_B
// from tblRiskPortfolio as rp
//  where month(rp.date)='08' and year(rp.date)='2007';


// Retention Rate

// calculating attrition

$data_aux['attrition'] =current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT COUNT(id) FROM tblClients WHERE zone_id = 0 OR advisor_id = 0"))));

$data['Institutional_Effectiveness']['Retention Rate']=($data_aux['attrition'] + $data['Breadth_Of_Outreach']['Total_Active_Clients']) ? round(100*(1-($data_aux['attrition']/($data_aux['attrition'] + $data['Breadth_Of_Outreach']['Total_Active_Clients']))),2) : 0;

/*
Institutional Efficiency
=========================
*/

// Number of Loan Officers

// $data['Institutional_Efficiency']['Number_of_Loan_Officers']  += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT count(id)
//  FROM tblUsers
//  WHERE access_code = 3 and memo not like '%s' and memo not like '%s' and memo not like '%s';",'%foreign%','%seattle%','%System%'))));

$data['Institutional_Efficiency']['Number_of_Loan_Officers'] = $accounting[$year][$month]['loan_officers_count'];


// Number of Total Staff Members

// $data['Institutional_Efficiency']['Number_of_Total_Staff_Members']  += current(current(WEBPAGE::$dbh->getAll(sprintf("SELECT count(id)
//  FROM tblUsers WHERE access_code !=0 and memo not like '%s' and memo not like '%s' and memo not like '%s' 
// ;",'%foreign%','%seattle%','%System%'))));

//$accounting[$year][$month]

$data['Institutional_Efficiency']['Number_of_Total_Staff_Members']  = $accounting[$year][$month]['total_staff_count'];

// Cost/Dollar Lent

$data['Institutional_Efficiency']['Cost/Dollar_Lent']=$accounting[$year][$month]['cost_dollar_lent'];

// Cost/Loan Disbursed

$data['Institutional_Efficiency']['Cost/Loan_Disbursed']=$accounting[$year][$month]['cost_loan_disbursed'];

//Active Clients/Loan Officer

$data['Institutional_Efficiency']['Active Clients/Loan Officer']=!($data['Institutional_Efficiency']['Number_of_Loan_Officers']=='-')? $data['Breadth_Of_Outreach']['Total_Active_Clients']/$data['Institutional_Efficiency']['Number_of_Loan_Officers']:'-';

// Active Clients/Staff Member

$data['Institutional_Efficiency']['Active Clients/Staff Member']=!($data['Institutional_Efficiency']['Number_of_Total_Staff_Members']=='-')?$data['Breadth_Of_Outreach']['Total_Active_Clients']/$data['Institutional_Efficiency']['Number_of_Total_Staff_Members']:'-';

// Portfolio/Loan Officer

$data['Institutional_Efficiency']['Portfolio/Loan Officer']=!($data['Institutional_Efficiency']['Number_of_Loan_Officers']=='-') ? $data['Breadth_Of_Outreach']['Gross_Portfolio_Outstanding']/$data['Institutional_Efficiency']['Number_of_Loan_Officers']:'-';



// // Number of Children

$head = array('fact'=>"Esperanza International - Dominican Republic - PMR",'value'=>"&nbsp;&nbsp;");



$ldata=array();
$row=0;
foreach($data as $header=>$parameter) {
$ldata[$row][$header.'_label']=sprintf('<table><tr><td><h1>%s</h1></td></tr></table>',str_replace('_','  ',$header));
$ldata[$row][$header.'_html']="&nbsp;&nbsp;";
$row++;
        foreach($parameter as $label=>$value) {
            $ldata[$row][$label.'_label']=str_replace('_',' ',$label);
        if($value=="-") {$ldata[$row][$label]="-";}
            elseif(is_float($value)) {$ldata[$row][$label]=number_format($value, 2, '.', ',');}
            else {$ldata[$row][$label]=number_format($value, 2, '.', ',');}
      $row++;
        }

}




$_html.= count($data) ? WEBPAGE::printchart($ldata,$head) : $_LABELS['noData'];
print $_html;
/*
Accounting data
================
*/

?>
<p>* Monetary values in dominican pesos (DOP) unless otherwise specified</p>
</body>
</html>             
