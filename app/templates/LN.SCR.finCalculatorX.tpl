<h1>{title}</h1>
<script language="JavaScript" type="text/javascript" src="javascript/calEffRate.js"></script>
{iLoanForm_javascript}
<form {iLoanForm_attributes}>
{iLoanForm_scr_name_html}
{iLoanForm_ref_html}
{iLoanForm_id_html}
<table>
<tr>
<td valign="top">
<table cellspacing=0 cellpadding=0 border=0>
  <tr><td colspan=2 class=label>{loanType_label}</td></tr>
  <tr><td colspan=2><iframe frameborder="0" marginwidth="0" marginheight="0" width=350px; height=80 src="index.popup.php?scr_name=LN.SCR.finCalculatorXPLP"></iframe></td></tr>
  <tr><td class=label>{iLoanForm_rdate_label}</td><td>{iLoanForm_rdate_html}</td></tr>
  <tr><td class=label>{iLoanForm_kp_label}</td><td>{iLoanForm_kp_html}</td></tr>
  <tr><td class=label>{iLoanForm_installment_label}</td><td>{iLoanForm_installment_html}</td></tr>
  <tr><td class=label>{iLoanForm_payment_frequency_label}</td><td>{iLoanForm_payment_frequency_html}</td></tr>
  <tr><td class=label>{iLoanForm_calendar_type_label}</td><td>{iLoanForm_calendar_type_html}</td></tr>
  <tr><td class=label>{rates_label} [ % ]</td><td></td></tr>
  <tr><td>{iLoanForm_fees_at_label}</td><td>{iLoanForm_fees_at_html}</td></tr>
  <tr><td>{iLoanForm_fees_af_label}</td><td>{iLoanForm_fees_af_html}</td></tr>
  <tr><td>{iLoanForm_rates_r_label}</td><td>{iLoanForm_rates_r_html}</td></tr>
  <tr><td>{iLoanForm_rates_d_label}</td><td>{iLoanForm_rates_d_html}</td></tr>
  <tr><td class=label>{iLoanForm_rates_e_label}</td><td>{iLoanForm_rates_e_html}</td></tr>
  <tr><td colspan=2 align=center><br><br>{iLoanForm_submit_html}</td></tr>
</table>
</td>
<td valign="top">
  <iframe name="finCalculatorXPlan" id="finCalculatorXPlan" src="" frameborder="0" marginwidth="0" marginheight="0" width="600" height="2600"></iframe>
</td>
</tr>
</table>
</form>
<script>
  function showPlan(url) {
    var objFrame=document.getElementById("finCalculatorXPlan");
    objFrame.src=url;
    self.resizeTo(1000,700);
  }
  {showPlan}
</script>