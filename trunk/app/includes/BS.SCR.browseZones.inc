<?php
require_once 'class/zone.php';

$zone = new ZONE();

  $fields[] 						= 'CONCAT(short_name, " - ", zone) as zone';
  $extra['zone']['header']			= 'tblZones.zone';
  $extra['zone']['pack']			= '%s';
  $extra['zone']['align']			= 'left';

  $fields[] 						= 'id as view';
  $extra['view']['header']			= 'view';
  $extra['view']['pack']			= sprintf("<a href='%s?scr_name=BS.SCR.viewZone&ref=BS.SCR.browseZones&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['view']);
  $extra['view']['align']			= 'center';

  $fields[] 						= 'id as edit';
  $extra['edit']['header']			= 'edit';
  $extra['edit']['pack']			= sprintf("<a href='%s?scr_name=BS.SCR.addZone&ref=BS.SCR.editZone&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['edit']);
  $extra['edit']['align']			= 'center';

  $extra['param']['ref']			= WEBPAGE::$scr_name;
  
  $mrow = SQL::select('tblZones',$fields,'parent_id = 0');
  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);
  require_once 'BS.SCR.pager.inc';
  
?>