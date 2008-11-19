<?
// check client
if (!$_GET[client_id]) { 
  $location = sprintf("Location: %sindex.php?scr_name=BS.SCR.browseClients&ref=%s",$_CONF[url],WEBPAGE::$scr_name);
  header($location); 
  exit;
  }

// check survey
if ($_CONF[survey_check]) {	
  $tables = 'tblSurveyAnswers';	 
  $fields = 'client_id'; 
  $params = sprintf("survey_id = '%s'
				AND	( CURDATE()<DATE_ADD(date, INTERVAL %s MONTH) )					
				AND	client_id = '%s'",$_CONF[survey_id],$_CONF[survey_margin],$_GET[client_id]);
  $survey_ok = count(SQL::select($tables,$fields,$params));
  } else {
  $survey_ok = true;
  }
if (!($survey_ok)) {
  $location = sprintf("Location: %sindex.php?scr_name=SV.SCR.applySurveyStepB&ref=%s&id=%s&client_id=%s",$_CONF[url],WEBPAGE::$scr_name,$_CONF[survey_id],$_GET[client_id]);
  header($location); 
  exit;
  }

// check business
if ($_CONF[business_check]) {
  $params = sprintf("id = '%s' AND FIND_IN_SET('%s', client_list)",$_GET[business_id],$_GET[client_id]);
  $business_ok = count(SQL::select('tblBusiness','id',$params));
  } else {
  $business_ok = true;
  }
if (!($business_ok)) {
  $location = sprintf("Location: %sindex.php?scr_name=LN.SCR.pickBusiness&ref=%s&client_id=%s",$_CONF[url],WEBPAGE::$scr_name,$_GET[client_id]);
  header($location); 
  exit;
  } 

// check loan modality
if (!$_GET[loan_type_id]) { 
  $location = sprintf("Location: %sindex.php?scr_name=LN.SCR.pickLoanType&ref=%s&client_id=%s&business_id=%s",$_CONF[url],WEBPAGE::$scr_name,$_GET[client_id],$_GET[business_id]);
  header($location); 
  exit;
  }


?>