<html>
<title>{html_title_client_name}</title>
<head>
<LINK href='themes/blue/style.login.css' rel='stylesheet' type='text/css'>
</head>
<body>
<table cellspacing=0 cellpadding=0 width='100%' height='100%' background='themes/blue/logo-watermark.png' class=login>
  <tr>
    <td colspan=2 class=logo align=center>{html_title_client_name}</td>
  </tr>
  <tr>
    <td width=100px>&nbsp;</td>
    <td>
      <table cellspacing=5 cellpadding=0>
        <tr>
          <td width='100px' valign=top></td>
          <td><img class=logo src='themes/blue/loginlogo.png' alt=''><br><br><br></td>
        </tr>
        <tr>
          <td width='100px' rowspan=3 valign=top></td>
          <td class=welcome>{db_outdated}<hr></td>
        </tr>
        <tr>
	      <td class=contact>{db_outdated_inf}</td>
        </tr>
        <tr>
          <td>
	        <br>
	        <span class=msg>{db_outdated_wrn}</span>
	        <table border=0 cellspacing=0 cellpadding=0 width=250px>
	          <tr><td colspan=2 align=right><br>{update_button}</td></tr>
	          <tr><td colspan=2 class=contact align=left><br><br><br><br><br>Ver. {emms_version}</td></tr>
	        </table>
          </td>
        </tr>
      </table>
    </td>
  </tr>
  <tr>
    <td colspan=2 class=copyright><hr>{copyright}<br><br></td>
  </tr>
</table>
</body>
</html>