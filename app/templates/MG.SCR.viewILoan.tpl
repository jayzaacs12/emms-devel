<table cellspacing=40 cellpadding=40 border=0>
  <tr><td colspan=2><h1>{title}</h1></td></tr>
  <tr>
    <td valign=top>
    <table cellspacing=0 cellpadding=0 border=0>
      <tr>
        <td class=label>{loan_code_label}</td>
        <td>{loan_code}<br></td>
      </tr>
      <tr>
        <td class=label>{status_label}</td>
        <td>{status}<br></td>
      </tr>
      <tr>
        <td class=label>{borrower_label}</td>
        <td><a href='{href_borrower}'>{borrower_name}</a></td>
      </tr>
      <tr>
        <td></td>
        <td>{borrower_code}</td>
      </tr>
      <tr>
        <td></td>
        <td>{borrower_type}</td>
      </tr>
      <tr>
        <td class=label>{survey_label}</td>
        <td><a href='{href_survey}'>{survey}</a></td>
      </tr>
      <tr>
        <td class=label>{business_label}</td>
        <td><a href='{href_business}'>{business}</a></td>
      </tr>
      <tr>
        <td class=label>{modality_label}</td>
        <td>{modality}</td>
      </tr>
      <tr>
        <td class=label>{kp_label} [ {currency_symbol} ]</td>
        <td>{kp}</td>
      </tr>	    
      <tr>
        <td class=label>{pmt_label}</td>
        <td>{pmt}</td>
      </tr>	    
      <tr>
        <td class=label>{savings_p_label}</td>
        <td>{savings_p}</td>
      </tr>
      <tr>
        <td class=label>{savings_v_label}</td>
        <td>{savings_v}</td>
      </tr>
      <tr>
        <td class=label>{installment_label}</td>
        <td>{installment}</td>
      </tr>
      <tr>
        <td class=label>{payment_frequency_label}</td>
        <td>{payment_frequency}</td>
      </tr>
      <tr>
        <td class=label>{rates_label} [ % ]</td>	
        <td></td>	
      </tr> 		    
      <tr>
        <td>{fees_at_label}</td>
        <td>{fees_at}</td>
      </tr>
      <tr>
        <td>{fees_af_label}</td>
        <td>{fees_af}</td>
      </tr>
      <tr>
        <td>{rates_r_label}</td>
        <td>{rates_r}</td>
      </tr>
      <tr>
        <td>{rates_d_label}</td>
        <td>{rates_d}</td>
      </tr>		    
      <tr>
        <td class=label>{rates_e_label}</td>
        <td>{rates_e}</td>
      </tr>	
    </table>
    </td>
    <td>&nbsp;</td>
    <td valign=top>
    <table cellspacing=0 cellpadding=0 border=0>
      <tr>
        <td class=label>
          <h1>{options}</h1><br>
          <ul>
          <li><a href='{pmtPlan_href}'>{pmtPlan_label}</a><br><br>
          <li><a href='{remove_href}'>{remove_label}</a><br><br>
          <li><a href='{new_href}'>{new_label}</a><br><br>
          </ul>
        </td>
      </tr>
    </table>
    </td>
  </tr>
</table>
<br><br>