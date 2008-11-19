<?php
class SURVEY_ITEM extends SURVEY
{  
  /* SURVEY_ITEM parameters */
  var $frostdata = array();
  var $data  = array( 'status'     => 'P',
  					  'NOA'		   => '2',
  					  'process'	   => '1',
                      'creator_id' => 0);
  var $fields = array();

  function SURVEY_ITEM($id = '')  //class constructor
  {
  if ($id) {

    $this->data = current(SQL::select('tblSurveyItems','*',sprintf('id=%s',$id)));
    $num = explode('|',$this->data[answer_num]);
    $txt = explode('|',$this->data[answer_txt]);
    $this->data[NOA] = count($txt);
    for($i=0;$i<$this->data[NOA];$i++) {
      $this->data[sprintf("answer_txt_%s",$i)] = $txt[$i];
      $this->data[sprintf("answer_num_%s",$i)] = $num[$i];
	  }
      
    } else {

    $this->data['creator_date'] = date('Y-m-d');
    $this->frostdata = array();  

    }
  }

  function getTemplateData($id)
  {
  $sitem = new SURVEY_ITEM($id);
  return $sitem->data;
  }
  
}
?>