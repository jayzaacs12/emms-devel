<br>
<table cellpadding=0 cellspacing=0>
  <tr><td class=label>{borrower_label}		</td><td>{borrower}</td></tr>
  <tr><td class=label>{borrower_type_label}	</td><td>{borrower_type}</td></tr>
  <tr><td class=label>{loan_type_label}		</td><td>{loan_type}</td></tr>
  <tr><td class=label>{amount_label}		</td><td>{amount}</td></tr>
  <tr><td class=label>{check_number_label}	</td><td>{check_number}</td></tr>
  <tr><td class=label>{check_status_label}	</td><td>{check_status}</td></tr>
  <tr> <!-- // PATCH 2008.07.21 -->
      <td class=label>{xp_dates}</td>
      <td><br>
        <table class="chart">
          <tr>
             <td>
               <iframe src="index.popup.php?scr_name=LN.SCR.changeDate&id={master_id}" width="400px" height="120px" scrolling="no" frameborder="0"></iframe>
            </td>
          </tr>
       </table>
     </td>
  </tr>

</table>

<br>
<!-- // PATCH 2008.07.21 -->
<table cellspacing=0 cellpadding=0 border=0>
<form name="showContract" action="index.popup.php" target="_blank">
<input type="hidden" name="scr_name" value="LN.SCR.groupContract" />
{showContract_id_html}
  <tr>
    <td>{showContract_tpl_html}</td>
    <td width="20px"></td>
    <td>{showContract_submit_html}</td>
  </tr>
</form>
</table>

<table><tr><td class=label><br>{chart_title}	</td><td></td></tr></table>
{chart}
<table>
<tr><td>{lnk_cancellation_letter}</td></tr>
<tr><td>{lnk_promissory_note}</td></tr>
<tr><td>{lnk_original_payment_plan}</td></tr>
</table>
<br /><br /><br /><br />

