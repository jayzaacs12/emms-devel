<h1>{form_title}</h1>
<table cellspacing=0 cellpadding=0 border=0>
<caption><br></caption>
{dateForm_javascript}
<form {dateForm_attributes}>
{dateForm_scr_name_html}
{dateForm_olddate_html}
{dateForm_href_html}
  <tr>
    <td class=label>{dateForm_olddate_l_label}</td>
    <td>{dateForm_olddate_l_html}</td>
  </tr>
  <tr>
    <td class=label>{dateForm_date_label}</td>
    <td>{dateForm_date_html}</td>
  </tr>
  <tr>    
    <td colspan=2 align=center>
	  <br>
	  {dateForm_cancel_html}{dateForm_submit_html}    
	</td>
  </tr>
</table>