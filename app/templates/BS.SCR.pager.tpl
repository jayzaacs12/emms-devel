{pager_header}
<br>
<table cellpadding=0 cellspacing=0>
  <!-- BEGIN filter --> 
  <tr>
    <td class=infolabel>{criteria_label}</td>
	<td width=20px></td>
	<td>{criteria_value}</td>
  </tr>
  <!-- END filter -->
  <tr>
    <td class=infolabel>{total_label}</td>
	<td></td>
	<td>{total}</td>
  </tr>
</table>
<br><br>
<table cellpadding=0 cellspacing=0>
  <tr>
    <td>
      <!-- BEGIN query_results --> 
        <table class=chart>
          <tr>
            <!-- BEGIN header --> 
	          <td class=header>{column_name}</td>
            <!-- END header --> 
          </tr>
          <!-- BEGIN results --> 
          <tr>
            <!-- BEGIN row --> 
              <td class=chart align={align}>{item}</td>
	        <!-- END row --> 
          </tr>
	      <!-- END results --> 
        </table>				     
      <!-- END query_results --> 
    </td>
    <td>
  </tr>
  <tr>
    <td align=right>
	  <br>
      <table>
        <tr>
          <td>{first}</td>
		  <td>{back}</td>
		  <td width=5px></td>
		  <td>{pages}</td>
		  <td width=5px></td>
		  <td>{next}</td>
		  <td>{last}</td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<a href='index.php?scr_name=RP.SCR.QueryCacheToXLS'>{xls_download}</a>
<br><br><br><br>
<table>
  <tr><td align=center>{refresh_button}{new_search}</td></tr>
</table>
{refresh_form}
{pager_foot}
<br><br>
