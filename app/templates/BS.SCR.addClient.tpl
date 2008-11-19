<script language='javascript'>
function setSociety() {
  advisor_id = document.clientForm.advisor_id.value;
  zone_id = document.clientForm.zone_id.value;
  window.open("index.popup.php?scr_name=BS.SCR.pickSociety&zone_id="+zone_id+"&advisor_id="+advisor_id,"PickSociety","menubar=no,scrollbars=no,resizable=no,width=580,height=400");
  }
function unsetSociety() {
  document.clientForm.society_id.value = 0; 
  document.clientForm.society.value = "";
  }
</script>
<table cellspacing=0 cellpadding=0 border=0>
<caption><br></caption>
{clientForm_javascript}
<form {clientForm_attributes}>
{clientForm_scr_name_html}
{clientForm_id_html}
{clientForm_society_id_html}
{clientForm_activate_html}
  <tr>
    <td class=label>{clientForm_code_label}</td>
    <td>{clientForm_code_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_birthdate_label}</td>
    <td>{clientForm_birthdate_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_first_label}</td>
    <td>{clientForm_first_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_middle_label}</td>
    <td>{clientForm_middle_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_last_label}</td>
    <td>{clientForm_last_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_nick_label}</td>
    <td>{clientForm_nick_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_skills_label}</td>
    <td>{clientForm_skills_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_education_label}</td>
    <td>{clientForm_education_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_cstatus_label}</td>
    <td>{clientForm_cstatus_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_spouse_label}</td>
    <td>{clientForm_spouse_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_dependants_label}</td>
    <td>{clientForm_dependants_html}<br><br></td>
  </tr>
  <tr>
    <td class=label>{clientForm_gender_label}</td>
    <td>{clientForm_gender_html}<br><br></td>
  </tr>
  <tr>
    <td class=label>{clientForm_phone_label}</td>
    <td>{clientForm_phone_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_mobile_label}</td>
    <td>{clientForm_mobile_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_email_label}</td>
    <td>{clientForm_email_html}</td>
  </tr>
   <tr>
    <td class=label>{clientForm_address_label}</td>
    <td>{clientForm_address_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_advisor_id_label}</td>
    <td>{clientForm_advisor_id_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_zone_id_label}</td>
    <td>{clientForm_zone_id_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_society_label}</td>
    <td>{clientForm_society_html}</td>
    <td>{clientForm_pickSociety_html}</td>
  </tr>
  <tr>
    <td class=label>{clientForm_memo_label}</td>
    <td>{clientForm_memo_html}</td>
  </tr>
  <tr>    
    <td colspan=2 align=center>
	  <br>
	  {clientForm_cancel_html}{clientForm_reset_html}{clientForm_submit_html}
	  {clientForm_close_html}{clientForm_edit_html}{clientForm_view_html}	      
	</td>
  </tr>
</table>