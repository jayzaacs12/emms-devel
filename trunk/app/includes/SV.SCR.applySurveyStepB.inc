<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITDynamic.php';
require_once 'class/client.php';
require_once 'class/survey.php';

$client = new CLIENT($_REQUEST['client_id']);
$survey = new SURVEY($_REQUEST['id']);
$survey_items = $survey->getSurveyItems();
$tpl->setVariable('client_label', 	sprintf("%s: ",$_LABELS['tblClients.id']));
$tpl->setVariable('client', 		$client->data['name']);
$tpl->setVariable('client_code', 	$client->data['code']);
$tpl->setVariable('name', 			$survey->data['name']);
if ($survey->data['description']) { $tpl->setVariable('description', sprintf("%s<br><br>",$survey->data['description'])); }

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('applyForm','POST');

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'loan_id',		$_REQUEST['loan_id']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);
$form->addElement('hidden',		'client_id',	$_REQUEST['client_id']);
$form->addRule('client_id',		$_LABELS['required'],'required');

$button['submit'] = &HTML_QuickForm::createElement('static', 'submit', 	'', $btn->imglink($PHP_SELF,$_LABELS['cancel']));
$button['reset']  = &HTML_QuickForm::createElement('static', 'reset', 	'', $btn->imgreset('applyForm',$_LABELS['reset']));
$button['cancel'] = &HTML_QuickForm::createElement('static', 'cancel', 	'', $btn->imgsubmit('applyForm',$_LABELS['submit']));

$c = 1;
foreach ($survey_items as $key=>$val) {
  $txt = explode('|',$val['answer_txt']);
  $num[] = explode('|',$val['answer_num']);
  $inx = 1;
  foreach($txt as $x => $y) {
    $option[$key][$x] = $form->createElement('radio',null,null,$y,$inx, 'class=radio');
    $inx++;
	}
  $form->addGroup($option[$key], $key, sprintf("%s. %s",$c++, $val['question']));
  $form->addRule($key,	$_LABELS['selectOption'],'required');
  $tplblock[$key] = 'qf_surveyItem';
  }

$form->freeze($survey->frostdata);

if ($form->validate()) {

  $form->freeze();
  $values = $form->_submitValues;
  $data['survey_id'] 			= $values['id'];
  $data['client_id'] 			= $values['client_id'];
  unset($values['scr_name']);
  unset($values['id']);
  unset($values['client_id']);
  unset($values['loan_id']);
  $data['answer_list']		= implode(',',$values);
  $data['date']				= date('Y-m-d');
  $data['advisor_id']			= WEBPAGE::$userID;

  $totals = explode(',',$data['answer_list']);
  foreach($totals as $x=>$y) {
    $data['totals'] += $num[$x][$y-1];
    }

  $lastID = SQL::insert('tblSurveyAnswers',$data);

  $_REQUEST['loan_id'] ? WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.viewILoan&id=%s&ico=scs&msg=rqloan.scs.survey',$_REQUEST['loan_id'])) : '';

  WEBPAGE::redirect(sprintf('index.php?scr_name=SV.SCR.viewGraph&ref=SV.SCR.applySurveyStepA&survey_id=%s&client_id=%s&ico=scs&msg=scs',$data['survey_id'],$data['client_id']));
/*
  $button = array();
  $button['close'] = &HTML_QuickForm::createElement('static', 'close','', $btn->imglink($PHP_SELF,$_LABELS['close']));
//  $button['edit']  = &HTML_QuickForm::createElement('static', 'edit', '', $btn->imglink(WEBPAGE::appendParam2URL('id',$lastID),$_LABELS['edit']));
  $button['view']  = &HTML_QuickForm::createElement('static', 'view', '', $btn->imglink(sprintf("%s?scr_name=%s&ref=%s&survey_id=%s&client_id=%s",$PHP_SELF,'SV.SCR.viewGraph','SV.SCR.applySurveyStepA',$data[survey_id],$data[client_id],$_LABELS['view']));
*/
  }

$form->addGroup($button, 'button');
$tplblock['button'] = 'qf_buttonBar';

$renderer =& new HTML_QuickForm_Renderer_ITDynamic($tpl);
$renderer->setElementBlock($tplblock);
$form->accept($renderer);

?>