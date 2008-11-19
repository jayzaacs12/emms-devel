<?php
require_once 'class/program.php';

$program = new PROGRAM();

  $fields[] 					= 'program';
  $extra['program']['header']	= 'tblPrograms.program';
  $extra['program']['pack']		= '%s';
  $extra['program']['align']	= 'left';

  $fields[] 					= 'id as view';
  $extra['view']['header']		= 'view';
  $extra['view']['pack']		= sprintf("<a href='%s?scr_name=BS.SCR.viewProgram&ref=BS.SCR.browsePrograms&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['view']);
  $extra['view']['align']		= 'center';

  $fields[] 					= 'id as edit';
  $extra['edit']['header']		= 'edit';
  $extra['edit']['pack']		= sprintf("<a href='%s?scr_name=BS.SCR.addProgram&ref=BS.SCR.editProgram&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['edit']);
  $extra['edit']['align']		= 'center';

  $extra['param']['ref']		= WEBPAGE::$scr_name;
  
  $mrow = SQL::select('tblPrograms',$fields);
  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);
  require_once 'BS.SCR.pager.inc';
  
?>