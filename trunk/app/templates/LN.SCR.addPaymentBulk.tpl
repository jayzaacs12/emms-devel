<h1>{addPaymentBulk}</h1>{cn_date}<br><br>
<table cellspacing=0 cellpadding=0>
  <tr><td class=label>{society_label}</td>	<td>{society}</td></tr>
  <tr><td></td>								<td>{code}</td></tr>
  <tr><td class=label>{zone_label}</td>		<td>{zone}</td></tr>
</table>
<br>
{applyForm_javascript}
<form {applyForm_attributes}>
{applyForm_scr_name_html}
{applyForm_id_html}
{due_chart}
<table cellspacing=0 cellpadding=0>
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
    <td class=label></td>
    <td><br><br>{applyForm_submit_html}</td>
  </tr>
</table>
</form>

