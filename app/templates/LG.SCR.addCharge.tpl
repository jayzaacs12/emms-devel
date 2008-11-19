<h1>{title}</h1>
<br /><hr>
<h2>{applyLCharge}</h2>
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
      {lchargeForm_javascript}
      <form {lchargeForm_attributes}>
      {lchargeForm_scr_name_html}
      {lchargeForm_id_html}
      <tr>
        <td class=label>
          {lchargeForm_amount_label}<br />
          {lchargeForm_amount_html}<br /><br />
          {lchargeForm_description_label}<br />
          {lchargeForm_description_html}<br />
        </td></tr>
      <tr><td align=right>{lchargeForm_submit_html}&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>
      </table>

    </td>
  </tr>
</table>