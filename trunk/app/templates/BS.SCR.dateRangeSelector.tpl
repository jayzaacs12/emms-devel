<h1>{form_title}</h1>
<table cellspacing=0 cellpadding=0 border=0>
<caption><br></caption>
{dateForm_javascript}
<form {dateForm_attributes}>
{dateForm_scr_name_html}
{dateForm_oldrange_html}
{dateForm_href_html}
  <tr>
    <td class=label>{dateForm_oldrange_l_label}</td>
    <td>{dateForm_oldrange_l_html}</td>
  </tr>
  <tr>
    <td class=label>{dateForm_date_from_label}</td>
    <td>{dateForm_date_from_html}</td>
  </tr>
  <tr>
    <td class=label>{dateForm_date_to_label}</td>
    <td>{dateForm_date_to_html}</td>
  </tr>
  <tr>    
    <td colspan=2 align=center>
	  <br>
	  {dateForm_cancel_html}{dateForm_submit_html}    
	</td>
  </tr>
</table>
