<table cellspacing=0 cellpadding=0 border=0>
<caption><br></caption>
{custom_javascript}
{programForm_javascript}
<form {programForm_attributes}>
{programForm_scr_name_html}
{programForm_id_html}
  <tr>
    <td class=label>{programForm_program_label}</td>
    <td>{programForm_program_html}</td>
  </tr>
  <tr>
    <td class=label>{programForm_fund_id_label}</td>
    <td style='padding:5px 0px 10px 0px'>{programForm_current_funds_html}</td>
  </tr>
  <tr>
    <td></td>
    <td>{programForm_fund_id_html}</td>
  </tr>
  <tr>
    <td class=label>{programForm_description_label}</td>
    <td>{programForm_description_html}<br><br></td>
  </tr>
  <tr>
    <td class=label>{programForm_status_label}</td>
    <td>{programForm_status_html}<br><br></td>
  </tr>
  <tr>
    <td></td>
    <td style='padding:10px 0px 10px 0px'>{programForm_confidential_html}</td>
  </tr>
  <tr>    
    <td colspan=2 align=center>
	  <br><br>
	  {programForm_cancel_html}{programForm_reset_html}{programForm_submit_html}
	  {programForm_close_html}{programForm_edit_html}{programForm_view_html}	      
	</td>
  </tr>
</table>