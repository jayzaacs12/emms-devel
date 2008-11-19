<table cellpadding=0 cellspacing=0 width='100%'>
  <tr><td align=right>{date}</td></tr>
</table>
<h1>{form_title}</h1>
<table cellpadding=0 cellspacing=0>
  <tr><td class=label>{group_label}		</td><td>{group}	</td></tr>
  <tr><td class=label>{branch_label}	</td><td>{branch}	</td></tr>
  <tr><td class=label>{advisor_label}	</td><td>{advisor}	</td></tr>
</table>
<form name=applyForm action='index.popup.php' method=POST>
<input type=hidden name=scr_name value='LN.SCR.addMultPayment'>
<input type=hidden name=apply value=true>
<table>
  <tr>
    <td width=10px>		{attendance}		</td>
    <td>				{attendance_label}	</td>
  </tr>
</table>
<table  class=chart>
  <tr>
    <td class=header>						</td>
	<td class=header>	{client_label}		</td>
	<td class=header>	{loan_label}		</td>
	<td class=header>	{pBalance_label}	</td>
	<td class=header>	{principal_label}	</td>
	<td class=header>	{interest_label}	</td>
	<td class=header>	{payment_label}		</td>
	<td class=header>						</td>
  </tr>
  <!-- BEGIN results --> 
  <tr>
    <td class=chart>				{att_pars}			</td>
	<td class=chart>				{client}			</td>
	<td align=center class=chart>	{loan}				</td>
	<td align=right class=chart> 	{pBalance}			</td>
	<td align=right class=chart> 	{principal}			</td>
	<td align=right class=chart> 	{interest}			</td>
	<td align=right class=chart> 	{payment}			</td>
	<td width=10px class=chart>		{pmt_pars}			</td>
  </tr>
  <!-- END results --> 
  <!-- BEGIN msg --> 
  <tr>
    <td class=chart>				{msg_att_pars}			</td>
	<td class=chart>				{msg_client}			</td>
	<td align=center class=chart>	{msg_loan}				</td>
	<td colspan=5 class=chart> 		{message}				</td>
  </tr>
  <!-- END msg --> 
</table>
<br>
{submit}
</form>