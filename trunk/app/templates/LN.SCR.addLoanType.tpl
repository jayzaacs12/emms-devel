<table cellspacing=0 cellpadding=0 border=0>
<caption><br></caption>
{ltypeForm_javascript}
<form {ltypeForm_attributes}>
{ltypeForm_scr_name_html}
{ltypeForm_id_html}
{ltypeForm_editor_id_html}
{ltypeForm_editor_date_html}
  <tr>
    <td class=label>{ltypeForm_description_label}</td>
    <td>{ltypeForm_description_html}</td>
  </tr>
  <tr>
    <td class=label>{ltypeForm_borrower_type_label}</td>
    <td>{ltypeForm_borrower_type_html}</td>
  </tr>
  <tr>
    <td class=label>{ltypeForm_currency_id_label}</td>
    <td>{ltypeForm_currency_id_html}</td>
  </tr>
  <tr>
    <td class=label>{ltypeForm_calendar_type_label}</td>
    <td>{ltypeForm_calendar_type_html}</td>
  </tr>
  <tr>
    <td class=label>{ltypeForm_payment_frequency_label}</td>
    <td>{ltypeForm_payment_frequency_html}</td>
  </tr>
  <tr>
    <td class=label>{ltypeForm_margin_c_label}</td>
    <td>{ltypeForm_margin_c_html}</td>
    <td>{ltypeForm_margin_c_lock_html}</td>
  </tr>
  <tr>
    <td class=label>{ltypeForm_installment_label}</td>
    <td>{ltypeForm_installment_html}</td>
    <td>{ltypeForm_installment_lock_html}</td>
  </tr>
  <tr>
    <td class=label>{ltypeForm_fees_at_label}</td>
    <td>{ltypeForm_fees_at_html}</td>
    <td>{ltypeForm_fees_at_lock_html}</td>
  </tr>
  <tr>
    <td class=label>{ltypeForm_fees_af_label}</td>
    <td>{ltypeForm_fees_af_html}</td>
    <td>{ltypeForm_fees_af_lock_html}</td>
  </tr>
  <tr>
    <td class=label>{ltypeForm_rates_r_label}</td>
    <td>{ltypeForm_rates_r_html}</td>
    <td>{ltypeForm_rates_r_lock_html}</td>
  </tr>
  <tr>
    <td class=label>{ltypeForm_rates_d_label}</td>
    <td>{ltypeForm_rates_d_html}</td>
    <td>{ltypeForm_rates_d_lock_html}</td>
  </tr>
  <tr>
    <td class=label>{ltypeForm_margin_d_label}</td>
    <td>{ltypeForm_margin_d_html}</td>
    <td>{ltypeForm_margin_d_lock_html}</td>
  </tr>
  <tr>
    <td class=label>{ltypeForm_savings_p_label}</td>
    <td>{ltypeForm_savings_p_html}</td>
    <td>{ltypeForm_savings_p_lock_html}</td>
  </tr>
  <tr><td colspan=2><br><br></td></tr>
  <tr>
    <td class=label>{ltypeForm_rates_e_label}</td>
    <td>{ltypeForm_rates_e_html}</td>
  </tr>
  <tr>    
    <td colspan=2 align=center>
	  <br>
	  {ltypeForm_cancel_html}{ltypeForm_reset_html}{ltypeForm_submit_html}
	  {ltypeForm_close_html}{ltypeForm_edit_html}{ltypeForm_view_html}	      
	</td>
  </tr>
</table>
<script>
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
</script>