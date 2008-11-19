<table cellspacing=0 cellpadding=0 border=0>
<caption><br></caption>
{fundForm_javascript}
<form {fundForm_attributes}>
{fundForm_scr_name_html}
{fundForm_id_html}
{fundForm_sponsor_id_html}
  <tr>
    <td class=label>{fundForm_sponsor_label}</td>
    <td>{fundForm_sponsor_html}</td>
  </tr>
  <tr>
    <td class=label>&nbsp;</td>
    <td><a href='{donation_link_href}' onClick=''>{donation_link_text}</a></td>
  </tr>
  <tr>
    <td><br /></td>
  </tr>
  <tr>
    <td class=label>{fundForm_src_amount_label}</td>
    <td>{fundForm_src_amount_html}</td>
  </tr>
  <tr>
    <td class=label>{fundForm_src_currency_id_label}</td>
    <td>{fundForm_src_currency_id_html}</td>
  </tr>
  <tr>
    <td><br /></td>
  </tr>
  <tr>
    <td class=label>{fundForm_conv_amount_label}</td>
    <td>{fundForm_conv_amount_html}</td>
  </tr>
  <tr>
    <td class=label>{fundForm_conv_currency_id_label}</td>
    <td>{fundForm_conv_currency_id_html}</td>
  </tr>
  <tr>
    <td colspan=2 align=center>
	  <br>
	  {fundForm_cancel_html}{fundForm_reset_html}{fundForm_submit_html}
	  {fundForm_close_html}{fundForm_edit_html}{fundForm_view_html}
	</td>
  </tr>
</table>