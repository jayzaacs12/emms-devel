<br>
<table cellpadding=0 cellspacing=0 border=0>
  <tr>
    <td class=infolabel>{client_label}</td>
    <td>{client}</td>
  </tr>
  <tr>
    <td></td>
    <td>{client_code}</td>
  </tr>
  <tr>
    <td colspan=2>{client_lookup}</td>
  </tr>
</table>

<h2>{name}</h2>
<table cellpadding=0 cellspacing=0 border=0 width=450px>
<caption><hr></caption>
<tr><td colspan=4>{description}</td></tr>
<form {qf_attributes}>

<!-- BEGIN qf_hidden_block -->
  <!-- BEGIN qf_hidden_loop -->
  {qf_hidden}
  <!-- END qf_hidden_loop -->
<!-- END qf_hidden_block -->

<!-- BEGIN qf_surveyItem -->
  <tr><td></td><td class=label colspan=3>{qf_group_label}</td></tr>
  <!-- BEGIN qf_surveyItem_error -->  
  <tr><td></td><td colspan=3 class=required>{qf_error}</td></tr>
  <!-- END qf_surveyItem_error -->
  <!-- BEGIN qf_surveyItem_loop -->   
	<!-- BEGIN qf_surveyItem_radio -->
      <tr><td></td><td colspan=3>{qf_element}</td></tr>
	<!-- END qf_surveyItem_radio -->
  <!-- END qf_surveyItem_loop -->
      <tr><td><br></td></tr>
<!-- END qf_surveyItem -->
</table>

<!-- BEGIN qf_buttonBar -->
<table cellpadding=0 cellspacing=0>
  <tr>
  <!-- BEGIN qf_buttonBar_loop -->    	
	<!-- BEGIN qf_buttonBar_element -->
      <td>{qf_element}</td>
	<!-- END qf_buttonBar_element -->
  <!-- END qf_buttonBar_loop -->
  </tr>
</table>
<!-- END qf_buttonBar -->
<br><br><br>