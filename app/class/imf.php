<?php
class IMF extends WEBPAGE implements ROLES

{
  
  /* IMF parameters */
  var $frostdata 	= array();
  var $imfdata  	= array();
  var $imffields 	= array();

  function IMF()  //class constructor
  {
  }

  function getTemplateData($id)
  {
  }
  
  function getBranches()
  {    
  return WEBPAGE::$dbh->getAssoc('select id,zone from tblZones where parent_id = 0');
  }

  function getPrograms($status)
  {    
  return WEBPAGE::$dbh->getAssoc(sprintf('select id,program from tblPrograms where status = "%s"',$status));
  }

  function getLoanOfficers($active)
  {  
  return WEBPAGE::$dbh->getAssoc(sprintf('select id,concat(first," ",last) as name from tblUsers where ( access_code & %s ) != 0 and active = "%s"',self::ADVISOR, $active));
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
  
}
?>
