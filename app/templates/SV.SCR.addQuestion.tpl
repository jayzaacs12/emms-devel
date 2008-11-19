<table cellspacing=0 cellpadding=0 border=0>
<caption><br></caption>
{sitemForm_javascript}
<form {sitemForm_attributes}>
{sitemForm_scr_name_html}
{sitemForm_id_html}
{sitemForm_process_html}
  <tr>
    <td class=label>{sitemForm_category_label}</td>
    <td>{sitemForm_category_html}</td>
  </tr>
  <tr>
    <td class=label>{sitemForm_question_label}</td>
    <td>{sitemForm_question_html}</td>
  </tr>
  <tr><td><br><br></td></tr>
  <tr>
    <td class=label>{sitemForm_NOA_label}</td>
    <td>{sitemForm_NOA_html}</td>
  </tr>
  <tr><td><br></td></tr>
  <tr>
    <td></td>
    <td class=label>{sitemForm_answer_txt_0_label}</td>
	<td class=label>{sitemForm_answer_num_0_label}</td>
  </tr>
  <tr>
    <td></td>
    <td valign=bottom>{sitemForm_answer_txt_0_html}</td>
	<td valign=bottom>{sitemForm_answer_num_0_html}</td>
  </tr>
  <tr>
    <td></td>
    <td valign=bottom>{sitemForm_answer_txt_1_html}</td>
	<td valign=bottom>{sitemForm_answer_num_1_html}</td>
  </tr>
  <tr>
    <td></td>
    <td valign=bottom>{sitemForm_answer_txt_2_html}</td>
	<td valign=bottom>{sitemForm_answer_num_2_html}</td>
  </tr>
  <tr>
    <td></td>
    <td valign=bottom>{sitemForm_answer_txt_3_html}</td>
	<td valign=bottom>{sitemForm_answer_num_3_html}</td>
  </tr>
  <tr>
    <td></td>
    <td valign=bottom>{sitemForm_answer_txt_4_html}</td>
	<td valign=bottom>{sitemForm_answer_num_4_html}</td>
  </tr>
  <tr>
    <td></td>
    <td valign=bottom>{sitemForm_answer_txt_5_html}</td>
	<td valign=bottom>{sitemForm_answer_num_5_html}</td>
  </tr>
  <tr>
    <td></td>
    <td valign=bottom>{sitemForm_answer_txt_6_html}</td>
	<td valign=bottom>{sitemForm_answer_num_6_html}</td>
  </tr>
  <tr>
    <td></td>
    <td valign=bottom>{sitemForm_answer_txt_7_html}</td>
	<td valign=bottom>{sitemForm_answer_num_7_html}</td>
  </tr>
  <tr>
    <td></td>
    <td valign=bottom>{sitemForm_answer_txt_8_html}</td>
	<td valign=bottom>{sitemForm_answer_num_8_html}</td>
  </tr>
  <tr>
    <td></td>
    <td valign=bottom>{sitemForm_answer_txt_9_html}</td>
	<td valign=bottom>{sitemForm_answer_num_9_html}</td>
  </tr>
  <tr>    
    <td colspan=2 align=center>
	  <br>
	  {sitemForm_cancel_html}{sitemForm_reset_html}{sitemForm_submit_html}
	  {sitemForm_close_html}{sitemForm_edit_html}{sitemForm_view_html}	      
	</td>
  </tr>
</table>
<SCRIPT>
function refresh() {
  document.sitemForm.process.value = 0;
  document.sitemForm.submit();
  }			
</SCRIPT>    