<?php
class UPROFILE
{
    /* UPROFILE parameters */
    var $id = '';
    var $username = '';
    var $password = '';
    var $creator = '';
    var $creator_date = '';
    var $editor = '';
    var $editor_date = '';
    var $access_code = '';
    var $super = '';
    var $code = '';
    var $first = '';
    var $middle = '';
    var $last = '';
    var $birthdate = '';
    var $gender = '';
    var $email = '';
    var $zone = '';
    var $memo = '';

    function UPROFILE($data)
    {
    global $_LABELS;
	$this->id = $data[id];
	$this->active = $data[active];
	$this->username = $data[username];
	$this->password = $data[password];
	$this->creator = $data[creator];
	$this->creator_date = $data[creator_date];
	$this->editor = $data[editor];
	$this->editor_date = $data[editor_date];
	$this->access_code = $data[access_code];
    $this->super = coollink('index.php?scr_name=BS.SCR.browseUsers','id',$data[super_id],$_LABELS['details'],'recordview',$data[super]);
	$this->code = $data[code];
    $this->first = $data[first];
	$this->middle = $data[middle];
    $this->last = $data[last];
    $this->name = sprintf("%s %s %s", $data[first],$data[middle],$data[last]);
    if (!($data[active])) { $this->name = sprintf("<strike>%s</strike> <font color=brown>%s</font>",$this->name, $_LABELS['MSG.INF.016']) ; }
	$this->birthdate = $data[birthdate];
	$this->gender = $data[gender];
    $this->email = $data[username]."@eird.net";
	if ($data[email]) { $this->email .= "<br>".$data[email]; }
    $this->zone = coollink('index.php?scr_name=BS.SCR.browseZones','id',$data[zone_id],$_LABELS['details'],'recordview',$data[zone]);

	$this->memo = $data[memo];
    }

    function display($UserID)
    {
	global $_LABELS;
	$sql = "SELECT 
		    u.id, 
		    u.username, 
		    u.password, 
		    CONCAT(uc.first,' ',uc.last) AS creator, 
		    DATE_FORMAT(u.creator_date,'%M %e, %Y') AS creator_date,
		    CONCAT(ue.first,' ',ue.last) AS editor,  
		    DATE_FORMAT(u.editor_date,'%M %e, %Y') AS editor_date,
		    u.access_code, 
		    u.super_id,
		    CONCAT(us.first,' ',us.last) AS super, 
		    u.code, 
		    u.first, 
		    u.middle, 
		    u.last, 
		    DATE_FORMAT(u.birthdate,'%M %e, %Y') AS birthdate,
		    u.gender, 
		    u.email,
			z.id AS zone_id, 
		    z.zone, 
		    u.memo,
			u.active  
		FROM 
		    (tblUsers AS u,tblUsers AS us,tblUsers AS uc,tblUsers AS ue)		   
		LEFT JOIN  tblZones AS z ON u.zone_id = z.id 
		WHERE 
		      u.id = '$UserID'
		  AND u.super_id = us.id 
		  AND u.creator_id = uc.id 
		  AND u.editor_id = ue.id";

	include("BS.PAR.accessgroups.inc");
	$n = count($agroups);
	$mrow = submitSelect_MultAssoc($sql);
	$num = count($mrow);
	for ($j=0; $j<$num; $j++) {
	    $row = $mrow[$j];
	    $uprofile = new UPROFILE($row);
	    $AccessCode = $uprofile->access_code;
	    for ($i=0; $i<$n; $i++) {
	    $c = $n - $i - 1;
	        if ($AccessCode >= $agroups[$c]->code) { 
		    $membership[] = $agroups[$c]->name;
		    $AccessCode -= $agroups[$c]->code;
		    }
		}
	    UPROFILE::printProfile($uprofile, $membership);    
	    }
	return $uprofile->id;
    }


  function printProfile($uprofile, $membership)
  {	
  global $_LABELS;
  global $_CONF;$_CONF[auto_photo]=1;
  $img_path = sprintf("img/users/%s.jpg",$uprofile->username);
  if (($_CONF[auto_photo])&&(!(file_exists($img_path)))) {
    $img_remote_path = sprintf("http://web.jce.do/consultas/FOTOS/%s/%s/%s/%s.jpg",substr($uprofile->code,0,3),substr($uprofile->code,4,2),substr($uprofile->code,6,2),$uprofile->code);
/*
  		if ($fp_remote=fopen($img_remote_path, "r")) {
			$fp_local = fopen($img_path,"w");
			$image = fread($fp_remote,102400);
			fwrite($fp_local,$image,102400);
    		fclose($fp_remote);
    		fclose($fp_local);
			}
*/
    $img_path = $img_remote_path;
    }

  $num = count($membership);
  for ($i=0; $i<$num; $i++) {
    $membershipList .= $membership[$i]."<br>";
    }
  $gen = "tblUsers.gender.".$uprofile->gender;
  $gen = $_LABELS[$gen];

  printf ("<h1 class=query>%s</h1>", $_LABELS['MSG.TTL.002']);

  ?>
  <table class=recordview>
  <caption class=recordview><?= $uprofile->name; ?></caption>
    <tr><td colspan=2><hr class=recordview></td></tr>
	<tr>
      <td>
        <?
		printf("
		  <table>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s. %s</td></tr>
	  		<tr><td class=recordviewlabel>%s</td><td class=recordviewvalue>%s. %s</td></tr>
		  </table>",
		  $_LABELS['tblUsers.username'], $uprofile->username,
		  $_LABELS['tblUsers.code'],$uprofile->code,
		  $_LABELS['tblUsers.email'],$uprofile->email,
		  $_LABELS['tblUsers.super_id'],$uprofile->super,
		  $_LABELS['tblZones.zone'],$uprofile->zone,
		  $_LABELS['tblUsers.birthdate'],$uprofile->birthdate,
		  $_LABELS['tblUsers.gender'],$gen,
		  $_LABELS['tblUsers.access_code'],$membershipList,
		  $_LABELS['tblUsers.memo'],$uprofile->memo,
		  $_LABELS['tblUsers.creator_id'],$uprofile->creator,$uprofile->creator_date,
		  $_LABELS['tblUsers.editor_id'],$uprofile->editor,$uprofile->editor_date
		  );
		?>
      </td>
	  <td class=recordviewvalue>
	    <img class=rpt src='<?= $img_path; ?>'> 
	  </td>
	</tr>
  </table>
  <?
  $buttondata = UPROFILE::genButtonData($uprofile);
  $bar = New SCRBAR($buttondata);
  SCRBAR::printbar($bar);
  }

    function getName($userID) {
    	$sql = "SELECT CONCAT(first, ' ', last) AS name from tblUsers where id = '$userID'";
    	$data = submitSelect_OneAssoc($sql);
    	return $data[name];
    }

    function getZoneID($userID) {
    	$sql = "SELECT zone_id from tblUsers where id = '$userID'";
    	$data = submitSelect_OneAssoc($sql);
    	return $data[zone_id];
    }	

    function getRoles($Perm) 
	{
	global $_LABELS;	
	$opt = explode(",", $_LABELS["tblUsers.access_code.optList"]);
	$nam = explode(",", $_LABELS["tblUsers.access_code.optNames"]);
	$n = count($opt);
	for ($i=0; $i<$n; $i++) {
    	$val = explode("|", $opt[$i]);
    	if ($val[1] & intval($Perm)) { $roles[] = $nam[$i]; }
    	}
    return $roles;   
    }

    function genButtonData($uprofile)
    {	
    global $_LABELS;
    $c=0;
	$buttondata[$c][id]="BS.SCR.addUser";
	$buttondata[$c][href]="index.php?scr_name=BS.SCR.addUser&id=".$uprofile->id;
	$buttondata[$c][alt]= $_LABELS['edit'];
	$buttondata[$c][onClick]="";
	$buttondata[$c][ico]="edit";

	if ($uprofile->active) {
		$c++;
		$buttondata[$c][id]="BS.SCR.suspendUser";
		$buttondata[$c][href]="index.php?scr_name=BS.SCR.suspendUser&id=".$uprofile->id;
		$buttondata[$c][alt]= $_LABELS['deactivate'];
		$buttondata[$c][onClick]="";
		$buttondata[$c][ico]="deactivate";
		}
	return $buttondata;
	}
}
?>