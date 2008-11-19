<html>
<title>{html_title_client_name}</title>
<head>
{loginForm_javascript}
<LINK href='themes/blue/style.login.css' rel='stylesheet' type='text/css'>
</head>
<body onLoad='javascript:document.loginForm.screenWidth.value=window.screen.width'>
<br /><br />
<table cellspacing=0 cellpadding=0 background='themes/blue/logo-watermark.png' class=login>
  <tr>
    <td>
      <table cellspacing=2 cellpadding=0 border=0 width='500px'>
        <tr>
          <td rowspan=6 width='100px'></td>
          <td class=sponsorship_program_name>{sponsorship_program_name}</td>
        </tr>
        <tr>
          <td class=client_name>{client_name}<!--<hr>--><br /><br /></td>
        </tr>
        <tr>
          <td class=languages>
            <a href='index.php?lang=esp'>Espa&ntilde;ol</a>
	          <a>|</a>
	          <a href='index.php?lang=eng'>English</a>
	          <a>|</a>
	          <a href='index.php?lang=fra'>Français</a>
          </td>
        </tr>
        <tr>
	      <td class=contact><!--<br /><br /><br />{contact}--></td>
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
	        </table>
          </td>
        </tr>
        <tr>
           <td class=copyright><!--<hr>--><b>Powered by e-MMS</b><br />{copyright}<br><br></td>
        </tr>
      </table>
    </td>
  </tr>
</table>
</body>
</html>