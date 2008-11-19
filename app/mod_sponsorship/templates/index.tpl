<!-- BEGIN html -->
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-3">
<title>{html_title_client_name}</title>
<link href='{theme}style.css' rel='stylesheet' type='text/css'>
<script language="JavaScript" type="text/javascript" src="../CS.functions.js"></script>
</head>
<body onLoad='this.focus()'>
<table border=0 width='95%' cellspacing='0' cellpadding='0'>
  <tr>
    <td class=MFIname align=right>{client_name}</td>
  </tr>
  <tr>
    <td align=right>{greetings}</td>
  </tr>
  <tr>
    <td class=sponsorship_program_name>{sponsorship_program_name}</td>
  </tr>
  <tr>
    <td class=sponsor_name>{sponsor_name}</td>
  </tr>
  <tr>
    <td class=sponsor_contact>{sponsor_contact}</td>
  </tr>
  <!--
  <tr>
    <td class=content><div class=login_info><span class=login_info_label>{login_info_label}: </span>{sponsor_contact_name}</div></td>
  </tr>
  -->
  <tr>
    <td class=content>
    <br />{go_home}<br /><br />{message}
    <h1>{content_title}</h1>
    {content}
    <br /><br />
    <table cellpadding=0 cellspacing=0><tr><td>{gohome_ico}</td><td width=5px>&nbsp;</td><td>{gohome_txt}</td></tr></table>
    </td>
  </tr>
</table>
<!-- END html -->