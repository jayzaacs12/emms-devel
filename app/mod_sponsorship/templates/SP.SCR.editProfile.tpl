<table cellspacing=0 cellpadding=0 border=0>
<caption><br></caption>
{userForm_javascript}
<form {userForm_attributes}>
{userForm_scr_name_html}
{userForm_id_html}
  <tr>
    <td class=label>{userForm_username_label}</td>
    <td>{userForm_username_html}</td>
  </tr>
  <tr>
    <td class=label>{userForm_sponsor_label}</td>
    <td>{userForm_sponsor_html}</td>
  </tr>
  <tr>
    <td class=label>{contact_info}</td>
  </tr>
  <tr>
    <td class=sublabel>{userForm_first_label}</td>
    <td>{userForm_first_html}</td>
  </tr>
  <tr>
    <td class=sublabel>{userForm_middle_label}</td>
    <td>{userForm_middle_html}</td>
  </tr>
  <tr>
    <td class=sublabel>{userForm_last_label}</td>
    <td>{userForm_last_html}</td>
  </tr>
  <tr>
    <td class=sublabel>{userForm_email_label}</td>
    <td>{userForm_email_html}</td>
  </tr>
  <tr>
    <td class=label>{userForm_memo_label}</td>
    <td colspan=4>{userForm_memo_html}</td>
  </tr>
  <tr>
    <td colspan=5 align=center>
	  <br>
	  {userForm_cancel_html}{userForm_reset_html}{userForm_submit_html}
	  {userForm_close_html}{userForm_edit_html}{userForm_view_html}
	</td>
  </tr>
</table>