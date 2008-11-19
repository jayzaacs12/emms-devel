function calEffRate() 
{

	fees_at = new Number(document.forms[0].fees_at.value);
	fees_af = new Number(document.forms[0].fees_af.value);
	rates_r = document.forms[0].rates_r.value;
	installment = document.forms[0].installment.value;
	calendar_type = document.forms[0].calendar_type.value;
	payment_frequency = document.forms[0].payment_frequency.value;	
	n=1;
	switch(payment_frequency) {
		case "W":
			n = installment / 7 ;
			break;
		case "BW":
			n = installment / 14 ;
			break;			
		case "M":
			n = installment / 30 ;
			break;
		case "Q":
			n = installment / 90 ;
			break;	
		case "SA":
			n = installment / 180 ;
			break;
		case "A":
			n = installment / 360 ;
			break;
										
		}	
	i = (rates_r*installment)/(100*calendar_type*n);
	PMTn1 = ((1+((fees_at+fees_af)/100))*i)/(1-Math.pow((1/(1+i)),n));	
	i_e = new Number(i);
	rates_e = new Number(rates_r);
	PMTn2 = i_e/(1-Math.pow((1/(1+i_e)),n));
	while (PMTn2 < PMTn1) {
	    rates_e += 0.01;
		i_e = (rates_e*installment)/(100*calendar_type*n);
		PMTn2 = i_e/(1-Math.pow((1/(1+i_e)),n));
		}		
	document.forms[0].rates_e.value = rates_e;	
}