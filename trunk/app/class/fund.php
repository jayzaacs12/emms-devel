<?php
class FUND extends WEBPAGE
{  
  /* FUND parameters */
  var $frostdata = array();
  var $funddata  = array( 'status'     => 'A',
                          'creator_id' => 0);
  var $fundfields = array();

  function FUND($id = '')  //class constructor
  {
  if ($id) {

    $this->funddata = current(SQL::select('tblFunds','*',sprintf('id=%s',$id)));
    
    } else {

    $this->funddata['creator_date'] = date('Y-m-d');
    $this->frostdata = array();  

    }
  }

  function getTemplateData($id)
  {
  global $_LABELS;
  global $_CONF;

  $fields = sprintf("		    
  				f.*, c.currency,
		    	CONCAT(uc.first,' ',uc.last) AS creator, 
		    	DATE_FORMAT(f.creator_date,'%s') AS f_creator_date,
		    	CONCAT(ue.first,' ',ue.last) AS editor,  
		    	DATE_FORMAT(f.editor_date,'%s') AS f_editor_date", $_CONF['date_format_mysql'], $_CONF['date_format_mysql']);
  $tables = "tblFunds AS f, tblCurrencys AS c, tblUsers AS uc, tblUsers AS ue";
  $param  = sprintf("f.id = '%s' AND c.id = f.currency_id AND f.creator_id = uc.id AND f.editor_id = ue.id",$id);

  $data 				  = current(SQL::select($tables, $fields, $param));
    
  $c=0;
  $data['buttondata'][$c][id]	    = "AC.SCR.addFund";
  $data['buttondata'][$c][href]		= "index.php?scr_name=AC.SCR.addFund&id=".$id;
  $data['buttondata'][$c][alt]		= $_LABELS['edit'];
  $data['buttondata'][$c][onClick]	= "";
  $data['buttondata'][$c][ico]		= "edit";

  return $data;
  }
  
  function currencys()
  {
  return SQL::getAssoc('tblCurrencys','id,currency');
  }

}
?>