<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/coolbutton.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/client.php';
require_once 'class/society.php';
require_once 'class/sql.php';
require_once 'Date.php';

    $master 	= new LOAN_MASTER($_REQUEST['id']);

    $valid_username = WEBPAGE::$dbh->getAll(sprintf("select username from tblUsers u where u.id = %s limit 1;",$master->data['creator_id']));

    $sample_loan_id = WEBPAGE::$dbh->getCol(sprintf("select loan_id from tblLoansMasterDetails lmd where lmd.master_id = %s limit 1;",$master->data['id']));

    $loan = new LOAN($sample_loan_id[0]);



    $form = new HTML_QuickForm('changeDateForm','POST');
    $btn  = new TTFButton(WEBPAGE::$theme);

    $form->addElement('static', 	'submit', 	'', $btn->imgsubmit('changeDateForm',$_LABELS['xp_dates_update']));
    $form->addElement('static', 	'reset', 	'', $btn->imgreset('changeDateForm',$_LABELS['reset']));
    $form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
    $form->addElement('hidden',		'id',			$_REQUEST['id']);
    $form->addElement('hidden',		'user_id',			WEBPAGE::$userID);
    $form->addElement('hidden',		'creator_id',			$master->data['creator_id']);
    // PATCH: 2008.07.21
    $form->addElement('text',		'xp_delivered_date',			$_LABELS['tblLoansMaster.xp_delivered_date'],	'onKeyUp=\'this.className="activeFormElm"\'');
    $form->addElement('text',		'xp_first_payment_date',		$_LABELS['tblLoansMaster.xp_first_payment_date'], 'onKeyUp=\'this.className="activeFormElm"\'');

    //add rules
    $form->addRule('xp_delivered_date',	'yyyy-mm-dd','required');
    $form->addRule('xp_delivered_date',	'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
    $form->addRule('xp_first_payment_date',	'yyyy-mm-dd','required');
    $form->addRule('xp_first_payment_date',	'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');

    $form->registerRule('checkDeliveredDate', 	'callback', 'checkDeliveredDate');
    $form->addRule('xp_delivered_date', 		    $_LABELS['nonValidDate'], 'checkDeliveredDate', true);

    $form->registerRule('checkFirstPaymentDate', 	'callback', 'checkFirstPaymentDate');
    $form->addRule('xp_first_payment_date', 		    $_LABELS['nonValidDate'], 'checkFirstPaymentDate', true);

    // PATCH: 2008.07.21
    if (WEBPAGE::$userID != $master->data['creator_id'] || $master->data['check_status'] == 'R'){
    	$form->removeElement('submit');
    $form->freeze();
    }


    if ($form->validate()) {


      $values = $form->_submitValues;

      //$form->freeze();

  $lastID = SQL::insert('tblLoansMaster',$values,'id');

  }


$form->setDefaults($master->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

function checkDeliveredDate($date) {
  // PATCH: 2008.07.21
  $mdate = new Date();
  $mdate->subtractSeconds(24*60*60);
  $date_delivered = explode('-',$_REQUEST['xp_delivered_date']);
  if (!checkdate($date_delivered[1],$date_delivered[2],$date_delivered[0]))             {return false;}
  if (date('w',date(mktime(0,0,0,$date_delivered[1],$date_delivered[2],$date_delivered[0]))) == 0) { return false; } // verifica que no sea domingo
  if (date('w',date(mktime(0,0,0,$date_delivered[1],$date_delivered[2],$date_delivered[0]))) == 6) { return false; } // verifica que no sea sabado
  if ($date < $mdate->format('%Y-%m-%d')) { return false; }
  return true;
  }

function checkFirstPaymentDate($date) {
  global $loan;
  global $_CONF;
  $date_first_payment = explode('-',$_REQUEST['xp_first_payment_date']);
  if (!checkdate($date_first_payment[1],$date_first_payment[2],$date_first_payment[0])) {return false;}
  if ($date < date('Y-m-d')) return false;
  if ($date <= $_POST['xp_delivered_date']) return false;
  if ($_CONF['flex_1st_pmt_date'] == '1') return true;
  if ($date > $loan->next_payment_date($_POST['xp_delivered_date'])) return false;
  return true;
  // if (($date >= date('Y-m-d')) && ($date > $_POST['xp_delivered_date']) && ($date <= $loan->next_payment_date($_POST['xp_delivered_date'])))  { return true; } else { return false; }
  }

?>
