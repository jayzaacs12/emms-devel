<?php
class BUSINESS extends BUSINESS_TYPE
{  
  /* BUSINESS parameters */
  var $frostdata 	= array();
  var $data  		= array('creator_id' => 0);
  var $businessfields  = array();

  function BUSINESS($id = '')  //class constructor
  {
  global $_CONF;
  if ($id) {
  
    $tables = 'tblBusiness as b, tblBusinessTypes as bt, tblUsers as uc, tblUsers as ue';
    $fields = sprintf("
	           b.*,bt.type,bt.activity,
	           CONCAT(uc.first,' ',uc.last) AS creator,
			   DATE_FORMAT(b.creator_date,'%s') AS f_creator_date,
		       CONCAT(ue.first,' ',ue.last) AS editor,  
		       DATE_FORMAT(b.editor_date,'%s') AS f_editor_date",
			   $_CONF['date_format_mysql'], $_CONF['date_format_mysql']);
	$params = sprintf('b.id=%s AND b.type_id = bt.id AND uc.id = b.creator_id AND ue.id = b.editor_id',$id);
    
    $this->data = current(SQL::select($tables,$fields,$params));
    foreach(explode(',', $this->data[client_list]) as $key=>$value) {
      $owner = new CLIENT($value);
      $this->data[sprintf('owner%s',$key+1)] = $owner->data[code];
      $this->data[owners][$owner->data[id]]   = $owner->data[name];
      }
    
	} else {

    $this->data['creator_date'] = date('Y-m-d');
    
    }
  }

  function getTemplateData($id)
  {
  global $_LABELS;

  $bz = new BUSINESS($id);
  $data = $bz->data;
    
  $c=0;
  $data['buttondata'][$c][id]	    = "BS.SCR.addBusiness";
  $data['buttondata'][$c][href]		= "index.php?scr_name=BS.SCR.addBusiness&id=".$id;
  $data['buttondata'][$c][alt]		= $_LABELS['edit'];
  $data['buttondata'][$c][onClick]	= "";
  $data['buttondata'][$c][ico]		= "edit";

  return $data;
  }
  
  function types()
  {
  return SQL::getAssoc_order('tblBusinessTypes','id,type',true,'activity');
  }
  
}
?>