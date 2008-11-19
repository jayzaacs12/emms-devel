<table>
  <tr><td colspan=2><h1>{rpt_label}</h1></td></tr>
  <tr><td class=label>{date_range_label}</td><td>{date_range}</td></tr>
  <tr>
    <td class=label></td>
    <td>
      <form name=theForm action='index.popup.php?scr_name=BS.SCR.dateRangeSelector' method=post>
        <input type=hidden name=href value='index.popup.php?scr_name=RP.SCR.transactionsCredits'>
        <input type=hidden name=oldrange value='{oldrange}'>
        <a href='javascript:document.theForm.submit()'>{dateRangeSelector}</a>
      </form>
    </td>
  </tr>
</table>
<br>
{chart}<br>
<br><br>