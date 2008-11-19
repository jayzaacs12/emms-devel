<script language="JavaScript" type="text/javascript" src="javascript/calEffRate.js"></script>
<script language=javascript>
	function calSubTotal() 
	{
	pg_value = new Number(document.forms[0].pg_value.value);
	re_value = new Number(document.forms[0].re_value.value);
	fgd_value = new Number(document.forms[0].fgd_value.value);
	fgt_value = new Number(document.forms[0].fgt_value.value);
	document.forms[0].collateral_value.value = pg_value + re_value + fgd_value + fgt_value;
	}
</script>
<table cellspacing=0 cellpadding=0 border=0>
<caption><hr></caption>
{iLoanForm_javascript}
<form {iLoanForm_attributes}>
{iLoanForm_scr_name_html}
{iLoanForm_ref_html}
{iLoanForm_id_html}
{iLoanForm_client_id_html}
{iLoanForm_business_id_html}
{iLoanForm_loan_type_id_html}
{iLoanForm_margin_c_html}
{iLoanForm_lt_savings_p_html}
<tr>
  <td valign=top>
    <table cellspacing=0 cellpadding=0 border=0>
      <tr>
        <td class=label>{client_label}</td>
        <td>{client}</td>
      </tr>
      <tr>
        <td></td>
        <td>{client_code}<br><br></td>
      </tr>
      <tr>
        <td class=label>{business_label}</td>
        <td>{business}</td>
      </tr>
      <tr>
        <td class=label>{loanType_label}</td>
        <td>{loanType}<br><br></td>
      </tr>
      <tr>
        <td class=label>{iLoanForm_kp_label} [ {currency} ]</td>
        <td>{iLoanForm_kp_html}</td>
      </tr>
     <tr>
        <td><br></td>	
      </tr> 		    
      <tr>
        <td class=label>{iLoanForm_savings_p_label}</td>
        <td>{iLoanForm_savings_p_html}</td>
      </tr>
      <tr>
        <td class=label>{iLoanForm_savings_v_label}</td>
        <td>{iLoanForm_savings_v_html}</td>
      </tr>
      <tr>
        <td class=label><br><br>{collateralType_label} [ {currency} ]</td>	
        <td></td>	
      </tr> 		    
      <tr>
        <td>{iLoanForm_collateral_min_value_label}</td>
        <td>{iLoanForm_collateral_min_value_html}</td>
      </tr>
      <tr>
        <td>{iLoanForm_pg_value_label}</td>	
        <td>{iLoanForm_pg_value_html}</td>							
      </tr> 		    
      <tr>
        <td>{iLoanForm_re_value_label}</td>	
        <td>{iLoanForm_re_value_html}</td>							
      </tr> 		    
      <tr>
        <td>{iLoanForm_fgd_value_label}</td>	
        <td>{iLoanForm_fgd_value_html}</td>							
      </tr> 		    
      <tr>
        <td>{iLoanForm_fgt_value_label}</td>	
        <td>{iLoanForm_fgt_value_html}</td>							
      </tr> 		    
      <tr>
        <td>{iLoanForm_collateral_value_label}</td>	
        <td>{iLoanForm_collateral_value_html}</td>							
      </tr> 
	  <tr><td><br><br></td></tr>		    
      <tr>
        <td class=label>{iLoanForm_installment_label}</td>
        <td>{iLoanForm_installment_html}</td>
      </tr>	    
      <tr>
        <td class=label>{iLoanForm_payment_frequency_label}</td>
        <td>{iLoanForm_payment_frequency_html}</td>
      </tr>
      <tr>
        <td class=label>{iLoanForm_calendar_type_label}</td>
        <td>{iLoanForm_calendar_type_html}</td>
      </tr>
      <tr>
        <td class=label>{iLoanForm_margin_d_label}</td>
        <td>{iLoanForm_margin_d_html}</td>
      </tr>
      <tr>
        <td class=label><br><br>{rates_label} [ % ]</td>	
        <td></td>	
      </tr> 		    
      <tr>
        <td>{iLoanForm_fees_at_label}</td>
        <td>{iLoanForm_fees_at_html}</td>
      </tr>
      <tr>
        <td>{iLoanForm_fees_af_label}</td>
        <td>{iLoanForm_fees_af_html}</td>
      </tr>
      <tr>
        <td>{iLoanForm_rates_r_label}</td>
        <td>{iLoanForm_rates_r_html}</td>
      </tr>
      <tr>
        <td>{iLoanForm_rates_d_label}</td>
        <td>{iLoanForm_rates_d_html}</td>
      </tr>
	  <tr><td><br><br></td></tr>		    
      <tr>
        <td class=label>{iLoanForm_rates_e_label}</td>
        <td>{iLoanForm_rates_e_html}</td>
      </tr>	
      <tr>    
        <td colspan=2 align=center>
	      <br><br>
	      {iLoanForm_cancel_html}{iLoanForm_reset_html}{iLoanForm_submit_html}
	      {iLoanForm_close_html}{iLoanForm_edit_html}{iLoanForm_view_html}	      
	    </td>
      </tr>
    </table>
  </td>		
  <td valign=top><img class=record alt='' src='{img}'></td>
</tr> 	
</table>
<br><br>