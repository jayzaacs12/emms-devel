<table class=chart>
  <tr>
    <!-- BEGIN header -->
    <td class={header_class}>{column_name}</td>
    <!-- END header -->
  </tr>
  <!-- BEGIN results -->
  <tr class='{row_class}' onmouseover="this.className='rowOn'" onmouseout="this.className='{row_class}'">
    <!-- BEGIN row -->
    <td class={cell_class}>{item}</td>
	<!-- END row -->
  </tr>
  <!-- END results -->
</table>
<table>
  <!-- BEGIN legend -->
  <tr class={style}><td width=20px>&nbsp;</td><td class=legend>{legend_text}</td></tr>
  <!-- END legend -->
</table>
<br><br>
<a href='index.php?scr_name=RP.SCR.ChartCacheToXLS'>{xls_download}</a>