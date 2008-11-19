<!-- BEGIN html --> 
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd"> 
<html>
<head>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-3">
<title>{html_title_client_name}</title>
<link href='{theme}style.css' rel='stylesheet' type='text/css'>
<link rel="stylesheet" href="../phplayersmenu-3.2.0/layerstreemenu.css" type="text/css">
<style type="text/css">
<!--
@import url("../phplayersmenu-3.2.0/layerstreemenu-hidden.css");
//-->
</style>
<script language="JavaScript" type="text/javascript" src="../phplayersmenu-3.2.0/libjs/layersmenu-browser_detection.js"></script>
<script language="JavaScript" type="text/javascript" src="../phplayersmenu-3.2.0/libjs/layerstreemenu-cookies.js"></script>
<script language="JavaScript" type="text/javascript" src="CS.functions.js"></script>
</head>
<body>
<table border=0 width='{screenWidth}px' cellspacing='0' cellpadding='0'>
  <tr>
    <td>
      <table border=0 cellspacing='0' cellpadding='0' width='100%'>
        <tr>
		  <td width='200px' align=center><img style='margin-top:8px;margin-bottom:8px;' src='{theme}logo.png' border='0'></td>
		  <td valign=bottom>
		    <table border=0 cellspacing='0' cellpadding='0' width='100%'>
			  <tr>
				<td align=right valign=bottom class=MFIname>{client_name}</td>
			  </tr>
			  <tr>
				<td align=right>{greetings}<br><br></td>
			  </tr>
			  <tr>
				<td align=right>
				  <!-- BEGIN tabmenu --> 
				    <table cellpadding='3px' cellspacing='1px'>
					  <tr>
					    <!-- BEGIN tabitem --> 
					      <td class=tabmenu><a class=tabmenu href='{URL}'>{TAB}</a></td>
					    <!-- END tabitem --> 
					  </tr>
					</table>				     
				  <!-- END tabmenu --> 
				</td>
			  </tr>
			</table>
		  </td>
	    </tr>
		<tr>
		  <td></td>
		</tr>
	  </table>
	</td>
  </tr>
  <tr>
	<td class=treemenucontainer height=3px></td>
  </tr>
  <tr>
	<td>
	  <table border=0 cellspacing="0" cellpadding="0" width='100%'>
	    <tr>
		  <td class=treemenucontainer width=200px valign=top>
		    <table cellpadding="5" width='100%'>
		      <tr>
			    <td align=center  class=treemenulabel>{treemenulabel}<br></td>
		      </tr>
		      <tr>
				<td class=treemenu height='300px' valign=top>{navtree}</td>
			  </tr>
			</table>
			<br>
		  </td>		  
		  <td valign=top align=left width='{screenWidth}px' class=content>
		    <!-- BEGIN path --><div class=path><span class=pathlabel>Path: </span><!-- BEGIN pathitem -->{PAD} {PATH}<!-- END pathitem --><!-- END path --></div>
			{message}
			<h1>{content_title}</h1>
			{content}
		  </td>
		</tr>
	  </table>
	</td>
  </tr>
</table>
<!-- END html --> 