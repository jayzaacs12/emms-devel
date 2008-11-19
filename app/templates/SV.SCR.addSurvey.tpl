<script language='javascript'>
function pickQuestions() {
  window.open("index.popup.php?scr_name=SV.SCR.pickQuestions","PickSociety","menubar=no,scrollbars=yes,resizable=no,width=750,height=550");
  }
</script>
<table cellspacing=0 cellpadding=0 border=0>
<caption><br></caption>
{surveyForm_javascript}
<form {surveyForm_attributes}>
{surveyForm_scr_name_html}
{surveyForm_id_html}
{surveyForm_question_list_html}
  <tr>
    <td class=label>{surveyForm_name_label}</td>
    <td>{surveyForm_name_html}</td>
  </tr>
  <tr>
    <td class=label>{surveyForm_description_label}</td>
    <td>{surveyForm_description_html}</td>
  </tr>
  <tr>
    <td class=label>{surveyForm_preview_label}</td>
    <td width=300px><br>{surveyForm_preview_html}</td>
    <td valign=top>{surveyForm_pickQuestions_html}</td>
  </tr>
  <tr>    
    <td colspan=2 align=center>
	  <br><br>
	  {surveyForm_cancel_html}{surveyForm_reset_html}{surveyForm_submit_html}
	  {surveyForm_close_html}{surveyForm_edit_html}{surveyForm_view_html}	      
	</td>
  </tr>
</table>