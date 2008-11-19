<h1>{content_title}</h1>
<table cellspacing=0 cellpadding=0 border=0>
<caption><br></caption>
<script>
function updateModeFields() {
switch (document.plotterForm.mode.value) {
  case 'program_id': 
    document.plotterForm.zone_id.value 			= '-';
    document.plotterForm.zone_id.disabled 		= true;
    document.plotterForm.advisor_id.value 		= '-';
    document.plotterForm.advisor_id.disabled 	= true;
    document.plotterForm.program_id.disabled 	= false; 
    break;
  case 'zone_id': 
    document.plotterForm.program_id.value 		= '-';
    document.plotterForm.program_id.disabled 	= true;
    document.plotterForm.advisor_id.value 		= '-';
    document.plotterForm.advisor_id.disabled 	= true;
    document.plotterForm.zone_id.disabled 	= false; 
    break;
  case 'advisor_id': 
    document.plotterForm.program_id.value 		= '-';
    document.plotterForm.program_id.disabled 	= true;
    document.plotterForm.zone_id.value 			= '-';
    document.plotterForm.zone_id.disabled 		= true;
    document.plotterForm.advisor_id.disabled 	= false; 
    break;
  default: 
    document.plotterForm.program_id.value 		= '-';
    document.plotterForm.program_id.disabled 	= true;
    document.plotterForm.zone_id.value 			= '-';
    document.plotterForm.zone_id.disabled 		= true;
    document.plotterForm.advisor_id.value 		= '-';
    document.plotterForm.advisor_id.disabled 	= true;
    break;
  }
}
</script>
{custom_javascript}
{plotterForm_javascript}
<form {plotterForm_attributes}>
{plotterForm_scr_name_html}
  <tr>
    <td class=label>{plotterForm_fact_label}</td>
    <td>{plotterForm_fact_html}</td>
  </tr>
  <tr>
    <td class=label>{plotterForm_dateFrom_label}</td>
    <td>{plotterForm_dateFrom_html}</td>
  </tr>
  <tr>
    <td class=label>{plotterForm_dateTo_label}</td>
    <td>{plotterForm_dateTo_html}</td>
  </tr>
  <tr>
    <td class=label>{plotterForm_cycle_label}</td>
    <td>{plotterForm_cycle_html}</td>
  </tr>
  <tr>
    <td class=label>{plotterForm_mode_label}</td>
    <td>{plotterForm_mode_html}</td>
  </tr>
  <tr>
    <td class=label>{plotterForm_zone_id_label}</td>
    <td>{plotterForm_zone_id_html}</td>
  </tr>
  <tr>
    <td class=label>{plotterForm_program_id_label}</td>
    <td>{plotterForm_program_id_html}</td>
  </tr>
  <tr>
    <td class=label>{plotterForm_advisor_id_label}</td>
    <td>{plotterForm_advisor_id_html}</td>
  </tr>
</table>
<table cellspacing=1 cellpadding=0>
  <tr><td>{graph}</td></tr>
  <tr><td><b>{graph_title}</b></td></tr>
</table>
<table cellspacing=0 cellpadding=0 border=0>
  <tr>    
    <td align=center>
	  <br><br>
	  {plotterForm_cancel_html}{plotterForm_reset_html}{plotterForm_submit_html}
	  {plotterForm_close_html}{plotterForm_edit_html}{plotterForm_view_html}	      
	</td>
  </tr>
</table>
<a href="javascript:print()">{print}</a>
