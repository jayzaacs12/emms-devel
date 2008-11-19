feature = "";

var edit=new Image();
var edit_on=new Image();
edit.src="img/icons/edit.ico";
edit_on.src="img/icons/edit_on.ico";

function setSociety(id,code,zone_id,advisor_id) {
    window.opener.document.theform.society_id.value = id;
    window.opener.document.theform.society.disabled = false;
    window.opener.document.theform.society.value = code;
    window.opener.document.theform.society.disabled = true;
    window.opener.document.theform.zone_id.disabled = false;
    window.opener.document.theform.zone_id.value = zone_id;
    window.opener.document.theform.zone_id.disabled = true;
    window.opener.document.theform.advisor_id.disabled = false;
    window.opener.document.theform.advisor_id.value = advisor_id;
    window.opener.document.theform.advisor_id.disabled = true;
    window.close();
    }
    
function openWin(archive,name,options) {
	if (options == '') { options = 'menubar=no,scrollbars=yes,resizable=no,width=600,height=400'; }
	second=window.open(archive,name,options);
}
    
function popupform(theForm) {
  if (! window.focus) return true;
  windowname = new Date();
  windowname = windowname.toString();
  window.open('', windowname, 'menubar=no,scrollbars=yes,resizable=yes,width=750,height=550');
  theForm.target=windowname;
  return true;
  }

function closeFrameless() { 
  top.window.close();
  }

function openFrameless(windowX,windowY,windowW,windowH,urlPop,title){

var autoclose = true
s = "width="+windowW+",height="+windowH;
var beIE = document.all?true:false

  if (beIE){
    NFW = window.open("","popFrameless","fullscreen,"+s)  
    NFW.blur()
    window.focus()       
    NFW.resizeTo(windowW,windowH)
    NFW.moveTo(windowX,windowY)
    var frameString=""+
"<html>"+
"<head>"+
"<title>"+title+"</title>"+
"</head>"+
"<frameset rows='*,0' framespacing=0 border=0 frameborder=0>"+
"<frame name='top' src='"+urlPop+"' scrolling=no>"+
"<frame name='bottom' src='about:blank' scrolling='no'>"+
"</frameset>"+
"</html>"
    NFW.document.open();
    NFW.document.write(frameString)
    NFW.document.close()
  } else {
    NFW=window.open(urlPop,"popFrameless","scrollbars,"+s)
    NFW.blur()
    window.focus() 
    NFW.resizeTo(windowW,windowH)
    NFW.moveTo(windowX,windowY)
  }   
  NFW.focus()   
  if (autoclose){
    window.onunload = function(){NFW.close()}
  }
}
function pickServerSide(issue) {
    pickWin = window.open('', issue, 'scrollbars=1,titlebar=0');
    pickWin.document.writeln('<LINK href=css/style.css rel=stylesheet type=text/css><TITLE>DEMO</TITLE>');
    pickWin.document.writeln(issue);
    }
function pickClientSide(f,e1,v1,e2,v2) {
    document.forms[f].elements[e1].value = v1;
    document.forms[f].elements[e2].value = v2;
    }
function set(feature) {
    if (document.layers) {
	document[feature].visibility = 'visible';
	} else {
	document.all[feature].style.visibility = 'visible';
	}
    }
function hide(feature) {
    if (document.layers) {
	document[feature].visibility = 'hidden';
	} else {
	document.all[feature].style.visibility = 'hidden';
	}
    }

function changeMouseFlag(f) {

	if (( f == "" )&&( feature != "" )) { 
		hide(feature);
		feature = "";
		document.onmousemove=null;
		return true;
		}

	
	if (( f != "" )&&( feature == "" )) { 
		feature = f;
		if (document.layers)  document.captureEvents(Event.MOUSEMOVE); 
		document.onmousemove=mtrack;
		set(feature);
		return true;
		}


	if (( f != "" )&&( f == feature ))  { 
		hide(feature);
		feature = "";
		document.onmousemove=null;
		return true;
		}

	if (( f != "" )&&( f != feature ))  { 
		hide(feature);
		feature = f;
		set(feature);
		return true;
		}
}

function mtrack(e) {
   var Text= 'Coordinates: ';

   if (document.layers) {
      document[feature].top = e.pageY;
      document[feature].left = e.pageX;
      Text += e.pageX+','+e.pageY
   }
   else if (document.all) {
      document.all[feature].style.top =  event.clientY;
      document.all[feature].style.left =  event.x;
      Text += event.x+','+event.y;
   }

//   window.status= Text;

}
