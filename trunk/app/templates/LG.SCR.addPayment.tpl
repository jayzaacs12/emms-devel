<h1>{title}</h1>
<br /><hr>
<h2>{applyPayment}</h2>
<br /><br />
<table>
  <tr>
    <td valign=top>
    
      <table cellspacing=0 cellpadding=0 border=0>
        <tr><td class=label>{borrower_name_label}</td><td>{borrower_name}</td></tr>
        <tr><td class=label>{borrower_code_label}</td><td>{borrower_code}</td></tr>
        <tr><td class=label>{loan_code_label}</td><td>{loan_code}<br></td></tr>
        <tr><td class=label>{status_label}</td><td>{status}<br></td></tr>
        <tr><td class=label>{li_cause_label}</td><td>{li_cause}<br></td></tr>
      </table>
      
    </td>
    
    <td width=30px>&nbsp;</td>
    
    <td valign=top>
    
      <table cellspacing=0 cellpadding=0 border=0>
      {lpaymentForm_javascript}
      <form {lpaymentForm_attributes}>
      {lpaymentForm_scr_name_html}
      {lpaymentForm_id_html}
      {lpaymentForm_max_amount_html}
      <tr><td class=label>{lpaymentForm_amount_label}<br /> <br /></td></tr>
      <tr><td>{max_payment_label}{max_payment}</td></tr>
      <tr><td class=label>{lpaymentForm_amount_html}<br /></td></tr>
      <tr><td align=right>{lpaymentForm_submit_html}&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>
      </table>
      
    </td>
  </tr>
</table>

