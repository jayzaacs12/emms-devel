<table cellspacing=0 cellpadding=0 border=0>
<caption><br></caption>
{userForm_javascript}
<form {userForm_attributes}>
{userForm_scr_name_html}
{userForm_id_html}
{userForm_oldpassword_html}
  <tr>
    <td class=label>{userForm_username_label}</td>
    <td>{userForm_username_html}</td>
    <td width=20px></td>
    <td class=label>{userForm_super_id_label}</td>
    <td>{userForm_super_id_html}</td>
  </tr>
  <tr>
    <td class=label>{userForm_password_label}</td>
    <td>{userForm_password_html}</td>
    <td></td>
    <td class=label>{userForm_zone_id_label}</td>
    <td>{userForm_zone_id_html}</td>
  </tr>
  <tr>
    <td class=label>{userForm_verify_label}</td>
    <td>{userForm_verify_html}</td>
    <td></td>
    <td class=label>{userForm_active_label}</td>
    <td>{userForm_active_html}</td>
  </tr>
  <tr>
    <td class=label>{userForm_code_label}</td>
    <td>{userForm_code_html}</td>
    <td></td>
    <td class=label>{userForm_roles_label}</td>
    <td rowspan=6 valign=top>{userForm_roles_html}</td>
  </tr>
  <tr>
    <td class=label>{userForm_email_label}</td>
    <td>{userForm_email_html}</td>
  </tr>
  <tr>
    <td class=label>{userForm_first_label}</td>
    <td>{userForm_first_html}</td>
  </tr>
  <tr>
    <td class=label>{userForm_middle_label}</td>
    <td>{userForm_middle_html}</td>
  </tr>
  <tr>
    <td class=label>{userForm_last_label}</td>
    <td>{userForm_last_html}</td>
  </tr>
  <tr>
    <td class=label>{userForm_birthdate_label}</td>
    <td>{userForm_birthdate_html}</td>
  </tr>
  <tr>
    <td class=label>{userForm_gender_label}</td>
    <td>{userForm_gender_html}<br><br></td>
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