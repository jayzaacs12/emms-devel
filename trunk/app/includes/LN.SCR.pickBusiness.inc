<?php
if ($_REQUEST['loan_id'] && $_REQUEST['business_id']) {
  WEBPAGE::$dbh->query(sprintf("update tblLoans as l set business_id = '%s' where l.id = '%s' AND find_in_set(l.client_id, (select b.client_list from tblBusiness as b where b.id = '%s') )", $_REQUEST['business_id'], $_REQUEST['loan_id'], $_REQUEST['business_id']));
  WEBPAGE::redirect(sprintf("index.php?scr_name=LN.SCR.viewILoan&id=%s&ico=scs&msg=scs",$_REQUEST['loan_id']));
  }

require_once 'class/client.php';
require_once 'class/business_type.php';
require_once 'class/business.php';

$client = new CLIENT($_REQUEST['client_id']);

if (!count($client->data['business'])) {
  WEBPAGE::redirect(sprintf("index.php?scr_name=BS.SCR.addBusiness&ref=LN.SCR.requestLoan&owner_id=%s&loan_id=%s&ico=inf&msg=rqloan.inf.business",$_REQUEST['client_id'],$_REQUEST['loan_id']));
  }

$tpl->setVariable('client_label', $_LABELS['tblClients.id']);
$tpl->setVariable('client', $client->data['name']);
$tpl->setVariable('client_code', $client->data['code']);
$tpl->setVariable('notListedAddNew', sprintf("<a href='index.php?scr_name=BS.SCR.addBusiness&ref=LN.SCR.requestLoan&owner_id=%s&loan_id=%s'>%s</a>",$_REQUEST['client_id'],$_REQUEST['loan_id'],$_LABELS['notListedAddNew']));

$tpl->addBlockfile('pickBusinessChart','pickBusinessChart',sprintf("%s.%s",'ST.chart','tpl'));
$tpl->setCurrentBlock("pickBusinessChart") ;

$header = array('#',$_LABELS['tblBusiness.name'],$_LABELS['tblBusinessTypes.type'],$_LABELS['select']);
foreach ($header as $key => $val) {
  $tpl->setCurrentBlock("header");
  $tpl->setVariable("column_name", $val) ;
  $tpl->parseCurrentBlock("header");
  }

$c = 1;
foreach ($client->data['business'] as $key => $val) { 

  $buz = new BUSINESS($key);
  
  $tpl->setCurrentBlock("row");
  $tpl->setVariable("align", 'right') ;
  $tpl->setVariable("item", $c++) ;
  $tpl->parseCurrentBlock("row");

  $tpl->setCurrentBlock("row");
  $tpl->setVariable("align", 'left') ;
  $tpl->setVariable("item", $val) ;
  $tpl->parseCurrentBlock("row");

  $tpl->setCurrentBlock("row");
  $tpl->setVariable("align", 'left') ;
  $tpl->setVariable("item", $buz->data['type']) ;
  $tpl->parseCurrentBlock("row");

  $tpl->setCurrentBlock("row");
  $tpl->setVariable("align", 'center') ;
  $tpl->setVariable("item", sprintf("<a href='index.php?scr_name=LN.SCR.pickBusiness&loan_id=%s&business_id=%s'>%s</a>", $_REQUEST['loan_id'],$key,$_LABELS['select'])) ;
  $tpl->parseCurrentBlock("row");
    
  $tpl->setCurrentBlock("results");
  $tpl->parseCurrentBlock("results");
  
  }
  
?>