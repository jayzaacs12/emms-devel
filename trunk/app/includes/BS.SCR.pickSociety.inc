<?
require_once 'class/society.php';

if (($_GET['zone_id'])&&($_GET['advisor_id'])) {

  $soc = new SOCIETY($_GET['parent_id']);

  $extra['param']['label']['tblSocieties.zone_id']	    = $_GET['zone_id'];
  $extra['param']['label']['tblSocieties.advisor_id']	= $_GET['advisor_id'];

  $fields[] 					= "s.name";
  $extra['name']['header']		= 'tblSocieties.name';
  $extra['name']['pack']		= '%s';
  $extra['name']['align']		= 'left';
    
  $fields[] 					= "CONCAT((LPAD(z.id,3,'0')),'.',s.category,'.',(LPAD(s.id,5,'0'))) AS code";
  $extra['code']['header']		= 'tblSocieties.code';
  $extra['code']['pack']		= '%s';
  $extra['code']['align']		= 'center';

  $fields[] 					= "CONCAT('\"',s.id,'\",\"',s.name,'\"') AS pick";
  $extra['pick']['header']		= 'pickSociety';
  $extra['pick']['pack']		= sprintf("<a href='javascript:updateSociety(%s)'>%s</a>",'%s',$_LABELS['pickSociety']);
  $extra['pick']['align']		= 'center';

  $extra['param']['ref']		= WEBPAGE::$scr_name;

  $param = sprintf("s.category != 'BG'
						AND s.zone_id = '%s'
		    			AND s.advisor_id = '%s'
						AND z.id = s.zone_id",
		    			$_GET['zone_id'],
						$_GET['advisor_id']);
  
  $mrow = SQL::select('tblSocieties AS s, tblZones AS z',$fields,$param);
  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);
  
  require_once 'BS.SCR.pager.inc';
  
  }
    
?>