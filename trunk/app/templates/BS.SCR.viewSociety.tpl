<h2>{society}</h2>
<table cellspacing=0 cellpadding=0>
<caption><hr></caption>
  <tr>
    <td valign=top>
      <table cellspacing=0 cellpadding=0>
        <tr><td class=label>{code_label}</td><td>{code}</td></tr>
        <tr><td class=label>{category_label}</td><td>{category}</td></tr>
        <tr><td class=label>{bde_label}</td><td>{bde}</td></tr>
        <tr><td class=label>{advisor_label}</td><td>{advisor}</td></tr>
        <tr><td class=label>{zone_label}</td><td>{zone}</td></tr>
        <tr><td class=label>{memo_label}</td><td width=300px>{memo}</td></tr>
        <tr><td colspan=2><br>{creator}</td></tr>
        <tr><td colspan=2>{editor}</td></tr>
      </table>
    </td>
    <td width=20px></td>
    <td width=250px valign=top>
      <table>
	    <tr><td class=label>{memberlist_label}</td></tr>
	    <tr><td>{legend}</td></tr>
	  </table>
        <table>
  	  <!-- BEGIN memberlist --> 
          <tr>
	          <td colspan=2><b>{group_name}<b></td><td>{move_button}</td><td>&nbsp;</td><td>{deactivate_button}</td>
          </tr>
          <!-- BEGIN memberdetails --> 
          <tr>
              <td><b>{president}{treasurer}{secretary}</b></td><td>{member_name}</td>
          </tr>
	      <!-- END memberdetails --> 
      <!-- END memberlist --> 	  
        </table>				     
	</td>
  </tr>
  <tr><td colspan=2>{toolbar}</td></tr>
</table>
{refreshForm}