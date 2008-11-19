{message}
<h1>{content_title}</h1>
<table cellspacing=0 cellpadding=0 border=0>
<caption><br></caption>
{searchForm_javascript}
<form {searchForm_attributes}>
{searchForm_scr_name_html}
  <tr>
    <td class=label>{searchForm_date_label}</td>
    <td>{searchForm_date_html}</td>
  </tr>
  <tr>
    <td class=label>{searchForm_zone_id_label}</td>
    <td>{searchForm_zone_id_html}</td>
  </tr>
  <tr>
    <td class=label>{searchForm_advisor_id_label}</td>
    <td>{searchForm_advisor_id_html}</td>
  </tr>
  <tr>
    <td class=label>{searchForm_borrower_type_label}</td>
    <td>{searchForm_borrower_type_html}</td>
  </tr>
  <tr>    
    <td colspan=2 align=center>
	  <br>
	  {searchForm_cancel_html}{searchForm_reset_html}{searchForm_submit_html}
	</td>
  </tr>
</table>
{chart}
