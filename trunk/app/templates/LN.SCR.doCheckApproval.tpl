<br>
<table cellpadding=0 cellspacing=0>
  <tr><td class=label>{zone_label}			</td><td>{zone}</td></tr>
  <tr><td class=label>{borrower_label}		</td><td>{borrower}</td></tr>
  <tr><td class=label>{borrower_type_label}	</td><td>{borrower_type}</td></tr>
  <tr><td class=label>{loan_type_label}		</td><td>{loan_type}</td></tr>
  <tr><td class=label>{amount_label}		</td><td>{amount}</td></tr>
  <tr><td class=label>{check_number_label}	</td><td>{check_number}</td></tr>
  <tr><td class=label>{check_status_label}	</td><td>{check_status}</td></tr>
  <tr><td class=label><br>{chart_title}		</td><td></td></tr>
</table>
{chart}
<br><br>
<table cellspacing=0 cellpadding=0 border=0>
{approvalForm_javascript}
<form {approvalForm_attributes}>
{approvalForm_scr_name_html}
{approvalForm_id_html}
  <tr>
    <td class=label>{approvalForm_memo_label}</td>
    <td>{approvalForm_memo_html}</td>
  </tr>
  <tr>    
    <td colspan=2 align=center>
	  <br>
	  {approvalForm_cancel_html}{approvalForm_reset_html}{approvalForm_submit_html}    
	</td>
  </tr>
</table>
</form>