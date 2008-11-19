<?php
class SURVEY extends WEBPAGE
{  
  /* SURVEY parameters */
  var $frostdata = array();
  var $data  = array( 'status'     => 'F',
                          'creator_id' => 0);
  var $fields = array();

  function SURVEY($id = '')  //class constructor
  {
  if ($id) {

    $this->data = current(SQL::select('tblSurveys','*',sprintf('id=%s',$id)));
    $this->data[preview] = $this->getQuestionList($this->data[question_list]);
    } else {

    $this->data['creator_date'] = date('Y-m-d');
    $this->frostdata = array();  

    }
  }

  function getTemplateData($id)
  {
  $survey = new SURVEY($id);
  $survey->data[survey_items] = $survey->getSurveyItems();
  return $survey->data;
  }
  
  function item_categories()
  {
  global $_LABELS;
  $opt = explode(",", $_LABELS['tblSurveyItems.category.optList']);
  $nam = explode(",", $_LABELS['tblSurveyItems.category.optNames']);
  foreach ($opt as $key => $val) {
    $ic[$val] = $nam[$key];
    }
  return $ic;
  }
  
  function getSurveyItems()
  {
  $data = SQL::getAssoc('tblSurveyItems','*',sprintf("FIND_IN_SET(id,'%s')",$this->data[question_list]));
  $si = explode(',',$this->data[question_list]);
  for ($i=0; $i<count($si); $i++) {
	$survey_items[$i] = $data[$si[$i]]; 
	}
  return $survey_items;	
  }


  function getQuestionList($question_list = '')
  {
  // Used to preview questionaire in the addSurvey form
  $ql = SQL::getAssoc('tblSurveyItems','id,question',sprintf("FIND_IN_SET(id,'%s')",$question_list));
  $question_list = explode(',',$question_list);
  $preView = "<TABLE cellpadding=0 cellspacing=0><TR><TD><OL>";
  for ($i=0; $i<count($question_list); $i++) {
	$preView .= "<LI class=n02>".$ql[$question_list[$i]]; 
	}
  $preView .= "</OL></TD></TR></TABLE>";
  return $preView;	
  }
  
  function getGraphData($survey_id,$client_id,$lang)
  {
  $tables = 'tblSurveyItems AS si,tblSurveys AS s,tblSurveyAnswers AS sa';
  $fields = 'sa.id AS saID, s.question_list, si.id AS siID, sa.date, sa.answer_list, si.answer_num, si.category';
  $params = sprintf('s.id = sa.survey_id AND FIND_IN_SET(si.id, s.question_list) AND sa.client_id = %s AND s.id = %s', $client_id, $survey_id);
  $mrow = SQL::select($tables,$fields,$params);
  $num = count($mrow); 
  for ($i=0; $i<$num; $i++) {
    $row = $mrow[$i];
    $an = explode('|', $row[answer_num]);
	$al = explode(',', $row[answer_list]);
	$ql = explode(',', $row[question_list]);
	$ql = array_flip($ql);
	$n = count($data[$row[category]][$row[saID]]);
	$data[$row[category]][$row[saID]][$n] = $an[$al[$ql[$row[siID]]]-1];
	rsort($an);
	$max[$row[category]][$row[saID]] += $an[0];
	}
  $categories = array_keys($data);

  $row = current(SQL::select('tblLabels',sprintf("opt_list AS cat_lst, opt_%s AS cat_name",$lang),"tbl='tblSurveyItems' AND col='category'"));  
  $cat_name = explode(',', $row[cat_name]);
  $cat_lst = explode(',', $row[cat_lst]);	
  $cat = array_flip($cat_lst);
  $n = count($categories);
  for ($i=0; $i<$n; $i++) {	
    $applications = array_keys($data[$categories[$i]]);
    $m = count($applications);
    if ($m == 1) {
	  $dat[$applications[0]][$cat_name[$cat[$categories[$i]]]] = 100*(array_sum($data[$categories[$i]][$applications[0]])/$max[$categories[$i]][$applications[0]]);
	  } else {
	  for ($j=0; $j<$m; $j++) {
	    $dat[$cat_name[$cat[$categories[$i]]]][$j+1] = 100*(array_sum($data[$categories[$i]][$applications[$j]])/$max[$categories[$i]][$applications[$j]]);
	    }
      }		
    }

  if ($m == 1) {	
    $dat[$applications[0]]['_'] = array_sum($dat[$applications[0]])/$n;
    } else {
    for ($j=0; $j<$m; $j++) {
	  for ($i=0; $i<$n; $i++) {	
	    $dat['_'][$j+1] += (100*(array_sum($data[$categories[$i]][$applications[$j]])/$max[$categories[$i]][$applications[$j]]))/$n;
	    }
	  }		
    }
  return $dat;
	  
  }
  
}
?>