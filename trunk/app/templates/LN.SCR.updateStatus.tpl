<table cellspacing=0 cellpadding=0 border=0>
<caption><hr></caption>
{updateStatusForm_javascript}
<form {updateStatusForm_attributes}>
{updateStatusForm_scr_name_html}
{updateStatusForm_ref_html}
{updateStatusForm_id_html}
{updateStatusForm_status_html}
{updateStatusForm_p_status_html}
  <tr>
    <td class=label>{status_label}</td>
    <td><strike>{p_status}</strike>&nbsp;&nbsp;>>&nbsp;&nbsp;<b>{status}</b></td>
  </tr>
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
    <td>{borrower_code}</td>
  </tr>
  <tr>
    <td><br><br></td>
  </tr>
  <tr>
    <td class=label>{updateStatusForm_fund_label}</td>
    <td>{updateStatusForm_fund_html}</td>
  </tr>
  <tr>
    <td class=label>{updateStatusForm_memo_label}</td>
    <td>{updateStatusForm_memo_html}</td>
  </tr>
  <tr>
    <td class=label>{li_details_label}</td>
  </tr>
  <tr>
    <td colspan=2>{li_details}</td>
  </tr>
  <tr>
    <td colspan=2 align=center>
	  <br>
	  {updateStatusForm_cancel_html}{updateStatusForm_reset_html}{updateStatusForm_submit_html}
	  {updateStatusForm_close_html}{updateStatusForm_edit_html}{updateStatusForm_view_html}
	</td>
  </tr>
</table>