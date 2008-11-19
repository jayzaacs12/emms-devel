<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/client.php';
require_once 'class/society.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';

$btn  = new TTFButton(WEBPAGE::$theme);

if ($_POST[apply]) {

  if ( isset($_POST[attendance]) ) {
    $attendanceData = $_POST[ma];
    unset($_POST[attendance]);
    }
  unset($_POST[ma]);

  $loanData = $_POST[values];

  unset($_POST[values]);
  unset($_POST[Submit]);
  unset($_POST[scr_name]);
  unset($_POST[apply]);
	
  $loanIDs = array_keys($_POST);

  if (is_array($attendanceData)) {	// track meeting attendance

    $sql_values = '';
	$coma = '';
    while ( $att = each($attendanceData) ) {
	  if ( $attendanceData[$att[key]][attendance] != 1 ) { $attendanceData[$att[key]][attendance] = 0; }
	  $sql_values .= sprintf("%s ('%s','%s',%s,%s,%s,%s) ", $coma,'NULL',$attendanceData[$att[key]][attendance],$attendanceData[$att[key]][client_id],$attendanceData[$att[key]][loan_id],WEBPAGE::$userID,'CURDATE()');		
	  $coma = ',';
	  }
    $table = 'tblMeetingAttendance';
    $fields = 'id,attendance,client_id,loan_id,creator_id,date';    
	SQL::insert_mult($table,$fields,$sql_values);

    }

  if (!( $num = count($loanIDs) )) { exit; }
		
  //track loan payment
  $sql_values = '';
  $coma = '';
  for ($i=0;$i<$num;$i++) {
    if ( $loanData[$loanIDs[$i]][new_balance] <= 0) { $CloanIDs[]=$loanIDs[$i]; }
	$sql_values .= sprintf("%s ('%s',%s,%s,%s,%s,%s,%s,%s,%s,'%s',%s) ", $coma,'NULL',$loanIDs[$i],'CURDATE()',$loanData[$loanIDs[$i]][payment],0,0,$loanData[$loanIDs[$i]][interes],$loanData[$loanIDs[$i]][principal],$loanData[$loanIDs[$i]][new_balance],0,WEBPAGE::$userID);		
	$coma = ',';
	}    
  $table = 'tblPayments';
  $fields = 'id,loan_id,date,payment,delay,delay_n,interes,principal,pending_principal,special,user_id';    
  SQL::insert_mult($table,$fields,$sql_values);

  if (count($CloanIDs)) {    		
    $sql = sprintf("UPDATE tblLoans set status = 'C' WHERE id IN (%s)", implode(',',$CloanIDs));
    WEBPAGE::$dbh->query($sql);
	}
/*
    require_once 'class/zprofile.php';   	
    $reference = 'Pago de Préstamo';
    $user = UPROFILE::getName($ID);
    $zone = ZPROFILE::getDescription(UPROFILE::getZoneID($ID)); 
    $date = date($_CONF[date_format]);
    for ($i=0;$i<count($loanIDs);$i++) {
      $pmt = $loanData[$loanIDs[$i]][payment];
	  $delay = 0;
	  $ra = $loanData[$loanIDs[$i]][interes];
      $ka = $loanData[$loanIDs[$i]][principal];
      $pk = $loanData[$loanIDs[$i]][pending_principal];
      $client = $loanData[$loanIDs[$i]][client];
	  include "LN.RPT.income.inc";
	  }
*/
  exit; 
  }


//no submit... 

if (!($id)) { exit; }
$sprofile = NEW SOCIETY($id);
$members = $sprofile->getMembers();
$num = count($members);

$l = 0;
$i = 0;
$floan = array();
foreach ($members AS $key => $val) {
  $loanIDs[$i] = CLIENT::loans($key,'G');
  if ( $n = count($loanIDs[$i][G]) ) { $ftable = true;} 
  for ($j=0;$j<$n;$j++) {
    $lprofile[$l] = new LOAN($loanIDs[$i][G][$j]);
    $lprofile[$l]->load_borrower($lprofile[$l]->data[client_id]);

    $data = LOAN::getPendingPaymentToDate($lprofile[$l]);
    switch (count($data)) {
	  case 0:
	    $lmessage[$i][msg] 			= 'No tiene cuotas pendientes';//no tiene cuotas pendientes
	    $lmessage[$i][loan_code] 	= $lprofile[$l]->data[loan_code];
	    $lmessage[$i][client] 		= $lprofile[$l]->data[borrower_name];
	    $lmessage[$i][client_id] 	= $lprofile[$l]->data[client_id];
	    $lmessage[$i][id] 			= $lprofile[$l]->data[id];
	    break;
	  case 1:
	    if ($data[0][special]) {
		  $lmessage[$i][msg] 		= 'No tiene cuotas pendientes'; //no tiene cuotas pendientes
		  }
		if ($data[0][delay]) {
		  $lmessage[$i][msg] 		= 'Prest. en mora'; //prestamo en mora
		  }
		$lmessage[$i][loan_code] 	= $lprofile[$l]->data[loan_code];
		$lmessage[$i][client] 		= $lprofile[$l]->data[borrower_name];
		$lmessage[$i][client_id] 	= $lprofile[$l]->data[client_id];
		$lmessage[$i][id] 			= $lprofile[$l]->data[id];
		if (!(($data[0][special])||($data[0][delay]))) { 				
		  $floan[$l]=$data[0];
		  $l++;
		  unset($lmessage[$i]);
		  }
		break;
		default:
		  $lmessage[$i][msg] 		= 'Prest. en mora'; //prestamo en mora
		  $lmessage[$i][loan_code] 	= $lprofile[$l]->data[loan_code];
		  $lmessage[$i][client] 	= $lprofile[$l]->data[borrower_name];
		  $lmessage[$i][client_id] 	= $lprofile[$l]->data[client_id];
		  $lmessage[$i][id] 		= $lprofile[$l]->data[id];
	  }
    }
  }

if (!($ftable)) { //no hay prestamos entregados activos en este banco
  $location = sprintf("Location: %sindex.popup.php?scr_name=ST.SCR.message&ico=info&msg=noActiveLoans",$_CONF[url]);
  header($location); 
  exit;
  }

$tpl->setVariable("form_title", 		$_LABELS['LN.SCR.addMultPayment']) ;
$tpl->setVariable("form_attributes", 	"name=applyForm action='index.popup.php?scr_name=LN.SCR.addMultPayment'");
$tpl->setVariable("group_label", 		$_LABELS['tblSocieties.id']) ;
$tpl->setVariable("group", 				$sprofile->data[name]) ;
$tpl->setVariable("branch_label", 		$_LABELS['tblSocieties.zone_id']) ;
$tpl->setVariable("branch", 			$sprofile->data[zone]) ;
$tpl->setVariable("advisor_label", 		$_LABELS['tblSocieties.advisor_id']) ;
$tpl->setVariable("advisor", 			$sprofile->data[advisor]) ;
$tpl->setVariable("date", 				date($_CONF[date_format])) ;

$tpl->setVariable("client_label", 		$_LABELS['tblClients.id']) ;
$tpl->setVariable("loan_label", 		$_LABELS['tblLoans.id']) ;
$tpl->setVariable("pBalance_label", 	$_LABELS['previousBalance']) ;
$tpl->setVariable("principal_label", 	$_LABELS['tblPayments.principal']) ;
$tpl->setVariable("interest_label", 	$_LABELS['tblPayments.interes']) ;
$tpl->setVariable("payment_label", 		$_LABELS['tblPayments.payment']) ;

$tpl->setVariable("attendance_label", 	'Track attendance') ;
$tpl->setVariable("attendance", 		"<input type='checkbox' name=attendance value='1' class=checkbox checked>\n") ;

foreach ($floan AS $i=>$val) {
	
  $href = sprintf("javascript:openWin('index.popup.php?scr_name=LN.SCR.pmtPlan&id=%s','PLAN','menubar=no,scrollbars=yes,resizable=no,width=800,height=600')", $lprofile[$i]->data[id]);
  $pmt_plan_link = sprintf("<a href=%s onmouseover=\"self.status='%s'; return true;\" onmouseout=\"window.status=''\" onClick=\"window.status=''\">%s</a>", $href, $_LABELS['details'], $lprofile[$i]->data[loan_code]);
  
  $tpl->setCurrentBlock("results");
  
  $att_pars  = sprintf("<input type=hidden 		name=ma[%s][client_id] 		value=%s>\n",							$lprofile[$i]->data[id], $lprofile[$i]->data[client_id]);
  $att_pars .= sprintf("<input type=hidden 		name=ma[%s][loan_id] 		value=%s>\n",							$lprofile[$i]->data[id], $lprofile[$i]->data[id]);
  $att_pars .= sprintf("<input type='checkbox' 	name=ma[%s][attendance] 	value='1' class=checkbox checked>\n",	$lprofile[$i]->data[id]);
  
  $tpl->setVariable("att_pars", $att_pars) ;

  $pmt_pars  = sprintf("<input type=hidden name=values[%s][payment] 		value='%s'>\n",								$lprofile[$i]->data[id], 	$floan[$i][payment]);
  $pmt_pars .= sprintf("<input type=hidden name=values[%s][interes] 		value='%s'>\n",								$lprofile[$i]->data[id], 	$floan[$i][interes]);
  $pmt_pars .= sprintf("<input type=hidden name=values[%s][principal] 		value='%s'>\n",								$lprofile[$i]->data[id], 	$floan[$i][principal]);
  $pmt_pars .= sprintf("<input type=hidden name=values[%s][prev_balance] 	value='%s'>\n",								$lprofile[$i]->data[id], 	$floan[$i][prev_balance]);
  $pmt_pars .= sprintf("<input type=hidden name=values[%s][new_balance] 	value='%s'>\n",								$lprofile[$i]->data[id], 	$floan[$i][new_balance]);
  $pmt_pars .= sprintf("<input type=hidden name=values[%s][client] 			value='%s'>\n",								$lprofile[$i]->data[id], 	$lprofile[$i]->data[borrower_name]);
  $pmt_pars .= sprintf("<input type='checkbox' 	name='%s' 					value='1' checked class=checkbox>\n",		$lprofile[$i]->data[id]);
  
  $tpl->setVariable("pmt_pars", $pmt_pars) ;

  $tpl->setVariable("client", 		$lprofile[$i]->data[borrower_name]) ;
  $tpl->setVariable("loan", 		$pmt_plan_link) ;
  $tpl->setVariable("pBalance", 	number_format($floan[$i][prev_balance],2)) ;
  $tpl->setVariable("principal", 	number_format($floan[$i][principal],2)) ;
  $tpl->setVariable("interest", 	number_format($floan[$i][interes],2)) ;
  $tpl->setVariable("payment", 		number_format($floan[$i][payment],2)) ;
  
  $tpl->parseCurrentBlock("results");
  } 	
		
if (is_array($lmessage)) {
  
  while ( $list = each($lmessage) ) {
    $href = sprintf("javascript:openWin('index.popup.php?scr_name=LN.SCR.pmtPlan&id=%s','PLAN','menubar=no,scrollbars=yes,resizable=no,width=800,height=600')", $lmessage[$list[key]][id]);
	$msg_pmt_plan_link 	= sprintf("<a href=%s onmouseover=\"self.status='%s'; return true;\" onmouseout=\"window.status=''\" onClick=\"window.status=''\">%s</a>", $href, $_LABELS['details'], $lmessage[$list[key]][loan_code]);
			
    $tpl->setCurrentBlock("msg");
	
	$msg_att_pars  = sprintf("<input type=hidden name=ma[%s][client_id] value=%s>",							$lmessage[$list[key]][id], $lmessage[$list[key]][client_id]);
	$msg_att_pars .= sprintf("<input type=hidden name=ma[%s][loan_id] value=%s>",							$lmessage[$list[key]][id], $lmessage[$list[key]][id]);
	$msg_att_pars .= sprintf("<input type='checkbox' name=ma[%s][attendance] value='1' class=checkbox>",	$lmessage[$list[key]][id]);

    $tpl->setVariable("msg_att_pars", 	$msg_att_pars) ;
    
    $tpl->setVariable("msg_client", 	$lmessage[$list[key]][client]) ;
    $tpl->setVariable("msg_loan", 		$msg_pmt_plan_link) ;
    $tpl->setVariable("message", 		$lmessage[$list[key]][msg]) ;

    $tpl->parseCurrentBlock("msg");
	}
	
  }
	
if ($num) {
  $tpl->setVariable("submit", $btn->imgsubmit('applyForm',$_LABELS['submit']));
  }
?>