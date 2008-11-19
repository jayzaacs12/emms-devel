<html>
<title>{html_title_client_name}</title>
<head>
{loginForm_javascript}
<LINK href='themes/blue/style.login.css' rel='stylesheet' type='text/css'>  
</head>
<body onLoad='javascript:document.loginForm.screenWidth.value=window.screen.width'>
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
          <td class=welcome>{welcome}<hr></td>
        </tr>
        <tr>
	      <td class=contact>{contact}</td>
        </tr>
        <tr>
          <td>
	        <br>
	        <span class=msg>{message}</span>
	        <form {loginForm_attributes}>
 	        {loginForm_screenWidth_html}
 	        {loginForm_lang_html}
	        <table border=0 cellspacing=0 cellpadding=0 width=250px>
	          <tr>
		        <td width=105px class=login>{loginForm_username_label}</td>
		        <td>{loginForm_username_html}</td>
	          </tr>
	          <tr>
		        <td class=login>{loginForm_password_label}</td>
		        <td>{loginForm_password_html}</td>
	          </tr>
	          <tr>
		        <td colspan=2 align=right><br>{loginForm_submit_html}</td>
	          </tr>
	          <tr><td colspan=2 class=contact align=left><br><br><br><br><br>Ver. {emms_version}</td></tr>
	        </table>
          </td>
        </tr>
      </table>
    </td>
  </tr>
  <tr>
    <td colspan=2 class=languages>
      <a href='index.php?lang=esp'>Espa&ntilde;ol</a> 
	  <a>|</a>
	  <a href='index.php?lang=eng'>English</a>  
	  <a>|</a>
	  <a href='index.php?lang=fra'>Français</a>
    </td>
  </tr>
  <tr>
    <td colspan=2 class=copyright><hr>{copyright}<br><br></td>
  </tr>
</table>
</body>
</html>