<?php
require_once "Spreadsheet/Excel/Writer.php"; 

$mrow = WEBPAGE::getCacheData(WEBPAGE::$queryCache); 

$extra = array_shift($mrow);
$referrer = $extra['param']['ref']; 
$popup = $extra['param']['popup'];

$xls =& new Spreadsheet_Excel_Writer(); 
$xls->send(sprintf('%s.xls',date('YmdHis'))); 

if (strlen($_LABELS[$referrer])>31) {

$_LABELS[$referrer] = substr($_LABELS[$referrer],0,30);

}

$sheet =& $xls->addWorksheet($_LABELS[$referrer]); 

$titleFormat =& $xls->addFormat(); 
$titleFormat->setFontFamily('Helvetica'); 
$titleFormat->setBold(); 
$titleFormat->setSize('13'); 
$titleFormat->setColor('navy'); 
$titleFormat->setBottom(2); 
$titleFormat->setBottomColor('navy'); 
$titleFormat->setAlign('merge'); 

$sheet->setRow(0,20); 
//$sheet->write(0,0,$_LABELS[$referrer],$titleFormat); 

$colHeaderFormat =& $xls->addFormat(); 
$colHeaderFormat->setFontFamily('Helvetica'); 
$colHeaderFormat->setBold(); 
$colHeaderFormat->setSize('10'); 
$colHeaderFormat->setColor('navy'); 


$row = 1;
if (is_array($extra['param']['label'])) {
  foreach ($extra['param']['label'] as $key=>$val) {
    if ($val != $_LABELS['all']) { 
      $sheet->write($row,0,str_replace('&nbsp;',' ',strip_tags($_LABELS[$key])),$colHeaderFormat); 
      $sheet->write($row,1,$val); 
      $row++;
      }
    }
  } 
  
unset($extra['param']);

if (count($mrow)) {
  $col = 0;
  $row++;
  foreach ($extra as $key => $val) {
    $sheet->write($row,$col,str_replace('&nbsp;',' ',strip_tags($_LABELS[$val['header']])),$colHeaderFormat); 
    $col++;
    }
  foreach ($mrow as $i => $record) { 
    $col = 0;
    $row++;
    foreach ($record as $key => $val) {
      $cell = $_LABELS[sprintf($extra[$key]['pack'],$val)] ? $_LABELS[sprintf($extra[$key]['pack'],$val)] : sprintf($extra[$key]['pack'],$val);
      $sheet->write($row,$col,str_replace('&nbsp;',' ',strip_tags($cell))); 
      $sheet->write(0,$col,$col ? '' : $_LABELS[$referrer],$titleFormat); 
      $col++;
      }  
    } 
  } 

$xls->close(); 

?>