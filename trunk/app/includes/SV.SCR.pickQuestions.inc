<?php
$btn  = new TTFButton(WEBPAGE::$theme);
?>
<HEAD>
<TITLE><?= $_CONF['client_name'] ?></TITLE>
<LINK href="css/style.css" rel="stylesheet" type="text/css">
<SCRIPT type="text/javascript" src="CS.functions.js"></SCRIPT>
	<style TYPE="text/css">
	<!-- 
	body {
	background: #ffffff;
	background-image: url("img/background.gif");
	background-repeat: no-repeat;
	background-position-y: -240px;	
	background-position-x: -50px;	
	background-attachment: fixed;	
	margin-top:  10px;
	margin-left: 10px;
	
	scrollbar-face-color: white; 
	scrollbar-shadow-color: #999999;
	scrollbar-highlight-color: white;
	scrollbar-3dlight-color: white;
	scrollbar-darkshadow-color: white;
	scrollbar-track-color: white;
	scrollbar-arrow-color: #cccccc;	
	
	}
	.link03 { color: #3399CC; }

	-->
	</style>
</HEAD>


<SCRIPT language=javascript>
list = new Array();
nlist = new Array();
function preLoad() {
	question_list = window.opener.document.surveyForm.question_list.value;
    if (question_list == "") { 
	list = new Array; 
	} else {
	list = question_list.split(",");
	n = list.length;
	m = this.document.anchors.length;
	for (i=0; i<n; i++) {
	    AnchorName = "ref"+list[i];
	    for (j=0; j<m; j++) {
	        if ( this.document.anchors[j].name == AnchorName ) { 
		    this.document.anchors[j].className="link03"; 
		    }
		}
	    }
	
	}
    }
function resetList() {
	list = new Array;
	m = this.document.anchors.length;	 
	for (j=0; j<m; j++) {
		this.document.anchors[j].className="link05"; 
		}
    }    
function pickItem(id) {
    var question_list="";
    n = list.length;
    add = "yes";
    c = 0;
    for (i=0; i<n; i++) {
	if (list[i]==id) { add = "no"; } else { nlist[c] = list[i]; c++; }
	}
    if ( add == "yes" ) { nlist[c] = id; }
    list = nlist;
    nlist = new Array();
    }
function complete() {
    window.opener.document.surveyForm.question_list.value = "";
    var n = list.length;
    if ( n > 0 ) {
	question_list = list[0]; 
	for (i=1; i<n; i++) {
	    question_list += "," + list[i];
	    }
	window.opener.document.surveyForm.question_list.value = question_list;
	}
//	window.opener.document.surveyForm.reload.value = 1;
    window.opener.document.surveyForm.submit();
    window.close();
    }
</SCRIPT>
<body onLoad=preLoad()>
<?
$mrow = SQL::select('tblSurveyItems','*',sprintf("creator_id = '%s' AND status = 'P'", WEBPAGE::$userID));
if (!( $num = count($mrow) )) {
  ?>
  <script>
  window.close();
  </script>
  <?
  exit;
  }
for ($i=0;$i<$num;$i++) {
  $c = count($row[$mrow[$i][category]]);
  $row[$mrow[$i][category]][$c] = $mrow[$i];
  }
  ?>
<a name=top></a>
<!--
<table class=msg align=center>
  <tr>
    <td><img src="<?= sprintf("%s%s/icons/info.png",WEBPAGE::_THEMES_PATH,WEBPAGE::$theme); ?>"></td>
    <td><h1><?= $_LABELS['MSG.INF.005'] ?></h1></td>
  </tr>
</table>
-->
<table align=center cellpadding=10>    
  <tr>
    <td cellpadding=0 cellspacing=0> 
	  <center><h1><?= $_LABELS['tblSurveys.question_list']; ?></h1></center>
      <br>	
	  <table cellpadding=2 cellspacing=1 class=lst width=100%>
	    <tr>
		  <?
		  $cat = array_keys($row);
		  for ($i=0;$i<count($cat);$i++) {
		    $cat_label[$i] = 'tblSurveyItems.category.'.$cat[$i];
			printf ("<td class=lst align=center><A class=lst href=#%s>%s</A></td>", $cat[$i], $_LABELS[$cat_label[$i]]);	
			}
		  ?>
		</tr>
	  </table>
	  <br>
    </td>
  </tr>
  <tr>
    <td>
      <? 
	  for ($i=0;$i<count($cat);$i++) {
	  ?>
      <table cellpadding=0 cellspacing=1>
      <caption class=lst><A name=<?= $cat[$i]; ?>></A><?= $_LABELS[$cat_label[$i]]; ?><hr style='color:silver' size=1></caption>
      <?
	  for ($j=0; $j<count($row[$cat[$i]]); $j++) {
	    $srow = $row[$cat[$i]][$j];
    	$answer_txt = explode("|", $srow[answer_txt]);
    	$answer_num = explode("|", $srow[answer_num]);    		
		?>
		<tr>
		  <td class=list>
		    <table width=500px cellpadding=0 cellspacing=0>
			<?
			print("<tr><td colspan=3 valign=top><A name=ref$srow[id] href=javascript:pickItem('$srow[id]') class=link05 onclick=\"if (this.className=='link05') { this.className='link03'; } else { this.className='link05'; } \">$srow[question]</A></td></tr>");
			$NOA = count($answer_txt);
			for ($k=0; $k<$NOA; $k++) {
	    	  $n = $k + 1;
	    	  print("<tr><td class=n02 align=right width=50px valign=top>$n.</td><td class=n02 valign=top width=350px>$answer_txt[$k]</td><td class=n02 align=center width=100px valign=top>[ $answer_num[$k] ]</td></tr>");
	    	  }
	    	?>
            </table>
		    <br>
	      </td>
	    </tr>
	    <?
        }
      ?>
      </table>
      <br><br>
	  <?
	  }
	?>	
	<br>
    </td>
  </tr>
</table>	
<center>
<?= $btn->imglink('JavaScript:window.close()',$_LABELS['cancel']); ?>
<?= $btn->imglink('JavaScript:resetList()',$_LABELS['reset']); ?>
<?= $btn->imglink('JavaScript:complete()',$_LABELS['submit']); ?>
</center>
</body>
</html>