<h1>{title}</h1>
<br /><hr><br />
<table width='98%'><tr><td align=right>{doWriteOff}{applyPayment}{applyCharge}</td></tr></table>

      <h2>{general_info}</h2>
      <table cellspacing=0 cellpadding=0 border=0>
        <tr><td colspan=2 align=right><br></td></tr>
        <tr>
          <td valign=top>
            <table cellspacing=0 cellpadding=0 border=0>
              <tr><td class=label>{borrower_name_label}</td><td>{borrower_name}</td></tr>
              <tr><td class=label>{borrower_code_label}</td><td>{borrower_code}</td></tr>
              <tr><td class=label>{status_label}</td><td>{status}<br></td></tr>
              <tr><td class=label>{li_cause_label}</td><td>{li_cause}<br></td></tr>
            </table>
          </td>
          <td width=40px>&nbsp;</td>
          <td valign=top>
            <table cellspacing=0 cellpadding=0 border=0>
              <tr><td class=label>{loan_code_label}</td><td>{loan_code}<br></td></tr>
              <tr><td class=label>{modality_label}</td><td>{modality}</td></tr>
              <tr><td class=label>{currency_label}</td><td>{currency}</td></tr>
              <tr><td class=label>{kp_label} [ {currency_symbol} ]</td><td>{kp}</td></tr>
            </table>
          </td>
        </tr>
      </table>
      <br /><br />

      <h2>{transactions_resume}</h2>
      {transactions_resume_chart}

      <h2>{ini_balances}</h2>
      {ini_balances_chart}

      <h2>{payments}</h2>
      {payments_chart}

      <h2>{write_off_charges}</h2>
      {write_off_charges_chart}
         <br /><br /> <br /><br />
      {printerlink}
         <br /> <br /><br />
      
