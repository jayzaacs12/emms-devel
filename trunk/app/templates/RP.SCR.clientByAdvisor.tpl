<table>
  <tr><td colspan=2><h1>{rpt_label}</h1></td></tr>
  <tr><td class=label>{data_date_label}</td><td>{data_date}</td></tr>
  <tr>
    <td class=label></td>
    <td>
      <form name=theForm action='index.popup.php?scr_name=BS.SCR.dateSelector' method=post>
        <input type=hidden name=href value='index.popup.php?scr_name=RP.SCR.clientByAdvisor'>
        <input type=hidden name=olddate value='{olddate}'>
        <a href='javascript:document.theForm.submit()'>{dateSelector}</a>
      </form>
    </td>
  </tr>
</table>
<br><b>{rpt_subtitle_label}</b>
{chart}
<br><br>
