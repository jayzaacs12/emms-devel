<?php

  $extra['param']['label']['PERM_USER']	    	= $_LABELS['PERM_USER.description'];
  $extra['param']['label']['PERM_ADVISOR']	    = $_LABELS['PERM_ADVISOR.description'];
  $extra['param']['label']['PERM_SUPER']	    = $_LABELS['PERM_SUPER.description'];
  $extra['param']['label']['PERM_FIN_ASSIST']	= $_LABELS['PERM_FIN_ASSIST.description'];
  $extra['param']['label']['PERM_FIN_MAN']	    = $_LABELS['PERM_FIN_MAN.description'];
  $extra['param']['label']['PERM_MANAGER']	    = $_LABELS['PERM_MANAGER.description'];
  $extra['param']['label']['PERM_IT']	    	= $_LABELS['PERM_IT.description'];
  $extra['param']['label']['PERM_ADMIN']	    = $_LABELS['PERM_ADMIN.description'];
  $extra['param']['label']['']	    			= '<br>';

  $fields[] 							= 'link';
  $extra['link']['header']				= 'action';
  $extra['link']['pack']				= '%s';
  $extra['link']['align']				= 'left';

  $fields[] 							= 'if(access_code & 1,"X","") as PERM_USER';
  $extra['PERM_USER']['header']			= 'PERM_USER';
  $extra['PERM_USER']['pack']			= '%s';
  $extra['PERM_USER']['align']			= 'center';

  $fields[] 							= 'if(access_code & 2,"X","") as PERM_ADVISOR';
  $extra['PERM_ADVISOR']['header']		= 'PERM_ADVISOR';
  $extra['PERM_ADVISOR']['pack']		= '%s';
  $extra['PERM_ADVISOR']['align']		= 'center';

  $fields[] 							= 'if(access_code & 4,"X","") as PERM_SUPER';
  $extra['PERM_SUPER']['header']		= 'PERM_SUPER';
  $extra['PERM_SUPER']['pack']			= '%s';
  $extra['PERM_SUPER']['align']			= 'center';

  $fields[] 							= 'if(access_code & 8,"X","") as PERM_FIN_ASSIST';
  $extra['PERM_FIN_ASSIST']['header']	= 'PERM_FIN_ASSIST';
  $extra['PERM_FIN_ASSIST']['pack']		= '%s';
  $extra['PERM_FIN_ASSIST']['align']	= 'center';

  $fields[] 							= 'if(access_code & 16,"X","") as PERM_FIN_MAN';
  $extra['PERM_FIN_MAN']['header']		= 'PERM_FIN_MAN';
  $extra['PERM_FIN_MAN']['pack']		= '%s';
  $extra['PERM_FIN_MAN']['align']		= 'center';

  $fields[] 							= 'if(access_code & 32,"X","") as PERM_MANAGER';
  $extra['PERM_MANAGER']['header']		= 'PERM_MANAGER';
  $extra['PERM_MANAGER']['pack']		= '%s';
  $extra['PERM_MANAGER']['align']		= 'center';

  $fields[] 							= 'if(access_code & 64,"X","") as PERM_IT';
  $extra['PERM_IT']['header']			= 'PERM_IT';
  $extra['PERM_IT']['pack']				= '%s';
  $extra['PERM_IT']['align']			= 'center';

  $fields[] 							= 'if(access_code & 128,"X","") as PERM_ADMIN';
  $extra['PERM_ADMIN']['header']		= 'PERM_ADMIN';
  $extra['PERM_ADMIN']['pack']			= '%s';
  $extra['PERM_ADMIN']['align']			= 'center';

/*
  $fields[] 							= 'id as view';
  $extra['view']['header']				= 'view';
  $extra['view']['pack']				= sprintf("<a href='%s?scr_name=MN.SCR.vieweSCRIPT&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['view']);
  $extra['view']['align']				= 'center';
*/
  $fields[] 							= 'id as edit';
  $extra['edit']['header']				= 'edit';
  $extra['edit']['pack']				= sprintf("<a href='%s?scr_name=MN.SCR.editAccessMask&id=%s'>%s</a>", $PHP_SELF,'%s',$_LABELS['edit']);
  $extra['edit']['align']				= 'center';

  $extra['param']['ref']				= WEBPAGE::$scr_name;
  
//  $mrow = SQL::select('tblMenus',$fields);
  $mrow = WEBPAGE::$dbh->getAll(sprintf("select %s from tblMenus where link!=''",implode(',',$fields)));
  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);
  $items_perpage = 200;
  require_once 'BS.SCR.pager.inc';
  
  
?>
