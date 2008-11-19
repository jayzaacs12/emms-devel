<?php
class ZONE extends WEBPAGE
{
  
  /* ZONE parameters */
  var $frostdata = array( 0 => 'username',
                          1 => 'creator_date',
						  2 => 'creator_date',
						  3 => 'active');
  var $zonedata  = array( 'active'     => 1,
                          'creator_id' => 0);
  var $zonefields = array();

  function ZONE($id = '')  //class constructor
  {
  if ($id) {

    $this->zonedata 				= current(SQL::select('tblZones','*',sprintf('id=%s',$id)));
    
    } else {

    $this->zonedata['creator_date'] = date('Y-m-d');
    $this->frostdata = array();  

    }
  }

  function getTemplateData($id)
  {
  global $_LABELS;
  global $_CONF;
  $fields = sprintf("	
  				z.*,
				CONCAT(uc.first,' ',uc.last) AS creator, 
				DATE_FORMAT(z.creator_date,'%s') AS f_creator_date, 
				CONCAT(ue.first,' ',ue.last) AS editor, 
				DATE_FORMAT(z.editor_date,'%s') AS f_editor_date", $_CONF['date_format_mysql'], $_CONF['date_format_mysql'], $_CONF['date_format_mysql']); 
  $tables = "tblZones AS z, tblUsers AS uc, tblUsers AS ue";
  $param  = sprintf("z.id = '%s' AND z.creator_id = uc.id AND z.editor_id = ue.id", $id);

  $data = current(SQL::select($tables, $fields, $param));

  $c=0;
  $data['buttondata'][$c][id]		= "BS.SCR.addZone";
  $data['buttondata'][$c][href]		= "index.php?scr_name=BS.SCR.addZone&id=".$id;
  $data['buttondata'][$c][alt]		= $_LABELS['edit'];
  $data['buttondata'][$c][onClick]	= "";
  $data['buttondata'][$c][ico]		= "edit";
	
  return $data;
  }
  
  function getPrograms($id = '')
  {
  if (!($id)) {$id = $this->zonedata['id'];}
  $fields = 'z.program_id AS id, p.program AS program'; 
  $table = 'tblZones AS z, tblPrograms AS p';
  $param = sprintf("z.parent_id = '%s' AND p.id = z.program_id",$id);  
  return SQL::getAssoc($table,$fields,$param);
  }
  
  function programs($id = '')
  {
  if (!($id)) {$id = $this->zonedata['id'];}
  $fields = 'p.id, p.program'; 
  $tables = 'tblPrograms AS p';
  $left = 'tblZones AS z'; 
  $on = sprintf("p.id = z.program_id AND z.parent_id = '%s'",$id);
  $param = "z.program_id IS NULL";
  
  return (SQL::getAssoc_leftjoin($tables, $fields, $left, $on, $param));
  }

}
?>