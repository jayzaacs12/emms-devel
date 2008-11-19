<table width="640px" height="350px">
  <tr>
    <td align="right">{print}</td>
  </tr>
  <tr>
    <td valign="top">
      <table width="640px">
  		<tr><td><h1>{organization}</h1>{address}</td></tr>
      </table>
      <table width="640px">
  		<tr><td><b>{page_title} # {receipt_number}</b></td></tr>
      </table>
      <table width="640px">
        <tr><td><p>{paymentReceipt} {createdByOn}</td></tr>
        <tr><td></td></tr>
      </table>
      <table width="640px">
        <!-- <tr><td><b>{delay_label}</b></td><td>{delay}</td></tr> -->
        <tr><td><b>{program_label}</b></td><td>{program}</td><td><b>{loanmaster_id_label}</b></td><td>{loanmaster_id}</td></tr>
  		<tr><td><b>{advisor_label}</b></td><td>{advisor}</td><td><b>{balance_label}</b></td><td>{balance}</td></tr>
  		<tr><td><b>{memo_label}</b></td><td colspan="3">{memo}</td></tr>
  		<tr><td></td><td colspan="3"><font color="red"><b>{pmt_receipt_flag_all}</b></font></td></tr>
  		<tr><td></td></tr>
  		<tr><td><b>{details_label}</b></td><td></td></tr>
	  </table>
      <table width="640px">
        <tr><td>{chart}</td></tr>
      </table>
    </td>
  </tr>
</table>