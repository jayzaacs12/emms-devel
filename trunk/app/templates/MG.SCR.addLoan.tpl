<h1>{title}</h1>
<br><br>
<table cellspacing=0 cellpadding=0>
<caption>{client_code}</caption>
  <tr><td class=label>{code_label}</td><td>{code}</td></tr>
</table>
<br><br>
<table cellspacing=0 cellpadding=0>
<caption>{client_data}</caption>
  <tr><td class=label>{name_label}</td><td>{name}</td></tr>
  <tr><td class=label>{zone_label}</td><td>{zone}</td></tr>
  <tr><td class=label>{advisor_label}</td><td>{advisor}</td></tr>
  <tr><td class=label>{society_label}</td><td>{society}</td></tr>
</table>
<br><br>

{iLoanForm_javascript}
<form {iLoanForm_attributes}>
{iLoanForm_scr_name_html}
{iLoanForm_id_html}
{iLoanForm_client_id_html}
<table cellspacing=0 cellpadding=0 border=0>
<caption>{loan_data}</caption>
  <tr>
    <td class=label>{iLoanForm_fund_id_label}</td>
    <td>{iLoanForm_fund_id_html}</td>
  </tr>
  <tr>
    <td class=label>{iLoanForm_business_id_label}</td>
    <td>{iLoanForm_business_id_html}</td>
  </tr>
  <tr>
    <td class=label>{iLoanForm_loan_type_id_label}</td>
    <td>{iLoanForm_loan_type_id_html}</td>
  </tr>
  <tr>
    <td class=label>{iLoanForm_number_of_payments_label}</td>
    <td>{iLoanForm_number_of_payments_html}</td>
  </tr>	    
  <tr>
    <td class=label>{iLoanForm_new_kp_label}</td>
    <td>{iLoanForm_new_kp_html}</td>
  </tr>
  <tr>
    <td class=label>{iLoanForm_new_delivered_date_label}</td>
    <td>{iLoanForm_new_delivered_date_html}</td>
  </tr>
  <tr>
    <td class=label>{iLoanForm_new_first_payment_date_label}</td>
    <td>{iLoanForm_new_first_payment_date_html}</td>
  </tr>
  <tr>
    <td class=label>{iLoanForm_savings_v_label}</td>
    <td>{iLoanForm_savings_v_html}</td>
  </tr>
  <tr>    
    <td colspan=2 align=center><br><br>{iLoanForm_cancel_html}{iLoanForm_submit_html}</td>
  </tr> 	
</table>


