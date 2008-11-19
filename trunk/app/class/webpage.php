<?php
error_reporting(E_ERROR);
class WEBPAGE
{
  /* WEBPAGE parameters */

  const _REQUIRED_FIELD 		= "{label}<span class=required size='1'>*</span>";
  const _FIELD_ERROR    		= "<span class=error>{error}</span><br />{html}";
  const _DEFAULT_LANG 			= "esp";
  const _THEMES_PATH			= "./themes/";
  const _DEFAULT_THEME 			= "blue";
  const _APP_LABELS_FILE		= "./tmp/labels.%s.data";
  const _APP_CONF_FILE			= "./tmp/conf.%s.data";
  const _APP_RUN_MODE_FILE		= "./tmp/run.mode.data";
  const _APP_MODE_LOG_FILE		= "./tmp/run.mode.log.data";
  const _APP_QUERY_CACHE		= "./tmp/query.cache.%s";
  const _DEFAULT_FRONTPAGE		= "TB.SCR.home";

  const _IMG_USER			= "./img/users/%s.jpg";
  const _IMG_CLIENT			= "./img/clients/%s.jpg";

  const _PAGER_MODE    			= 'Jumping';//'Sliding';//Jumping
  const _PAGER_PERPAGE    		= 10;
  const _PAGER_DELTA    		= 10;
  const _PAGER_EXPANDED			= false;
  const _PAGER_LINKCLASS		= 'pager';
  const _PAGER_ALTPREV			= 'previous';
  const _PAGER_ALTNEXT			= 'next';
  const _PAGER_ALTPAGE			= 'gotopage';
  const _PAGER_IMG			= "<img src='%s%s/icons/%s' alt='%s'>";
  const _PAGER_PREVIMG			= 'previous.png';
  const _PAGER_NEXTIMG			= 'next.png';
  const _PAGER_FIRSTIMG			= 'first.png';
  const _PAGER_LASTIMG			= 'last.png';
  const _PAGER_SEPARATOR		= "&nbsp;&nbsp;&nbsp;&nbsp;";
  const _PAGER_FIRSTPAGETEXT	        = "firstPage";
  const _PAGER_LASTPAGETEXT		= "lastPage";
  const _PAGER_CURPAGELINKCLASS	        = 'pager';
  const _PAGER_EXTRA_SCR_NAME           = 'BS.SCR.pager';

  const _HOTLINK			= "<a class=record href='index.php?scr_name=%s&%s=%s'>%s</a>";
//  const _HOTICON			= "<a href='index.php?scr_name=%s&%s=%s'><img alt='%s' src='./themes/%s/icons/%s.png'></a>";

  const _RUN_MODE_OUTDATED    = 'outdated';
  const _RUN_MODE_NORMAL      = 'normal';
  const _RUN_MODE_MAINTENANCE = 'maintenance';
  const _RUN_MODE_CRASHED     = 'crashed';

  static $url;
  static $runMode;
  static $runDate;
  static $scr_name;
  static $referrer;
  static $tabmenu;
  static $navtree;
  static $lang;
  static $screenWidth;
  static $theme;
  static $themePath;
  static $userID;
  static $userAccessCode;
  static $userName;
  static $userZone;
  static $zoneName;
  static $queryCache;
  static $auth_options = array(
  			'dsn' => 'mysql://root@localhost/emms210',
  			'table' => 'tblUsers',
  			'usernamecol' => 'username',
  			'passwordcol' => 'password',
  			'cryptType' => 'encryptPass',
  			'db_fields' => '*'
  			);

  static $dbh;
  static $btn;

  function WEBPAGE()  //class constructor
  {
  require_once 'DB.php';
  require_once 'class/TTFButton.php';

  self::$dbh = DB::connect(self::$auth_options['dsn']);
  self::$dbh->setFetchMode(DB_FETCHMODE_ASSOC);

//  self::$lang = self::_DEFAULT_LANG;
//  if ($lang = $_POST['lang']) self::$lang = $lang;
//  if ($lang = $_GET['lang'])  self::$lang = $lang;

  self::$lang = isset($_POST['lang']) ? $_POST['lang'] : self::_DEFAULT_LANG;
  self::$lang = isset($_GET['lang'])  ? $_GET['lang']  : self::$lang;


//  self::$scr_name = self::_DEFAULT_FRONTPAGE;
//  if ($scr_name = $_POST['scr_name']) self::$scr_name = $scr_name;
//  if ($scr_name = $_GET['scr_name'])  self::$scr_name = $scr_name;

  self::$scr_name = isset($_POST['scr_name']) ? $_POST['scr_name'] : self::_DEFAULT_FRONTPAGE;
  self::$scr_name = isset($_GET['scr_name'])  ? $_GET['scr_name']  : self::$scr_name;



//  if (!(self::$referrer = $_REQUEST['ref']))	self::$referrer = $_CONF[ref][self::$scr_name];
  self::$referrer = isset($_REQUEST['ref']) ? $_REQUEST['ref'] : '';

  if (isset($_POST['screenWidth'])) { self::$screenWidth = 0.95*$_POST['screenWidth']; }
  self::$theme = self::_DEFAULT_THEME ;
  self::$themePath = sprintf("%s%s/",self::_THEMES_PATH,self::$theme) ;

  self::$btn = new TTFButton(self::$theme);

  self::checkcachefiles();

//  self::$runMode = array_pop(WEBPAGE::$dbh->getAssoc(sprintf("select id,mode from tblDataLog where date = '%s' order by id",date('Y-m-d'))));
  $data = current(self::$dbh->getAssoc("select id,mode,date from tblDataLog order by id desc limit 1"));
  self::$runDate = $data['date'];
  self::$runMode = $data['mode'];
  if (self::$runDate != date('Y-m-d')) {
    if (self::$runMode != self::_RUN_MODE_NORMAL)  { self::$runMode = self::_RUN_MODE_CRASHED;  }
    if (self::$runMode != self::_RUN_MODE_CRASHED) { self::$runMode = self::_RUN_MODE_OUTDATED; }
    }

  self::$url = current(WEBPAGE::$dbh->getAssoc(sprintf("select var,%s from tblConfiguration where var='url'",self::$lang)));
  }

  static function START()
  {
  $start = new WEBPAGE();
  }

  static function LOAD_SESSION()
  {
  self::$lang			= $_SESSION['_authsession']['data']['lang'];
  self::$screenWidth 		= $_SESSION['_authsession']['data']['screenWidth'];
  self::$userID 		= $_SESSION['_authsession']['data']['id'];
  self::$userAccessCode	        = $_SESSION['_authsession']['data']['access_code'];
  self::$userName		= $_SESSION['_authsession']['data']['first'];
  self::$userZone		= $_SESSION['_authsession']['data']['zone_id'];
  self::$zoneName		= $_SESSION['_authsession']['data']['zoneName'];
  self::$tabmenu		= $_SESSION['_authsession']['data']['tabmenu'];
  self::$navtree		= $_SESSION['_authsession']['data']['navtree'];
  self::$queryCache		= sprintf(self::_APP_QUERY_CACHE,str_pad(self::$userID,3,'0',STR_PAD_LEFT));
  }

  static function getPagerOptions($referrer,$data,$custom_PAGER_PERPAGE = 0)
  {
// 'firstPageText'			=> $_LABELS[self::_PAGER_FIRSTPAGETEXT],
  global $_LABELS;
  return array(	'mode'       			=> self::_PAGER_MODE,
				'perPage'    			=> $custom_PAGER_PERPAGE ? $custom_PAGER_PERPAGE : self::_PAGER_PERPAGE,
				'delta'      			=> self::_PAGER_DELTA,
				'linkClass'	 			=> self::_PAGER_LINKCLASS,
				'altPrev'	 			=> $_LABELS[self::_PAGER_ALTPREV],
				'altNext'	 			=> $_LABELS[self::_PAGER_ALTNEXT],
				'altPage'	 			=> $_LABELS[self::_PAGER_ALTPAGE],
				'spacesBeforeSeparator' => 0,
				'spacesAfterSeparator'  => 0,
				'separator'	 			=> self::_PAGER_SEPARATOR,
				'prevImg'				=> sprintf(self::_PAGER_IMG,self::_THEMES_PATH,self::$theme,self::_PAGER_PREVIMG,$_LABELS[self::_PAGER_ALTPREV]),
				'nextImg'				=> sprintf(self::_PAGER_IMG,self::_THEMES_PATH,self::$theme,self::_PAGER_NEXTIMG,$_LABELS[self::_PAGER_ALTNEXT]),
				'firstPageText'			=> sprintf(self::_PAGER_IMG,self::_THEMES_PATH,self::$theme,self::_PAGER_FIRSTIMG,$_LABELS[self::_PAGER_FIRSTPAGETEXT]),
				'lastPageText'			=> sprintf(self::_PAGER_IMG,self::_THEMES_PATH,self::$theme,self::_PAGER_LASTIMG,$_LABELS[self::_PAGER_LASTPAGETEXT]),
				'curPageLinkClassName'	=> self::_PAGER_CURPAGELINKCLASS,
				'firstPagePre'			=> '',
				'firstPagePost'			=> '',
				'lastPagePre'			=> '',
				'lastPagePost'			=> '',
				'itemData'				=> $data,
				'extraVars' 		=> array( 	'scr_name'  => self::_PAGER_EXTRA_SCR_NAME,
												'ref'		=> $referrer)
			  );
  }

  static function printerlink()
  {
  global $_LABELS;
  return sprintf("<a href='javascript:print()'>%s</a>",$_LABELS['printSheet']);
  }

  static function hotlink($scr_name,$par,$value,$text)
  {
  return sprintf(self::_HOTLINK,$scr_name,$par,$value,$text);
  }

  static function hoticon($scr_name,$par,$value,$text,$icon)
  {
  return sprintf(self::_HOTICON,$scr_name,$par,$value,$text,self::$theme,$icon);
  }

  static function hotlist($scr_name,$par,$list)
  {
  $hotlist = array();
  foreach ($list as $key => $value) {
    $hotlist[$key] = sprintf(self::_HOTLINK,$scr_name,$par,$key,$value);
    }
  return $hotlist;
  }

  static function appendParam2URL($par,$val)
  {
  return sprintf("%s?scr_name=%s&%s=%s",$PHP_SELF,self::$scr_name,$par,$val);
  }

  static function getCurrentPath()
  {
  global $_LOCATION;
  global $_CONF;
  self::$referrer = self::$referrer ? self::$referrer : $_CONF['ref'][self::$scr_name];
  for ($i=0;$i<count($_SESSION['OrderedMenu']);$i++) {
    if ($_SESSION['OrderedMenu'][$i]['link'] == self::$referrer) {
      $_LOCATION[0]['id'] = $_SESSION['OrderedMenu'][$i]['id'];
      $_LOCATION[0]['parent_id'] = $_SESSION['OrderedMenu'][$i]['parent_id'];
      $_LOCATION[0]['link'] = $_SESSION['OrderedMenu'][$i]['link'];
      $_LOCATION[0]['label'] = $_SESSION['OrderedMenu'][$i][self::$lang];
      break;
      }
    }
  while (!($stop)) {
    $n = count($_LOCATION);
    $_LOCATION = self::getPARENT($_LOCATION[$n-1]['parent_id']);
    if ($_LOCATION[$n]['parent_id'] == $_LOCATION[$n]['id']) { $stop = true; }
    }
  for ($i=$n;$i>=0;$i--) {
    $path[] = $_LOCATION[$i]['label'];
    }
  return $path;
  }

  static function getPARENT($SCR_ID)
  {
  global $_LOCATION;
  $n = count($_LOCATION);
  for ($i=0;$i<count($_SESSION['OrderedMenu']);$i++) {
    if ($_SESSION['OrderedMenu'][$i]['id'] == $SCR_ID) {
	  $_LOCATION[$n]['id']=$SCR_ID;
      $_LOCATION[$n]['parent_id']=$_SESSION['OrderedMenu'][$i]['parent_id'];
      $_LOCATION[$n]['link']=$_SESSION['OrderedMenu'][$i]['link'];
      $_LOCATION[$n]['label']=$_SESSION['OrderedMenu'][$i][self::$lang];
      return $_LOCATION;
      }
    }
  }

  static function getCacheData ($cache)
  {
  if (file_exists($cache)) {
    // read data file
    $file = fopen($cache, 'r');
    if ($file) {
      $data = fread($file,filesize($cache));
      fclose($file);
      }
    return unserialize(gzuncompress($data));
    }
  }

  static function checkcachefiles()
  {

  if (!(file_exists(sprintf(self::_APP_LABELS_FILE,self::$lang)))) {

    $mrow = SQL::select('tblLabels');
    foreach($mrow as $key=>$row) {
      $label = $row['tbl'].'.'.$row['col'];
      $_LABELS[$label] = $row[self::$lang];
      if ($row['opt'] == '1') {
        $label_optList = $label.'.optList';
        $label_optNames = $label.'.optNames';
	$_LABELS[$label_optList]= $row['opt_list'];
	$_LABELS[$label_optNames]= $row['opt_'.self::$lang];
	$OPTlist = explode(',', $row['opt_list']);
	$OPTnames = explode(',', $_LABELS[$label_optNames]);
	$n = count($OPTlist);
	for($j=0; $j<$n; $j++) {
	  $label_OPT = $label.'.'.$OPTlist[$j];
	  $_LABELS[$label_OPT]= $OPTnames[$j];
	  }
        }
      }

    $mrow = SQL::select('tblLocalizedTexts',sprintf('%s, %s', 'msg_id', self::$lang));
    foreach($mrow as $key=>$row) {
      $_LABELS[$row['msg_id']] = $row[self::$lang];
      }

    self::makecachefile($_LABELS,self::_APP_LABELS_FILE);
    }

  if (!(file_exists(sprintf(self::_APP_CONF_FILE,self::$lang)))) {
    $mrow = SQL::select('tblConfiguration');
    foreach($mrow as $key=>$row) {
      $conf = $row['var'];
      $_CONF[$conf] = $row[self::$lang];
      }
    $mrow = SQL::select('tblMenus','*','link != ""');
    foreach($mrow as $key=>$row) {
      $conf = $row['link'];
      $_CONF[$conf] = $row['access_code'];
      $_CONF[ref][$conf] = $row['ref'];
      }
    self::makecachefile($_CONF,self::_APP_CONF_FILE);
    }
  }

  static function makecachefile ($cache_data,$cache_file) {
  // write application data to file

  if (is_array($cache_data)) {
    $data = gzcompress(serialize($cache_data),9);
    $file = fopen(sprintf($cache_file,self::$lang), 'w');
    if ($file) {
      fwrite($file, $data);
      fclose($file);
      }
    }
  }

  static function printmessage($ico,$msg)
  {
  $tpl = new HTML_Template_ITX('./templates');
  $tpl->loadTemplateFile('ST.SCR.message.tpl');

  $tpl->setCurrentBlock("message") ;
  $tpl->setVariable('ico',	sprintf('%s/icons/%s.png',WEBPAGE::$themePath, $ico));
  $tpl->setVariable('msg',	$msg);
  $tpl->parseCurrentBlock("message") ;

  return $tpl->get();
  }

  static function printchart($data,$head = array(),$pack = array())
  {
  global $_LABELS;

  array_unshift($data,$head);
  self::makecachefile($data,self::$queryCache);
  array_shift($data);

  $tpl = new HTML_Template_ITX('./templates');
  $tpl->loadTemplateFile('ST.chart.tpl');

  foreach($head as $key=>$val) {
    $tpl->setCurrentBlock("header") ;
    $tpl->setVariable('column_name', $val);
    $tpl->parseCurrentBlock("header") ;
    }

  foreach($data as $key=>$row) {
    foreach($row as $col => $val) {
      $tpl->setCurrentBlock("row") ;
      $tpl->setVariable('align', 'right');
      $tpl->setVariable('item', $pack[$col] ? $_LABELS[sprintf($pack[$col],$val)] : $val);
      $tpl->parseCurrentBlock("row") ;
      }
    $tpl->setCurrentBlock("results") ;
    $tpl->parseCurrentBlock("results") ;
    }
  $tpl->setCurrentBlock("chart") ;
  $tpl->setVariable('xls_download',$_LABELS['RP.SCR.ChartCacheToXLS']);
  $tpl->parseCurrentBlock("chart") ;
  return $tpl->get();
  }

  static function printchart_ii($mdata,$head,$totals,$styles,$flags)
  {
  /*
  example for each param
  $mdata = array($data_0,$data_1);
    with:
     $data_0[row_0][col_0]='val_0_00';
     $data_0[row_0][col_1]='val_0_01';
     $data_1[row_0][col_0]='val_1_00';
     $data_1[row_0][col_1]='val_1_01';
  $head = array('col_0'=>'col_0_name', 'col_1'=>'col_1_name');
  $totals = array('totals'=>true,'subtotals'=>true,'cols'=>array('col_1'));
       // only calculate totals for 'col_1';
       // also show subtotals for each data set
       // and grand total row
  $styles = array(  'header'    => array('cell'=>'header_cell_class','row'=>''),
                    'subtotals' => array('cell'=>'subtotals_cell_class','row'=>'subtotals_row_class'),
                    'totals'    => array('cell'=>'totals_cell_class','row'=>'totals_row_class'),
                    'data_0'     => array('cell'=>'data_0_cells_class','row'=>'data_0_row_class'),
                    'data_1'     => array('cell'=>'data_1_cells_class','row'=>'data_1_row_class'));
  $flags = array('cache','legend');
  $flags['cache'] = true; // Save results in hd cache - makes for excel download.
  $flags['legend'] = true; // Prints legend.
  */

  global $_LABELS;

  if (!(is_array($mdata)))          { return $_LABELS['noData']; }
  foreach ($mdata as $inx => $data) {
    if (!count($data)) { unset($mdata[$inx]); };
    }
  if (!(count($mdata)))             { return $_LABELS['noData']; }

  //if style info is empty, then create default styles
  if (!(count($styles))) {
      $styles['header']['cell']    = 'header';
      $styles['header']['row']     = '';
      $styles['subtotals']['cell'] = 'subtotals';
      $styles['subtotals']['row']  = 'subtotalsOff';
      $styles['totals']['cell']    = 'totals';
      $styles['totals']['row']     = 'totalsOff';
    foreach ($mdata as $inx=>$data) {
      if(count($data)) {
        $row_style  = ($row_style  == 'rowOff_ii') ? 'rowOff_ii_alt' : 'rowOff_ii';
        $styles[$inx]['cell'] = 'activeChart';
        $styles[$inx]['row']  = $row_style;
        }
      }
    }

  // to make sure we create a cache file to open in excel upon request
  $c = 0;
  foreach($mdata as $inx=>$data) {
    foreach($data as $k=>$v) {
      $cdata[$c] = $v;
      $c++;
      if (is_array($totals['cols'])) {
        foreach($v as $col=>$val) {
          if (in_array($col,$totals['cols'])) {
            $subtotals[$inx][$col] += $val;
            $ctotals[$col]         += $val;
            }  else {
            $subtotals[$inx][$col]  = '-';
            $ctotals[$col]          = '-';
            }
          }
        }
      }
    }
  if (is_array($ctotals)) { $cdata[$c] = $ctotals; }

  array_unshift($cdata,$head);
  $flags['cache'] ? self::makecachefile($cdata,self::$queryCache) : '';
  array_shift($cdata);

  //print html chart

  $tpl = new HTML_Template_ITX('./templates');
  $tpl->loadTemplateFile('ST.chart_ii.tpl');

  //print header row
  $tpl->setCurrentBlock("header") ;
  $tpl->setVariable('column_name', '#');
  $tpl->setVariable('header_class', $styles['header']['cell']);
  $tpl->parseCurrentBlock("header") ;
  foreach($head as $key=>$val) {
    $tpl->setCurrentBlock("header") ;
    $tpl->setVariable('column_name', $val);
    $tpl->setVariable('header_class', $styles['header']['cell']);
    $tpl->parseCurrentBlock("header") ;
  }

  //print each data set, row by row
  //including subtotal for each set
  $c = 1;
  foreach($mdata as $inx=>$data) {
    $row_style = $styles[$inx]['row'];
    //print regular rows
    foreach($data as $key=>$row) {
      $tpl->setCurrentBlock("row") ;
      $tpl->setVariable('cell_class', $styles[$inx]['cell']);
      $tpl->setVariable('item', sprintf('%s.&nbsp;',$c++));
      $tpl->parseCurrentBlock("row") ;
      foreach($row as $col => $val) {
        $tpl->setCurrentBlock("row") ;
        $tpl->setVariable('cell_class', $styles[$inx]['cell']);
        $tpl->setVariable('item', $val);
        $tpl->parseCurrentBlock("row") ;
        }
      $tpl->setCurrentBlock("results") ;
      $tpl->setVariable('row_class', $row_style);
      $tpl->parseCurrentBlock("results") ;
     }
    //print subtotals
    if (($totals['subtotals'])&&(is_array($subtotals[$inx]))) {
      $tpl->setCurrentBlock("row") ;
      $tpl->setVariable('cell_class', $styles['subtotals']['cell']);
      $tpl->setVariable('item', $_LABELS['subtotal']);
      $tpl->parseCurrentBlock("row") ;
      foreach($subtotals[$inx] as $col => $val) {
        $tpl->setCurrentBlock("row") ;
        $tpl->setVariable('cell_class', $styles['subtotals']['cell']);
        $tpl->setVariable('item', is_numeric($val) ? round($val,2) : $val);
        $tpl->parseCurrentBlock("row") ;
        }
      $tpl->setCurrentBlock("results") ;
      $tpl->setVariable('row_class', $styles['subtotals']['row']);
      $tpl->parseCurrentBlock("results") ;
      }
    }

  //print totals row
  if (($totals['totals'])&&(is_array($totals['cols']))) {
    $tpl->setCurrentBlock("row") ;
    $tpl->setVariable('cell_class', $styles['totals']['cell']);
    $tpl->setVariable('item', $_LABELS['total']);
    $tpl->parseCurrentBlock("row") ;
    foreach($ctotals as $col => $val) {
      $tpl->setCurrentBlock("row") ;
      $tpl->setVariable('cell_class', $styles['totals']['cell']);
      $tpl->setVariable('item', is_numeric($val) ? round($val,2) : $val);
      $tpl->parseCurrentBlock("row") ;
      }
    $tpl->setCurrentBlock("results") ;
    $tpl->setVariable('row_class', $styles['totals']['row']);
    $tpl->parseCurrentBlock("results") ;
  }

  //print legend
  if ($flags['legend']) {
    foreach($mdata as $inx=>$val) {
      if ($styles[$inx]['row']) {
        $tpl->setCurrentBlock("legend") ;
        $tpl->setVariable('style', $styles[$inx]['row']);
        $tpl->setVariable('legend_text', $_LABELS[$inx] ? $_LABELS[$inx] : $inx);
        $tpl->parseCurrentBlock("legend") ;
        }
      }
    }

  //finalize and print excel link
  $tpl->setCurrentBlock("chart") ;
  $cache ? $tpl->setVariable('xls_download',$_LABELS['RP.SCR.ChartCacheToXLS']) : '';
  $tpl->parseCurrentBlock("chart") ;
  return $tpl->get();
  }

  static function verbose_date_format($date)
  {
  global $_CONF;
  global $_LABELS;

  $fdate      = $_CONF['verbose_date_format'];
  $darray     = explode('-',$date);

  $date_unix  = mktime(0,0,0,$darray[1],$darray[2],$darray[0]);
  $month      = date('F',$date_unix);
  $weekday    = date('l',$date_unix);


  $fdate	  = str_replace('%M', $_LABELS[$month],     $fdate);
  $fdate	  = str_replace('%W', $_LABELS[$weekday],   $fdate);
  $fdate	  = str_replace('%D', $darray[2], $fdate);

  return str_replace('%Y', $darray[0], $fdate);
  }

  function redirect($url)
  {
  global $_CONF;
  $location = sprintf("Location: %s%s",$_CONF['url'],$url);
  header($location);
  exit;
  }

}