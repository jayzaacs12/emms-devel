<?php
// require this line on the caller script...  include '../ST.functions.inc';
require_once 'class/ltprofile.php';
require_once 'class/bzprofile.php';
require_once 'class/uprofile.php';
require_once 'class/cprofile.php';
require_once 'class/sprofile.php';
require_once 'class/xls.php';

class LPROFILE
{
    /* LPROFILE parameters */
    var $id = '';
    var $kp = '';
	var $kat = '';
	var $kaf = '';
    var $fees_at = '';
	var $fees_af = '';
	var $calendar_type = '';
	var $payment_frequency = '';
	var $rates_r = '';
	var $rates_d = '';
	var $rates_e = '';
	var $installment = '';
	var $margin_d = '';
	var $margin_k = '';
	var $margin_r = '';
	var $margin_c = '';
	var $client_id = '';
	var $client_img = '';
	var $business_id = '';	
	var $loan_type_id = '';
	var $loan_code = '';    
	var $creator_id = '';
	var $creator = '';
	var $creator_date = '';	
	var $editor_id = '';
	var $editor = '';
	var $editor_date = ''; 
	var $status = ''; 	
	var $header = ''; 	 
	var $zone_id = '';
	var $client_zone_id = '';
	var $program_id = '';

    function LPROFILE($data)
    {	
    global $_LABELS;
    $this->id = $data[id];
    $this->business_id = $data[business_id];    
    $this->kp = $data[kp]; 
    $this->kat = $data[kat];
    $this->kaf = $data[kaf]; 
    $this->pg_value = $data[pg_value]; 
    $this->pg_memo = $data[pg_memo];
    $this->re_value = $data[re_value]; 
    $this->re_memo = $data[re_memo];
    $this->fgd_value = $data[fgd_value]; 
    $this->fgd_memo = $data[fgd_memo];
    $this->fgt_value = $data[fgt_value]; 
    $this->fgt_memo = $data[fgt_memo];			   
    $this->fees_at = $data[fees_at];
	$this->fees_af = $data[fees_af]; 
	$this->calendar_type = $data[calendar_type]; 
	$this->payment_frequency = $data[payment_frequency]; 
	$this->rates_r = $data[rates_r];
	$this->rates_d = $data[rates_d]; 
	$this->rates_e = $data[rates_e]; 
	$this->installment = $data[installment]; 
	$this->margin_d = $data[margin_d]; 
	$this->margin_k = $data[margin_k]; 
	$this->margin_r = $data[margin_r]; 
	$this->margin_c = $data[margin_c]; 
	$this->client_id = $data[client_id]; 
	$this->loan_type_id = $data[loan_type_id]; 
	$this->loan_code = $data[loan_code]; 
	$this->creator_id = $data[creator_id]; 
	$this->creator = UPROFILE::getName($data[creator_id]);
	$this->creator_date = $data[creator_date];
	if ($data[editor_date]) { 
	  $this->editor_id = $data[editor_id]; 
	  $this->editor = UPROFILE::getName($data[editor_id]);	
	  $this->editor_date = $data[editor_date]; 
	  } else {
	  $this->editor_id = $this->creator_id; 
	  $this->editor = $this->creator;	
	  $this->editor_date = $this->creator_date; 
	  }
	
	$this->zone_id = $data[zone_id];
	$this->client_zone_id = $data[client_zone_id];
	$this->program_id = $data[program_id];	
	
	$this->revised_date = $data[revised_date]; 
	$this->revised = UPROFILE::getName($data[revised_id]);	
	$this->approval_date = $data[approval_date]; 
	$this->approval = UPROFILE::getName($data[approval_id]);	
	$this->disburse_date = $data[disburse_date]; 
	$this->disburse = UPROFILE::getName($data[disburse_id]);
		
	$this->delivered_date = $data[delivered_date]; 
	$this->delivered_date_utc = $data[delivered_date_utc]; 
	$this->delivered = UPROFILE::getName($data[delivered_id]);	
	$this->y_delivered_date = $data[y_delivered_date]; 	
	$this->m_delivered_date = $data[m_delivered_date]; 	
	$this->d_delivered_date = $data[d_delivered_date]; 
		
	$this->cancel_date = $data[cancel_date]; 
	$this->cancel = UPROFILE::getName($data[cancel_id]);	
	$this->rejected_date = $data[rejected_date]; 
	$this->rejected = UPROFILE::getName($data[rejected_id]);	
	$this->legal_in_date = $data[legal_in_date]; 
	$this->legal_in_date_utc = $data[legal_in_date_utc]; 
	$this->legal_in = UPROFILE::getName($data[legal_in_id]);	
	$this->legal_out_date = $data[legal_out_date]; 
	$this->legal_out = UPROFILE::getName($data[legal_out_id]);	
	$this->retracted_date = $data[retracted_date]; 
	$this->retracted = UPROFILE::getName($data[retracted_id]);	
	$this->r_open_date = $data[r_open_date]; 	
	$this->r_open = UPROFILE::getName($data[r_open_id]);	
	
	$this->pmt = $data[pmt];
	$this->y_creator_date = $data[y_creator_date]; 	
	$this->m_creator_date = $data[m_creator_date]; 	
	$this->d_creator_date = $data[d_creator_date]; 	
	$this->status = $data[status];
	$tmp = 'tblLoans.status.'.$data[status]; 	
	$this->status_label = $_LABELS[$tmp];

	switch ($data[borrower_type]) {
		CASE 'B':
			$cprofile = CPROFILE::createProfile($data[client_id]);
			$this->client = $cprofile->name;
			$this->client_img = $cprofile->photo;
			$this->header = $cprofile->header;
			$this->vscript = 'BS.SCR.browseClients'; //$cprofile->vscript;
			break;
		CASE 'I':
			$cprofile = CPROFILE::createProfile($data[client_id]);
			$this->client = $cprofile->name;
			$this->client_img = $cprofile->photo;
			$this->header = $cprofile->header;
			$this->vscript = 'BS.SCR.browseClients'; //$cprofile->vscript;
			break;
		CASE 'G':;
			$sprofile = SPROFILE::createProfile($data[client_id]);
			$this->client = $sprofile->name;
			$this->client_img = false;
			$this->header = $sprofile->header;
			$this->vscript = 'BS.SCR.browseSocieties'; //$sprofile->vscript;
			break;
		}
	
	}
	
	function createProfile($LoanID)
    {
    global $_CONF; 
	$sql = "SELECT
	  			l.*,
	  			DATE_FORMAT(l.creator_date,'%Y') AS y_creator_date,
	  			DATE_FORMAT(l.creator_date,'%m') AS m_creator_date,
	  			DATE_FORMAT(l.creator_date,'%e') AS d_creator_date,
	  			DATE_FORMAT(l.creator_date,'$_CONF[date_format_mysql]') AS creator_date,
	  			DATE_FORMAT(l.editor_date,'$_CONF[date_format_mysql]') AS editor_date,
	  			DATE_FORMAT(l.revised_date,'$_CONF[date_format_mysql]') AS revised_date,
	  			DATE_FORMAT(l.approval_date,'$_CONF[date_format_mysql]') AS approval_date,
	  			DATE_FORMAT(l.disburse_date,'$_CONF[date_format_mysql]') AS disburse_date,
	  			DATE_FORMAT(l.delivered_date,'%Y') AS y_delivered_date,
	  			DATE_FORMAT(l.delivered_date,'%m') AS m_delivered_date,
	  			DATE_FORMAT(l.delivered_date,'%e') AS d_delivered_date,
	  			DATE_FORMAT(l.delivered_date,'$_CONF[date_format_mysql]') AS delivered_date,
	  			UNIX_TIMESTAMP(l.delivered_date) AS delivered_date_utc,
	  			DATE_FORMAT(l.cancel_date,'$_CONF[date_format_mysql]') AS cancel_date,
	  			DATE_FORMAT(l.rejected_date,'$_CONF[date_format_mysql]') AS rejected_date,
	  			DATE_FORMAT(l.legal_in_date,'$_CONF[date_format_mysql]') AS legal_in_date,
	  			UNIX_TIMESTAMP(l.legal_in_date) AS legal_in_date_utc,
	  			DATE_FORMAT(l.legal_out_date,'$_CONF[date_format_mysql]') AS legal_out_date,
	  			DATE_FORMAT(l.retracted_date,'$_CONF[date_format_mysql]') AS retracted_date,
	  			DATE_FORMAT(l.r_open_date,'$_CONF[date_format_mysql]') AS r_open_date,
				t.borrower_type,
				t.calendar_type,
				t.payment_frequency,
				t.margin_k,
				t.margin_r,
				t.margin_c
			FROM 
				tblLoans AS l, tblLoanTypes AS t 
			WHERE 
				l.id = '$LoanID'
			AND t.id = l.loan_type_id";
	$data = submitSelect_OneAssoc($sql);
	$lprofile = new LPROFILE($data);
	return $lprofile;
    }	
	
		   
    function display($LoanID)
    {
	$lprofile = LPROFILE::createProfile($LoanID); 
	LPROFILE::printProfile($lprofile);
	return $lprofile->id;
    }


  function printProfile($lprofile)
  {	
  global $_LABELS;
  global $_CONF;

  $ltprofile = LTPROFILE::createProfile($lprofile->loan_type_id);	
  $bzprofile = BZPROFILE::createProfile($lprofile->business_id);	

  if ($lprofile->revised_date) { $revised_date_field = sprintf("<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td></tr>",$_LABELS['tblLoans.revised_id'],$lprofile->revised,$lprofile->revised_date); }	  		
  if ($lprofile->approval_date) { $approval_date_field = sprintf("<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td></tr>",$_LABELS['tblLoans.approval_id'],$lprofile->approval,$lprofile->approval_date); }
  if ($lprofile->disburse_date) { $disburse_date_field = sprintf("<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td></tr>",$_LABELS['tblLoans.disburse_id'],$lprofile->disburse,$lprofile->disburse_date); }
  if ($lprofile->delivered_date) { $delivered_date_field = sprintf("<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td></tr>",$_LABELS['tblLoans.delivered_id'],$lprofile->delivered,$lprofile->delivered_date); }
  if ($lprofile->cancel_date) { $cancel_date_field = sprintf("<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td></tr>",$_LABELS['tblLoans.cancel_id'],$lprofile->cancel,$lprofile->cancel_date); }
  if ($lprofile->rejected_date) { $rejected_date_field = sprintf("<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td></tr>",$_LABELS['tblLoans.rejected_id'],$lprofile->rejected,$lprofile->rejected_date); }
  if ($lprofile->r_open_date) { $r_open_date_field = sprintf("<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td></tr>",$_LABELS['tblLoans.r_open_id'],$lprofile->r_open,$lprofile->r_open_date); }
  if ($lprofile->legal_in_date) { $legal_in_date_field = sprintf("<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td></tr>",$_LABELS['tblLoans.legal_in_id'],$lprofile->legal_in,$lprofile->legal_in_date); }
  if ($lprofile->legal_out_date) { $legal_out_date_field = sprintf("<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td></tr>",$_LABELS['tblLoans.legal_out_id'],$lprofile->legal_out,$lprofile->legal_out_date); }

  if ( $lprofile->pg_value > 0 ) { $pg_value_field = sprintf("<tr><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td><td align=right class=recordviewvalue>%s</td></tr>",$_LABELS['tblCollateral.pawned_goods'],$lprofile->pg_memo,number_format($lprofile->pg_value,2,'.',',')); }
  if ( $lprofile->re_value > 0 ) { $re_value_field = sprintf("<tr><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td><td align=right class=recordviewvalue>%s</td></tr>",$_LABELS['tblCollateral.real_estate'],$lprofile->re_memo,number_format($lprofile->re_value,2,'.',',')); }
  if ( $lprofile->fgd_value > 0 ) { $fgd_value_field = sprintf("<tr><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td><td align=right class=recordviewvalue>%s</td></tr>",$_LABELS['tblCollateral.fiduciary_goods'],$lprofile->fgd_memo,number_format($lprofile->fgd_value,2,'.',',')); }
  if ( $lprofile->fgt_value > 0 ) { $fgt_value_field = sprintf("<tr><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td><td align=right class=recordviewvalue>%s</td></tr>",$_LABELS['tblCollateral.fiduciary_garantees'],$lprofile->fgt_memo,number_format($lprofile->fgt_value,2,'.',',')); }

  printf ("<h1 class=query>%s</h1>", $_LABELS['MSG.TTL.029']);
  ?>
  <table class=recordview>
  <caption class=recordview><?= $lprofile->client; ?></caption>
    <tr><td colspan=2><hr class=recordview></td></tr>
	<tr>
      <td>
        <?
 		printf("
		  <table>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td colspan=3><hr class=dash></td></tr>
			<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
			<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel><br></td>
	  		<tr><td class=recordviewlabel>%s ( %% )</td><td class=recordviewvalue>%s</td><td class=recordviewvalue align=right>%s</td></tr>
	  		<tr><td class=recordviewlabel></td><td class=recordviewvalue>%s</td><td class=recordviewvalue align=right>%s</td></tr>
	  		<tr><td class=recordviewlabel><br></td>
	  		<tr><td class=recordviewlabel>%s ( %% )</td><td class=recordviewvalue>%s</td><td class=recordviewvalue align=right>%s</td></tr>
	  		<tr><td class=recordviewlabel></td><td class=recordviewvalue>%s</td><td class=recordviewvalue align=right>%s</td></tr>
	  		<tr><td class=recordviewlabel><br></td>
	  		<tr><td class=recordviewlabel>%s ( %% )</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td colspan=3><hr class=dash></td></tr>
	  		<tr><td class=recordviewlabel colspan=3 style='text-align:center'>%s</td></tr>
			%s
			%s
			%s
			%s
			<tr><td class=recordviewvalue></td><td class=recordviewvalue></td><td align=right class=recordviewvalue style='background-color:#eeeeee'>%s</td></tr>
	  		<tr><td colspan=3><hr class=dash></td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td><td class=recordviewvalue>%s</td></tr>
			%s
			%s
			%s
			%s
			%s
			%s
			%s
			%s
			%s
		  </table>",
			$_LABELS['tblLoans.id'],$lprofile->loan_code,
			$_LABELS['tblLoans.status'],$lprofile->status_label,
			$_LABELS['tblClients.id'],coollink(sprintf("index.php?scr_name=%s",$lprofile->vscript),'id',$lprofile->client_id,$_LABELS['details'],'recordview',$lprofile->client),
			$_LABELS['tblLoans.kp'],number_format($lprofile->kp,2,'.',','),
			$_LABELS['MSG.OTH.035'],$ltprofile->currency,
			$_LABELS['tblBusiness.name'],coollink('index.php?scr_name=BS.SCR.browseBusiness','id',$bzprofile->id,$_LABELS['details'],'recordview',$bzprofile->name),
			$_LABELS['tblLoanTypes.description'],coollink('index.php?scr_name=LN.SCR.browseLoanTypes','id',$ltprofile->id,$_LABELS['details'],'recordview',$ltprofile->description),
			$_LABELS['tblLoanTypes.payment_frequency'],$ltprofile->payment_frequency_label,
			$_LABELS['tblLoanTypes.installment'],$lprofile->installment,
			$_LABELS['tblLoanTypes.calendar_type'],$lprofile->calendar_type,
			$_LABELS['tblLoanTypes.margin_k'],$lprofile->margin_k,
			$_LABELS['tblLoanTypes.margin_r'],$lprofile->margin_r,
			$_LABELS['tblLoanTypes.margin_d'],$lprofile->margin_d,
			$_LABELS['MSG.OTH.025'],$_LABELS['tblLoanTypes.fees_at'],$lprofile->fees_at,
			$_LABELS['tblLoanTypes.fees_af'],$lprofile->fees_af,
			$_LABELS['MSG.OTH.023'],$_LABELS['tblLoanTypes.rates_r'],$lprofile->rates_r,
			$_LABELS['tblLoanTypes.rates_d'],$lprofile->rates_d,
			$_LABELS['MSG.OTH.024'],$lprofile->rates_e,
			$_LABELS['tblCollateral.id'],
			$pg_value_field,
			$re_value_field, 
			$fgd_value_field,
			$fgt_value_field,
			number_format(($lprofile->pg_value + $lprofile->re_value + $lprofile->fgd_value + $lprofile->fgt_value),2,'.',','),
			$_LABELS['tblClients.creator_id'],$lprofile->creator,$lprofile->creator_date, 
			$_LABELS['tblClients.editor_id'],$lprofile->editor,$lprofile->editor_date,
			$revised_date_field,
			$approval_date_field,
			$disburse_date_field,
			$delivered_date_field,
			$cancel_date_field,
			$rejected_date_field,
			$r_open_date_field,
			$legal_in_date_field,
			$legal_out_date_field
		  );
		
		?>
      </td>
	  <td class=recordviewvalue>
	    <?
		if ($lprofile->client_img) {
		  printf ("<img class=recordview src='%s'>", $lprofile->client_img );
		  }
		?>	    
	  </td>
	</tr>
  </table>


<? 
	echo '<br><br><br><br>';
		$loan_status = unpackloanstatus($_CONF[loan_status],';','|',','); 
		for ($i=0;$i<count($loan_status[$lprofile->status][options]);$i++) {
			if ($loan_status[$loan_status[$lprofile->status][options][$i]][button]) {
				$link = sprintf("index.php?scr_name=LN.SCR.updateStatus&status=%s&p_status=%s&id=%s", $loan_status[$lprofile->status][options][$i], $lprofile->status, $lprofile->id);
				printf("%s", textbutton($link, $loan_status[$loan_status[$lprofile->status][options][$i]][label]) );
				}
			}

	echo '<br><br><br><br>';

		$buttondata = LPROFILE::genButtonData($lprofile);
		$bar = New SCRBAR($buttondata);
		SCRBAR::printbar($bar);	

	echo '<br><br><br><br>';


		
    }
    
    function displayCompact($LoanID)
    {
	$sql = "SELECT 	lt.* FROM tblLoanTypes AS lt WHERE lt.id = '$LoanTypeID'";
	$data = submitSelect_OneAssoc($sql);
	$ltprofile = new LTPROFILE($data);
	LTPROFILE::printCompactProfile($ltprofile);
	return $ltprofile->id;
    }

    function printCompactProfile($ltprofile)
    {	
    global $_LABELS;
    print ("<CENTER><TABLE cellspacing=1 cellpadding=2 width=100% class=rpt>\n");
	printf ("<TR><TD class=llst>%s</TD><TD class=llst>%s</TD><TD class=llst>%s</TD><TD class=llst>%s</TD></TR>", $_LABELS['tblLoanTypes.id'], $_LABELS['tblLoanTypes.installment'], $_LABELS['tblLoanTypes.payment_frequency'], $_LABELS['MSG.OTH.024']);
	printf ("<TR><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD></TR>", $ltprofile->description, $ltprofile->installment, $ltprofile->payment_frequency_label, $ltprofile->rates_e);
	printf ("</TABLE></CENTER>");
	}
	
	function getPMTPlan($lprofile,$startDateUTC,$truncDateUTC,$pk) 
	{
	$pmt = $lprofile->pmt;	
	if (!($pk)) { $pk = $lprofile->kp + $lprofile->kat + $lprofile->kaf; }

	if ($pk != 0) {
		$F_kp = round(($lprofile->kp / $pk),2);
		$F_kat = round(($lprofile->kat / $pk),2);
		$F_kaf = round(($lprofile->kaf / $pk),2);
		}

	if ( $truncDateUTC ) {	
//		$EDateUTC = mktime(0,0,0,$endDate[month],$endDate[day],$endDate[year]);	
		$TDateUTC = $truncDateUTC;
		} else {
		$TDateUTC = 0;
		}
		
	if ( $startDateUTC ) {
//		$PDateUTC = mktime(0,0,0,$startDate[month],$startDate[day],$startDate[year]);	
		$PDateUTC = $startDateUTC;
		} else {
		if ($lprofile->delivered_date) {
			$PDate[mon] = $lprofile->m_delivered_date;	
			$PDate[mday] = $lprofile->d_delivered_date;
			$PDate[year] = $lprofile->y_delivered_date; 	
			} else {
			$PDate[mon] = $lprofile->m_creator_date;	
			$PDate[mday] = $lprofile->d_creator_date;
			$PDate[year] = $lprofile->y_creator_date;
			} 	
		$PDateUTC = mktime(0,0,0,$PDate[mon],$PDate[mday],$PDate[year]); 
		}
		
	$PDate = getdate($PDateUTC);	

	$payment_frequency = $lprofile->payment_frequency;
	$calendar_type = $lprofile->calendar_type;
	$rates_r = $lprofile->rates_r;
/*
	$data[0][] = '-';
	$data[0][] = '<b>'.date("d/m/y", $PDateUTC).'</b>';
	$data[0][] = '<b>'.$pk.'</b>';
	$data[0][] = '-';
	$data[0][] = '-';
	$data[0][] = '-';
*/	
	$i = 0;

	while($pk > 0) {
		switch($payment_frequency) {
		case "W":
			$dateUTC[$i] = $PDateUTC + 7 * 24 * 60 * 60 ;
			$n = 7;
			$N[$i] = $n;
			break;
		case "BW":
			$dateUTC[$i] = $PDateUTC + 14 * 24 * 60 * 60 ;
			$n = 14;
			$N[$i] = $n;
			break;			
		case "M":
			if ($PDate[mon] < 12) { 
				++$PDate[mon];
				} else {
				++$PDate[year];
				$PDate[mon] = 1;
				}
			$dateUTC[$i] = mktime(0,0,0,$PDate[mon],$PDate[mday],$PDate[year]);
			$n = ($dateUTC[$i] - $PDateUTC)/(24 * 60 * 60);
			$N[$i] = $n;
			break;
		case "Q":
			if ($PDate[mon] < 10) { 
				$PDate[mon] += 3;			
				} else {
				++$PDate[year];
				$PDate[mon] -= 9;
				}
			$dateUTC[$i] = mktime(0,0,0,$PDate[mon],$PDate[mday],$PDate[year]);
			$n = ($dateUTC[$i] - $PDateUTC)/(24 * 60 * 60);
			$N[$i] = $n;
			break;	
		case "SA":
			if ($PDate[mon] < 7) { 
				$PDate[mon] += 6;			
				} else {
				++$PDate[year];
				$PDate[mon] -= 6;
				}
			$dateUTC[$i] = mktime(0,0,0,$PDate[mon],$PDate[mday],$PDate[year]);
			$n = ($dateUTC[$i] - $PDateUTC)/(24 * 60 * 60);
			$N[$i] = $n;
			break;	
		case "A":
			++$PDate[year];
			$dateUTC[$i] = mktime(0,0,0,$PDate[mon],$PDate[mday],$PDate[year]);
			$n = ($dateUTC[$i] - $PDateUTC)/(24 * 60 * 60);
			$N[$i] = $n;
			break;								
			} 

// End loop if $endDateUTC was passed
		if ( ($TDateUTC > 0) && ($TDateUTC < $dateUTC[$i]) ) { break; }
				
		$RA[$i] = round(($pk*(0.01*$rates_r/$calendar_type)*$n),2);
		
		if ( $pmt - $RA[$i] <= $pk ) {
			$KA[$i] = $pmt - $RA[$i];			
			$PK[$i] = $pk - $KA[$i];			
			$PMT[$i] = $pmt;
			} else {
			$KA[$i] = $pk;			
			$PK[$i] = 0;
			$PMT[$i] = $RA[$i] + $KA[$i];			
			}
		
//		$DATE[$i] = getdate($dateUTC[$i]);
		
		$DATEF[$i] = date("d/m/y", $dateUTC[$i]);
		
		$pk = $PK[$i];
		$PDateUTC = $dateUTC[$i];
		
		$data[$i][] = $i+1;		//quote
		$data[$i][] = $PDateUTC; //$DATEF[$i];	//date
		$data[$i][] = $PK[$i];		//pk
		$data[$i][] = $RA[$i];		//ra
		$data[$i][] = $KA[$i];		//ka
		$data[$i][] = $PMT[$i];		//pmt
			
		$i++;
		}
/*
	$data[$i+1][] = '';
	$data[$i+1][] = '';
	$data[$i+1][] = '';
	$data[$i+1][] = '<b>'.array_sum($RA).'</b>';	
	$data[$i+1][] = '<b>'.array_sum($KA).'</b>';	
	$data[$i+1][] = '<b>'.array_sum($PMT).'</b>';
	
	if($i < 30) {
		for ($j = $i+2; $j < 30; $j++) {
			$data[$j][] = '&nbsp;';
			}
		}
*/	

	return $data; 
	}
	
	
	function getSpecialPMTHistory($LoanID) 
	{
	$sql = "SELECT id FROM tblPayments WHERE loan_id = '$LoanID' AND special = '1' ORDER BY id";
	$data = submitSelect_MultAssoc($sql);
	return $data;
	}

	function getNotSpecialPMTHistory($LoanID) 
	{
	$sql = "SELECT id FROM tblPayments WHERE loan_id = '$LoanID' AND special = '0' ORDER BY id";
	$data = submitSelect_MultAssoc($sql);
	return $data;
	}

	function getPMTHistory($LoanID) 
	{
	$sql = "SELECT
	  			u.username AS username,
				p.payment AS pmt,
	  			p.principal AS ka,
	  			p.interes AS ra,
	  			p.pending_principal AS pk,
	  			UNIX_TIMESTAMP(p.date) AS date
			FROM 
				tblPayments AS p, tblUsers AS u
			WHERE 
				p.loan_id = '$LoanID'
				AND u.id = user_id
			ORDER BY pk DESC";
	$data = submitSelect_MultAssoc($sql);
	return $data;
	}
	
	function getPendingPaymentToDate($loan,$truncDateUTC = 0) 
	{
	global $_LABELS;
	$id = $loan->id;
	
	if ($truncDateUTC == 0) { $truncDateUTC = mktime(0,0,0); }
	$pmt_plan = LPROFILE::getPMTPlan($loan,$startDateUTC,$truncDateUTC,0);

	if (!($pmt_real = LPROFILE::getPMTHistory($id) )) { 
		$lastPMT[pk] = $loan->kp + $loan->kat + $loan->kaf; 
		$lastPMT[date] = $loan->delivered_date_utc;
		} else {
		$lastPMT = end($pmt_real);
		}

	$nsp_pmt = LPROFILE::getNotSpecialPMTHistory($id);
	
	//$pq es el # de cuotas atrasadas.
	switch ( $pq = MAX( 0, count($pmt_plan) - count($nsp_pmt) ))	{

		case 0:
		//$n cantidad de dias desde el ultimo pago
		$n = ($truncDateUTC - $lastPMT[date])/(24 * 60 * 60); 
		$special = 1; //no se ha cumplido la fecha de pagar una cuota regular
		//$ra interes acumulado desde el ultimo pago
		$ra = round(($lastPMT[pk]*(0.01*$loan->rates_r/$loan->calendar_type)*$n),2);		
					
		$data[0][date] = date($_LABELS['MSG.OTH.030'], $truncDateUTC);
		$data[0][prev_balance] = $lastPMT[pk];
		$data[0][principal] = 0;
		$data[0][new_balance] = $lastPMT[pk];
		$data[0][interes] = $ra;
		$data[0][delay] = 0;
		$data[0][delay_n] = 0;		
		$data[0][payment] = $ra;
		$data[0][special] = 1;
		$data[0][payment_max] = $lastPMT[pk] + $ra;
		$data[0][payment_total] = $ra;
	
		//max_special_pmt calcula el maximo pago posible en un pago especial
		//de manera que no se vaya a pagar mas de todo el capital pendiente
		//mas sus intereses
		break;

		case 1:	
		$n = ($truncDateUTC - $lastPMT[date])/(24 * 60 * 60); 
		$lastPlan = end($pmt_plan); 
		
		$delay_n = ($truncDateUTC - $lastPlan[1])/(24 * 60 * 60);
		if ( $delay_n > $loan->margin_d ) {
			$delay = round( (0.01 * $loan->rates_d * $delay_n * $loan->pmt),2 );		
			} else {
			$delay_n = 0; 
			$delay = 0;			
			}		

		$ra = round(($lastPMT[pk]*(0.01*$loan->rates_r/$loan->calendar_type)*$n),2);		
		
		$data[0][date] = date($_LABELS['MSG.OTH.030'], $lastPlan[1]);
		$data[0][prev_balance] = $lastPMT[pk];
		$data[0][principal] = MIN(($loan->pmt - $ra), $lastPMT[pk]);
		$data[0][new_balance] = $data[0][prev_balance] - $data[0][principal];
		$data[0][interes] = $ra;
		$data[0][delay] = $delay;
		$data[0][delay_n] = $delay_n;		
		$data[0][payment] = $data[0][principal] + $ra + $delay; //$loan->pmt + $delay;
		$data[0][special] = 0;					
		$data[0][payment_total] = $data[0][payment];
		$data[0][payment_max] = $data[0][payment] + $data[0][new_balance];
		break;

		default:

		for ($i=0;$i<$pq;$i++) {
			
			$inx = count($pmt_plan) - $pq + $i;

			$n = ($pmt_plan[$inx][1] - $lastPMT[date])/(24*60*60);
			$delay_n = ($truncDateUTC - $pmt_plan[$inx][1])/(24 * 60 * 60);
			if ( $delay_n > $loan->margin_d ) {
				$delay = round( (0.01 * $loan->rates_d * $delay_n * $loan->pmt),2 );		
				} else {
				$delay_n = 0; 
				$delay = 0;			
				}			

			$ra = round(($lastPMT[pk]*(0.01*$loan->rates_r/$loan->calendar_type)*$n),2);		
						
			$data[$i][date] = date($_LABELS['MSG.OTH.030'], $pmt_plan[$inx][1]);
			$data[$i][prev_balance] = $lastPMT[pk]; 
			$data[$i][principal] = MIN(($loan->pmt - $ra), $lastPMT[pk]);
			$data[$i][new_balance] = $data[$i][prev_balance] - $data[$i][principal];
			$data[$i][interes] = $ra;
			$data[$i][delay] = $delay;
			$data[$i][delay_n] = $delay_n;		
			$data[$i][payment] = $data[$i][principal] + $ra + $delay;
			$data[$i][special] = 0;
			$data[0][payment_total] += $data[$i][payment];
			$data[0][payment_max] = $data[0][payment_total] + $data[$i][new_balance];
//			$payment_tmp += $loan->pmt + $delay;						

			$lastPMT[date] = $pmt_plan[$inx][1];
			$lastPMT[pk] = $data[$i][new_balance];			
			}	
		}
	return $data;
	}

	function getDelinquencyDetails ($lprofile,$legal_in_date_utc = 0)
	{ //if 'legal-in' (loan writeoff) then save html details of all pending payments in $memo 
	global $_LABELS;
	if ($legal_in_date_utc == 0) { $legal_in_date_utc = $lprofile->$legal_in_date_utc; }
	$data = LPROFILE::getPendingPaymentToDate($lprofile);
	$details = '<table align=center cellspacing=1 cellpadding=2 width=100% bgcolor=silver>';
	$details .= sprintf("<tr style='text-align:center'><td class=lrpt>%s</td><td class=lrpt>%s</td><td class=lrpt>%s</td><td class=lrpt>%s</td><td class=lrpt colspan=2>%s</td><td class=lrpt>%s</td></tr>",$_LABELS['tblPayments.date'],$_LABELS['MSG.OTH.031'],$_LABELS['tblPayments.principal'],$_LABELS['tblPayments.interes'],$_LABELS['tblPayments.delay'],$_LABELS['tblPayments.payment']); 
	$i=0;
	do {
		$details .= sprintf("<tr style='text-align:right'><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td></tr>",$data[$i][date],number_format($data[$i][prev_balance],2,'.',','),number_format($data[$i][principal],2,'.',','),number_format($data[$i][interes],2,'.',','),number_format($data[$i][delay],2,'.',','),$data[$i][delay_n],number_format($data[$i][payment],2,'.',',')); 		
		$totals[principal] +=  $data[$i][principal];
		$totals[interes] +=  $data[$i][interes];
		$totals[delay] +=  $data[$i][delay];
		$totals[payment] +=  $data[$i][payment];
		} while (++$i < count($data)); 
	$i--;
	if ($data[$i][new_balance] > 0) {
		$details .= sprintf("<tr style='text-align:right;color:green'><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td></tr>",$data[$i][date],number_format($data[$i][new_balance],2,'.',','),number_format($data[$i][new_balance],2,'.',','),'0.00','0.00','0',number_format($data[$i][new_balance],2,'.',',')); 		
		$totals[principal] +=  $data[$i][new_balance];
		$totals[payment] +=  $data[$i][new_balance];
		}
	$details .= sprintf("<tr style='text-align:right;color:red;font-weight:bold'><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td><td class=lst>%s</td></tr>",'-','-',number_format($totals[principal],2,'.',','),number_format($totals[interes],2,'.',','),number_format($totals[delay],2,'.',','),'-',number_format($totals[payment],2,'.',',')); 		
	$details .= '</table>';
	$DelinquencyDetails[write_off_amount] = round($totals[principal]/(1 + 0.01*($lprofile->fees_af + $lprofile->fees_at)),2);
	$DelinquencyDetails[html_details] = $details;
	return $DelinquencyDetails;
	}

    function genButtonData($lprofile) 
	{
	global $_LABELS;

	$c=0;
	$buttondata[$c][id]="LN.SCR.addLoan";
	$buttondata[$c][href]="index.php?scr_name=LN.SCR.addLoan&id=".$lprofile->id;
	$buttondata[$c][alt]= $_LABELS['edit'];
	$buttondata[$c][onClick]="";
	$buttondata[$c][ico]="edit";
	
	$c++;
	$buttondata[$c][id]="LN.SCR.addPayment";
	$buttondata[$c][href]="javascript:openWin('index.popup.php?scr_name=LN.SCR.addPayment&id=$lprofile->id','PMT','menubar=no,scrollbars=yes,resizable=no,width=800,height=400')";
	$buttondata[$c][alt]= $_LABELS['addpmt'];
	$buttondata[$c][onClick]="";
	$buttondata[$c][ico]="money";

	$c++;
	$buttondata[$c][id]="LN.SCR.pmtPlan";
	$buttondata[$c][href]="javascript:openWin('index.popup.php?scr_name=LN.SCR.pmtPlan&id=$lprofile->id','PLAN','menubar=no,scrollbars=yes,resizable=no,width=800,height=600')";
	$buttondata[$c][alt]= $_LABELS['sheet'];
	$buttondata[$c][onClick]="";
	$buttondata[$c][ico]="sheet";

	$c++;
	$buttondata[$c][id]="LN.RPT.loanStatusHistory";
	$buttondata[$c][href]="javascript:openWin('index.popup.php?scr_name=LN.RPT.loanStatusHistory&id=$lprofile->id','LSH','menubar=no,scrollbars=yes,resizable=no,width=640,height=480')";
	$buttondata[$c][alt]= $_LABELS['MSG.OTH.040'];
	$buttondata[$c][onClick]="";
	$buttondata[$c][ico]="flow";

	return $buttondata;
	}	

}
?>