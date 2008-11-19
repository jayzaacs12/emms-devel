<br>
<table cellpadding=0 cellspacing=0>
  <tr><td class=label>{zone_label}			</td><td>{zone}</td></tr>
  <tr><td class=label>{borrower_label}		</td><td>{borrower}</td></tr>
  <tr><td class=label>{borrower_type_label}	</td><td>{borrower_type}</td></tr>
  <tr><td class=label>{loan_type_label}		</td><td>{loan_type}</td></tr>
  <tr><td class=label>{amount_label}		</td><td>{amount}</td></tr>
  <tr><td class=label>{check_status_label}	</td><td>{check_status}</td></tr>
  <tr><td class=label><br>{chart_title}		</td><td></td></tr>
</table>
{chart}
<br><br>
<table cellspacing=0 cellpadding=0 border=0>
{disbursementForm_javascript}
<form {disbursementForm_attributes}>
{disbursementForm_scr_name_html}
{disbursementForm_id_html}
  <tr>
    <td class=label>{disbursementForm_check_number_label}</td>
    <td>{disbursementForm_check_number_html}</td>
  </tr>
  <tr>
    <td class=label>{disbursementForm_fund_label}</td>
    <td>{disbursementForm_fund_html}</td>
  </tr>
  <tr>
    <td class=label>{disbursementForm_sponsor_id_label}</td>
    <td>{disbursementForm_sponsor_id_html}</td>
  </tr>
  <tr>
    <td class=label>{disbursementForm_memo_label}</td>
    <td>{disbursementForm_memo_html}</td>
  </tr>
  <tr>    
    <td colspan=2 align=center>
	  <br>
	  {disbursementForm_cancel_html}{disbursementForm_reset_html}{disbursementForm_submit_html}    
	</td>
  </tr>
</table>
</form>
