<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/coolbutton.php';
require_once 'class/survey.php';

$survey = new SURVEY($_REQUEST['id']);
$survey->data[preview] = $_REQUEST[question_list] ?  $survey->getQuestionList($_REQUEST[question_list]) : $survey->data[preview];
$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('surveyForm','POST');

$button[id]	    		= "pickQuestions";
$button[href]			= 'javascript:pickQuestions()';
$button[alt]			= $_LABELS['edit'];
$button[onClick]		= '';
$button[ico]			= "edit";
$pickQuestionsbutton  	= New COOLBUTTON($button);

$form->addElement('static', 	'pickQuestions', 	'', COOLBUTTON::getButton($pickQuestionsbutton));
$form->addElement('static',		'preview',	 		$_LABELS['tblSurveys.question_list']);
	
$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('surveyForm',$_LABELS['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('surveyForm',$_LABELS['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);
$form->addElement('hidden',		'question_list',$_REQUEST['question_list']);

$form->addElement('text',		'name',			$_LABELS['tblSurveys.name'],			'class=large');
$form->addElement('textarea',	'description',	$_LABELS['tblSurveys.description']);


//add rules
$form->addRule('name',			sprintf($_LABELS['rangelength'],1,255),'required');
$form->addRule('name',			sprintf($_LABELS['rangelength'],1,255),'rangelength', array(1,255));
$form->addRule('description',	sprintf($_LABELS['rangelength'],1,255),'rangelength', array(1,255));

$form->freeze($survey->frostdata);

if ($form->validate()) { 
  
  $form->removeElement('pickQuestions');

  $form->removeElement('submit');
  $form->removeElement('reset');
  $form->removeElement('cancel');

  $form->freeze();
  
  $values = $form->_submitValues;
  
  $values['date'] = date('Y-m-d');
  $values['creator_id']  = WEBPAGE::$userID;
  
  //First we need to frost survey items included on this survey
  $si = explode(',',$values[question_list]);
  foreach($si as $key => $vals) {
    SQL::insert('tblSurveyItems',array('id'=>$vals,'status'=>'F'),'id');
    }
  
  $lastID = SQL::insert('tblSurveys',$values,'id');
  
  $form->addElement('static', 	'view', 	'', $btn->imglink(sprintf("%s?scr_name=%s&ref=%s&id=%s",$PHP_SELF,'SV.SCR.viewSurvey',WEBPAGE::$scr_name,$lastID),$_LABELS['view']));
  $form->addElement('static', 	'edit', 	'', $btn->imglink(WEBPAGE::appendParam2URL('id',$lastID),$_LABELS['edit']));
  $form->addElement('static', 	'close', 	'', $btn->imglink($PHP_SELF,$_LABELS['close']));
  
  } 
if ($_REQUEST[id]) { $form->removeElement('pickQuestions'); }
$form->setDefaults($survey->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);
    
?>