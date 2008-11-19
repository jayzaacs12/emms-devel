<script language='javascript'>
function setParent() {
  advisor_id = document.societyForm.advisor_id.value;
  zone_id = document.societyForm.zone_id.value;
  window.open("index.popup.php?scr_name=BS.SCR.pickBG&zone_id="+zone_id+"&advisor_id="+advisor_id,"PickBG","menubar=no,scrollbars=no,resizable=no,width=400,height=320");
  }
function unsetParent() {
  document.societyForm.parent_id.value = 0; 
  document.societyForm.parent.value = "";
  }
</script>
<table cellspacing=0 cellpadding=0 border=0>
<caption><br></caption>
{societyForm_javascript}
<form {societyForm_attributes}>
{societyForm_scr_name_html}
{societyForm_parent_id_html}
{societyForm_cat_html}
{societyForm_id_html}

  <tr>
    <td class=label>{societyForm_category_label}</td>
    <td>{societyForm_category_html}</td>
  </tr>
  <tr>
    <td class=label>{societyForm_advisor_id_label}</td>
    <td>{societyForm_advisor_id_html}</td>
  </tr>
  <tr>
    <td class=label>{societyForm_zone_id_label}</td>
    <td>{societyForm_zone_id_html}</td>
  </tr>
  <tr>
    <td class=label>{societyForm_parent_label}</td>
    <td>{societyForm_parent_html}</td>
    <td>{societyForm_pickParent_html}</td>
  </tr>
  <tr>
    <td class=label>{societyForm_name_label}</td>
    <td>{societyForm_name_html}</td>
  </tr>
  <tr>
    <td class=label>{societyForm_memo_label}</td>
    <td>{societyForm_memo_html}</td>
  </tr>
  <tr>
    <td class=label>{societyForm_president_id_label}</td>
    <td>{societyForm_president_id_html}</td>          
  </tr>
  <tr>
    <td class=label>{societyForm_treasurer_id_label}</td>
    <td>{societyForm_treasurer_id_html}</td>          
  </tr>
  <tr>
    <td class=label>{societyForm_secretary_id_label}</td>
    <td>{societyForm_secretary_id_html}</td>          
  </tr>
  <tr>
    <td class=label>{societyForm_groups_auto_label}</td>
    <td>{societyForm_groups_auto_html}</td>          
  </tr>
   <tr>
    <td class=label>{societyForm_groups_num_label}</td>
    <td>{societyForm_groups_num_html}<br><br></td>          
  </tr>
  <tr>    
    <td colspan=2 align=center>
      <br>
	  {societyForm_cancel_html}{societyForm_reset_html}{societyForm_submit_html}
	  {societyForm_close_html}{societyForm_edit_html}{societyForm_view_html}	      
    </td>
  </tr>
</table>