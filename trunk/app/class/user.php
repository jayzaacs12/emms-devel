<?php
class USER extends WEBPAGE implements PERSONS, ROLES
{
  
  /* USER parameters */
  var $frostdata = array( 0 => 'username',
                          1 => 'creator_date',
						  2 => 'creator_date',
						  3 => 'active');
  var $userdata  = array( 'active'     => 1,
                          'creator_id' => 0);
  var $userfields = array();

  function USER($id = '')  //class constructor
  {
  if ($id) {

    $this->userdata 				= current(SQL::select('tblUsers','*',sprintf('id=%s',$id)));
	$this->userdata['roles'] 		= $this->getRoles($this->userdata['access_code']); //maskRoles(getRoles(),$user['access_code']);
    $this->userdata['verify'] 		= $this->userdata['password'];
    $this->userdata['oldpassword'] 	= $this->userdata['password'];
    
    } else {

    $this->userdata['creator_date'] = date('Y-m-d');
    $this->frostdata = array();  

    }
  }

  function getTemplateData($id)
  {
  global $_LABELS;
  global $_CONF;
  $fields = sprintf("	
  				u.*,
				CONCAT(uc.first,' ',uc.last) AS creator, 
				DATE_FORMAT(u.creator_date,'%s') AS f_creator_date, 
				CONCAT(ue.first,' ',ue.last) AS editor, 
				DATE_FORMAT(u.editor_date,'%s') AS f_editor_date, 
				CONCAT(us.first,' ',us.last) AS super, 
				DATE_FORMAT(u.birthdate,'%s') AS f_birthdate, 
				z.zone", $_CONF['date_format_mysql'], $_CONF['date_format_mysql'], $_CONF['date_format_mysql']); 
  $tables = "tblUsers AS u, tblUsers AS us, tblUsers AS uc, tblUsers AS ue";
  $left   = "tblZones AS z";		   
  $on     = "u.zone_id = z.id";
  $param  = sprintf("u.id = '%s' AND u.super_id = us.id AND u.creator_id = uc.id AND u.editor_id = ue.id", $id);

  $data 				  = current(SQL::select_leftjoin($tables, $fields, $left, $on, $param));
    
  $data['super']          = WEBPAGE::hotlink('BS.SCR.viewUser','id',$data['super_id'],$data['super']);
  $data['name']           = sprintf("%s %s %s", $data['first'],$data['middle'],$data['last']);
  $data['status']         = $_LABELS[sprintf('tblUsers.active.%s',$data['active'])];
  $data['zone']           = WEBPAGE::hotlink('BS.SCR.viewZone','id',$data['zone_id'],$data['zone']);
  $data['membership']     = self::getMembership($data['access_code']);

  $data['img_path']		  = sprintf(WEBPAGE::_IMG_USER,$data['username']);
  
  if (($_CONF[auto_photo])&&(!(file_exists($data['img_path'])))) {
    $img_remote_path = sprintf("http://web.jce.do/consultas/FOTOS/%s/%s/%s/%s.jpg",substr($data['code'],0,3),substr($data['code'],4,2),substr($data['code'],6,2),$data['code']);
	
	if (!($fp_remote=@fopen($img_remote_path, "r"))) {
	  $data['img_path']	= './img/unknown.png';
	  } else {
      $data['img_path'] = $img_remote_path;
  	  }
/*
  		if ($fp_remote=fopen($img_remote_path, "r")) {
			$fp_local = fopen($img_path,"w");
			$image = fread($fp_remote,102400);
			fwrite($fp_local,$image,102400);
    		fclose($fp_remote);
    		fclose($fp_local);
			}
*/
    }

    $c=0;
	$data['buttondata'][$c][id]			="BS.SCR.addUser";
	$data['buttondata'][$c][href]		="index.php?scr_name=BS.SCR.addUser&id=".$id;
	$data['buttondata'][$c][alt]		= $_LABELS['edit'];
	$data['buttondata'][$c][onClick]	="";
	$data['buttondata'][$c][ico]		="edit";

	if ($data['active']) {
	  $c++;
	  $data['buttondata'][$c][id]		="BS.SCR.suspendUser";
	  $data['buttondata'][$c][href]		="index.php?scr_name=BS.SCR.suspendUser&id=".$id;
	  $data['buttondata'][$c][alt]		= $_LABELS['deactivate'];
	  $data['buttondata'][$c][onClick]	="";
	  $data['buttondata'][$c][ico]		="user_rem";
	  }

  return $data;
  }
  
  function roles($access_code = 255)  //implements interface ROLES
  {
  global $_LABELS;
  $roles = array();
  $opt = explode(",", $_LABELS["tblUsers.access_code.optList"]);
  $nam = explode(",", $_LABELS["tblUsers.access_code.optNames"]);
  $n = count($opt);
  for ($i=0; $i<$n; $i++) {
    $val = explode("|", $opt[$i]);
    if ($val[1] & intval($access_code)) {
      $roles[$i] = array('role'=>$val[0], 'label'=>$nam[$i], 'code'=>$val[1]);
      }
    }
  return $roles;
  }    

  function supervisors() //implements interface PERSONS
  {
  return SQL::getAssoc('tblUsers','id,username',sprintf("( access_code & %s ) != 0", self::SUPER));
  }

  function advisors($opt='') //implements interface PERSONS
  {
  //return SQL::getAssoc('tblUsers','id,username',sprintf("( access_code & %s ) != 0", self::ADVISOR));
    if ($opt == '') {
    return SQL::getAssoc('tblUsers','id,username',sprintf("( access_code & %s ) != 0", self::ADVISOR));
    } else {
    return WEBPAGE::$dbh->getAssoc(sprintf("select max(u.id) id, concat(u.last,', ',u.first,' - ',u.username) username from tblUsers u, tblLoans l, view_loan_status vls where u.id = l.advisor_id and l.id = vls.loan_id and vls.status = '%s' group by u.username order by u.last",$opt));
    }

  }

  function getMyAdvisors($id = '') 
  {
  if (!($id)) { $id = $this->userdata['id']; }
  return SQL::getAssoc('tblUsers','id,username',sprintf("((super_id = '%s') OR (id = '%s')) AND ( access_code & %s ) != 0", $id, $id, self::ADVISOR));
  }

  function zones() //implements interface PERSONS
  {
  return SQL::getAssoc('tblZones','id,zone','parent_id = 0');
  }


  function getRoles($access_code) 
  {
  $roles = array();
  foreach (self::roles() as $key => $value) {
    if ($value['code'] & intval($access_code)) {
      $roles[$key] = $value['code'];
      }
    }
  return $roles;
  }

  function getMembership($access_code) 
  {
  $membership = array();
  $allroles = self::roles();
  foreach (self::getRoles($access_code) as $key => $value) {
    $membership[$key] = $allroles[$key]['label'];
    }
  return $membership;
  }
    
  function getName($userID) 
  {
  return current(SQL::getAssoc('tblUsers',"id, CONCAT(first, ' ', last)", sprintf("id = '%s'",$userID)));
  }

}
?>