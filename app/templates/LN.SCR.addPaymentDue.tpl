<h1>{form_name}</h1>
<br>
{applyForm_javascript}
<form {applyForm_attributes}>
{applyForm_scr_name_html}
{applyForm_ref_html}
{applyForm_id_html}
{applyForm_total_all_html}
{applyForm_total_oldest_html}
{applyForm_client_html}
<table cellpadding=0 cellspacing=0 border=0>
  <tr>
    <td class=label>{loan_code_label}</td>
    <td>{loan_code}</td>
  </tr>
  <tr>
    <td class=label>{client_label}</td>
    <td>{borrower_name}</td>
  </tr>
  <tr>
    <td></td>
    <td>{borrower_code}<br><br></td>
  </tr>
  <tr>
    <td class=label>{due_label}</td>
    <td>{due_chart}</td>
  </tr>
  <tr>
    <td class=label>{applyForm_mode_label}</td>
    <td>{applyForm_mode_html}<br><br></td>
  </tr>
  <tr>
    <td></td>
    <td><br></td>
  </tr>
  <tr>
    <td class=label></td>
    <td>{applyForm_pmt_receipt_flag_a_html}</td>
  </tr>
  <tr>
    <td class=label></td>
    <td>{applyForm_pmt_receipt_flag_b_html}</td>
  </tr>
  <tr>
    <td class=label>{applyForm_notes_label}</td>
    <td>{applyForm_notes_html}</td>
  </tr>
  <tr>
    <td></td>
    <td><br><br></td>
  </tr>
  <tr>
    <td class=label>{applyForm_total_pmt_label}</td>
    <td>{applyForm_total_pmt_html}</td>
  </tr>
  <tr>
    <td class=label></td>
    <td><br><br>{applyForm_submit_html}</td>
  </tr>
</table>
</form>
<br><br>