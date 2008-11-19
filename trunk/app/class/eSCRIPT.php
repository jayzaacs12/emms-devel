<?php
class eSCRIPT extends WEBPAGE implements ROLES
{
  
  /* eSCRIPT parameters */
  var $data  = array();
  var $fields = array();

  function eSCRIPT($id = '')  //class constructor
  {
  if ($id) {

    $this->data 					= current(SQL::select('tblMenus','*',sprintf('id=%s',$id)));
	$this->data['roles'] 			= $this->getRoles($this->data['access_code']); 
    
    } 
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
  
}
?>