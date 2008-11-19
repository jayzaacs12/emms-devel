<?
require_once 'class/society.php';

if ((!($_GET[child_id]))&&($_GET[parent_id])) {

  $soc = new SOCIETY($_GET[parent_id]);

  $extra['param']['label']['tblSocieties.zone_id']	    = $soc->data['zone'];
  $extra['param']['label']['tblSocieties.advisor_id']	= $soc->data['advisor_username'];

  $fields[] 					= "s.name";
  $extra['name']['header']		= 'tblSocieties.name';
  $extra['name']['pack']		= '%s';
  $extra['name']['align']		= 'left';
    
  $fields[] 					= "CONCAT((LPAD(z.id,3,'0')),'.',s.category,'.',(LPAD(s.id,5,'0'))) AS code";
  $extra['code']['header']		= 'tblSocieties.code';
  $extra['code']['pack']		= '%s';
  $extra['code']['align']		= 'center';

  $fields[] 					= 's.id as activate';
  $extra['activate']['header']	= 'activate';
  $extra['activate']['pack']	= sprintf("<a href='%s?scr_name=BS.SCR.activateBGMember&parent_id=%s&child_id=%s'>%s</a>", $PHP_SELF,$_GET[parent_id],'%s',$_LABELS['activate']);
  $extra['activate']['align']	= 'center';

  $param = sprintf("s.category = 'B'
						AND s.zone_id = '%s'
		    			AND s.advisor_id = '%s'
						AND z.id = s.zone_id
						AND ls.child_id IS NULL",
		    			$soc->data[zone_id],
						$soc->data[advisor_id]);
  $left = 'tblLinkSocieties as ls'; 
  $on = 's.id = ls.child_id AND ls.child_id != ls.parent_id';
  
  $mrow = SQL::select_leftjoin('tblSocieties AS s, tblZones AS z',$fields,$left, $on,$param);
  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);
  require_once 'BS.SCR.pager.inc';
  
  }
   
if (($_GET[child_id])&&($_GET[parent_id])) {

  $values = array(	'id'			=> 'NULL',
  					'type'			=> 'I',
  					'society_id'	=> $_GET[child_id],
  					'parent_id'		=> $_GET[parent_id],
  					'creator_id'	=> WEBPAGE::$userID,
					'date'			=> date('Y-m-d'));
					
  SQL::insert('tblSocietyIOM',$values);

  $values = array(	'parent_id'		=> $_GET[parent_id],
  					'child_id'		=> $_GET[child_id]
  					);
  
  SQL::insert('tblLinkSocieties',$values);
  
  ?>
  <script language=javascript>
  function closeAndUpdate() {
    top.window.opener.document.refreshForm.submit(); 
    top.window.close();
    }
  </script>
  <body onLoad='javascript:closeAndUpdate()'>
  <?
  
  }
  
?>