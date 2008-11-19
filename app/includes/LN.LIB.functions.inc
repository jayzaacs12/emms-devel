<?
function unpackloanstatus($package,$sep1,$sep2,$sep3) {
	global $_LABELS;
	$pack_arr = explode($sep1,$package);
	if (is_array($pack_arr)) { 
		for ($i=0;$i<count($pack_arr);$i++) {
			$pack_arr[$i] = explode($sep2,$pack_arr[$i]);
			$status[$pack_arr[$i][0]][status] = $pack_arr[$i][0];
			$label = sprintf("tblLoans.status.%s",$pack_arr[$i][0]);
			$status[$pack_arr[$i][0]][label] = $_LABELS[$label];
			$status[$pack_arr[$i][0]][button] = $pack_arr[$i][1];
			$status[$pack_arr[$i][0]][options] = explode($sep3, $pack_arr[$i][2]);
			$status[$pack_arr[$i][0]][mask] = $pack_arr[$i][3];
			}
		}
	return $status;
	}

function CalNumOfPMT($installment, $payment_frequency) {
	switch($payment_frequency) {
		case "W":
			$n = $installment / 7 ;
			break;
		case "BW":
			$n = $installment / 14 ;
			break;			
		case "M":
			$n = $installment / 30 ;
			break;
		case "Q":
			$n = $installment / 90 ;
			break;	
		case "SA":
			$n = $installment / 180 ;
			break;
		case "A":
			$n = $installment / 360 ;
			break;								
		}
	return $n;
}
function CalEffRate($rates_r,$fees_at,$fees_af,$installment,$calendar_type,$payment_frequency) 
{
	$n = CalNumOfPMT($installment, $payment_frequency);

	$i = ($rates_r*$installment)/(100*$calendar_type*$n);
	$PMTn1 = ((1+(($fees_at+$fees_af)/100))*$i)/(1-pow((1/(1+$i)),$n));	
	$i_e = $i;
	$rates_e = $rates_r;
	$PMTn2 = $i_e/(1-pow((1/(1+$i_e)),$n));
	while ($PMTn2 < $PMTn1) {
	    $rates_e += 0.01;
		$i_e = ($rates_e*$installment)/(100*$calendar_type*$n);
		$PMTn2 = $i_e/(1-pow((1/(1+$i_e)),$n));
		}		
	$rates_e = round($rates_e,2);
	return $rates_e;

//old code... with an error
	$PMTn1 = ((1+(($fees_at+$fees_af)/100))*$rates_r)/(1-pow((1/(1+((0.01*$rates_r*$installment)/$calendar_type))),$n));
	$rates_e = $rates_r;
	$PMTn2 = $rates_e/(1-pow((1/(1+((0.01*$rates_e*$installment)/$calendar_type))),$n));
	while ($PMTn2 < $PMTn1) {
		$rates_e += 0.01;
		$PMTn2 = $rates_e/(1-pow((1/(1+((0.01*$rates_e*$installment)/$calendar_type))),$n));
		}
	$rates_e = round($rates_e,2);
	return $rates_e;
}
function CalMinPMT($kp,$kat,$kaf,$rates_r,$installment,$calendar_type,$payment_frequency) 
{
	$n = CalNumOfPMT($installment, $payment_frequency); 
	$i = (0.01*$rates_r*$installment)/($calendar_type*$n);
	return ((($kp+$kat+$kaf)*$i)/(1-pow((1/(1+$i)),$n)));
}
function CalPMTPlan($lprofile) 
{
	$pmt = $lprofile->pmt;	
	$pk = $lprofile->kp + $lprofile->kat + $lprofile->kaf;
	
	$F_kp = round(($lprofile->kp / $pk),2);
	$F_kat = round(($lprofile->kat / $pk),2);
	$F_kaf = round(($lprofile->kaf / $pk),2);
	
	
	$PDate[mon] = $lprofile->m_creator_date;	
	$PDate[mday] = $lprofile->d_creator_date;
	$PDate[year] = $lprofile->y_creator_date; 
	
	$PDateUTC = mktime(0,0,0,$PDate[mon],$PDate[mday],$PDate[year]); 
	$PDate = getdate($PDateUTC);

	$payment_frequency = $lprofile->payment_frequency;
	$calendar_type = $lprofile->calendar_type;
	$rates_r = $lprofile->rates_r;

	$data[0][] = '-';
	$data[0][] = '<b>'.date("d/m/y", $PDateUTC).'</b>';
	$data[0][] = '<b>'.$pk.'</b>';
	$data[0][] = '-';
	$data[0][] = '-';
	$data[0][] = '-';
	
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
		
		$DATE[$i] = getdate($dateUTC[$i]);
		
		$DATEF[$i] = date("d/m/y", $dateUTC[$i]);
		
		$pk = $PK[$i];
		$PDateUTC = $dateUTC[$i];
		
		$data[$i+1][] = $i+1;
		$data[$i+1][] = $DATEF[$i];
		$data[$i+1][] = $PK[$i];
		$data[$i+1][] = $RA[$i];
		$data[$i+1][] = $KA[$i];		
		$data[$i+1][] = $PMT[$i];
			
		$i++;
	}

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
	
	return $data;
}
?>