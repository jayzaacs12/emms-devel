<?php
require_once "Spreadsheet/Excel/Writer.php"; 

$mrow = WEBPAGE::getCacheData(WEBPAGE::$queryCache); 
$head = array_shift($mrow);

$xls =& new Spreadsheet_Excel_Writer(); 
$xls->send(sprintf('%s.xls',date('YmdHis'))); 
$sheet =& $xls->addWorksheet('Chart'); 

$titleFormat =& $xls->addFormat(); 
$titleFormat->setFontFamily('Helvetica'); 
$titleFormat->setBold(); 
$titleFormat->setSize('13'); 
$titleFormat->setColor('navy'); 
$titleFormat->setBottom(2); 
$titleFormat->setBottomColor('navy'); 
$titleFormat->setAlign('merge'); 

$colHeaderFormat =& $xls->addFormat(); 
$colHeaderFormat->setFontFamily('Helvetica'); 
$colHeaderFormat->setBold(); 
$colHeaderFormat->setSize('10'); 
$colHeaderFormat->setColor('navy'); 


if (count($mrow)) {
  $row = 0;
  $col = 0;
  foreach ($head as $key => $val) {
    $sheet->write($row,$col,str_replace('&nbsp;',' ',strip_tags($val)),$colHeaderFormat); 
    $col++;
    }
  foreach ($mrow as $i => $record) { 
    $col = 0;
    $row++;
    foreach ($record as $key => $val) {
      $sheet->write($row,$col,str_replace('&nbsp;',' ',strip_tags($val))); 
      $col++;
      }  
    } 
  } 

$xls->close(); 

?>