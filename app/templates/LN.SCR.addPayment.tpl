<h1>{title}</h1>
<br>{message}<br>
<table cellpadding=0 cellspacing=0 border=0>
  <tr>
    <td class=label>{loan_code_label}</td>
    <td>{loan_code}</td>
  </tr>
  <tr>
    <td class=label>{client_label}</td>
    <td>{borrower_name}</td>
  </tr>
  <tr>
    <td></td>
    <td>{borrower_code}<br><br></td>
  </tr>
  <tr>
    <td></td>
    <td class=label>{pickOptionToContinue}<br><br></td>
  </tr>
  <tr>
    <td class=label></td>
    <td><li><a href='index.popup.php?scr_name=LN.SCR.addPaymentXT&id={loan_id}'>{addPaymentXT}</a></td>
  </tr>
  <tr><td><br></td></tr>
  <tr>
    <td class=label></td>
    <td><li><a href='index.popup.php?scr_name=LN.SCR.addPaymentEarly&id={loan_id}'>{addPaymentEarly}</a></a></td>
  </tr>
  <tr><td><br></td></tr>
  <tr>
    <td class=label></td>
    <td><li><a href='index.popup.php?scr_name=LN.SCR.addPaymentFull&id={loan_id}'>{addPaymentFull}</a></a></td>
  </tr>
</table>
<br><br>

