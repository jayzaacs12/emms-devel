<table class=chart>
  <tr>
    <!-- BEGIN header --> 
    <td class=header>{column_name}</td>
    <!-- END header --> 
  </tr>
  <!-- BEGIN results --> 
  <tr class='rowOff' onmouseover="this.className='rowOn'" onmouseout="this.className='rowOff'">
    <!-- BEGIN row --> 
    <td class=activeChart align={align}>{item}</td>
	<!-- END row --> 
  </tr>
  <!-- END results --> 
</table>
<br><br>	
<a class="hide" href='index.php?scr_name=RP.SCR.ChartCacheToXLS'>{xls_download}</a>			  
<br><br>	   
