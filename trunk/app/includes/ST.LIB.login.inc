<?
function loginFunction() 
{
  global $message;
  if (!$message) { $message = 'pleaseLogin'; }
  $_LABELS = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_LABELS_FILE,WEBPAGE::$lang));
  $_CONF = WEBPAGE::getCacheData(sprintf(WEBPAGE::_APP_CONF_FILE,WEBPAGE::$lang));
  require './includes/login.inc';
  exit;
}
  
function loginFailedCallback() 
{
  global $_REQUEST;
  global $message;
  $message = 'invalidLogin';
  loginFunction();
}

function loginCallback() 
{
  global $_CONF;
  $_SESSION['_authsession']['data']['module']      = 'main';
  $_SESSION['_authsession']['data']['url']         = current(SQL::getAssoc('tblConfiguration',sprintf('var,%s',WEBPAGE::$lang),'var="url"'));
  $_SESSION['_authsession']['data']['lang']        = WEBPAGE::$lang;
  $_SESSION['_authsession']['data']['theme']       = WEBPAGE::$theme;
  $_SESSION['_authsession']['data']['screenWidth'] = WEBPAGE::$screenWidth; 
  $_SESSION['_authsession']['data']['navtree']     = genNavTree();
  $_SESSION['_authsession']['data']['tabmenu']     = genTabMenu();
  $_SESSION['_authsession']['data']['zoneName']    = current(SQL::getAssoc('tblZones','id,zone',sprintf("id='%s'",$_SESSION['_authsession']['data']['id'])));
  $_GET['scr_name'] = 'TB.SCR.home';
}

function encryptPass() 
{
  $salt = md5($_POST['password']);
  $EncryptPass = crypt($_POST['password'], $salt);
  return $EncryptPass;
}

function genTabMenu() 
{
  $Tab = SQL::select('tblMenus','*',sprintf("type = 'tab' AND ( access_code & %s ) > 0",$_SESSION['_authsession']['data']['access_code']));
  foreach ($Tab as $i=>$row) {
    $tabmenu[$i]['URL'] = sprintf("%s&%s",$row[link],$row[param]);
    $tabmenu[$i]['TAB'] = $row[WEBPAGE::$lang];
    }
  return $tabmenu;
}

function genNavTree() 
{
  $Menu = SQL::select('tblMenus','*',sprintf("parent_id > 0 AND ( access_code & %s ) > 0",$_SESSION['_authsession']['data']['access_code']));
 
  // Para cada elemento del arreglo $Menu, 
  // guarda en arreglo $CAMINO[][] el camino de cada nodo,
  // desde la raiz hasta el nodo.
  if (is_array($Menu)) {
    
	$TDim = count($Menu);
    $TMD = 0;
    for ($i=0;$i<$TDim;$i++) {
	  $N = 0;
	  $CAMINO[$i][$N] = $Menu[$i][id];
	  $PARENT = $Menu[$i][parent_id];
	  while (!($CAMINO[$i][$N] == $PARENT)) {
	    $N++;
	    if ($N >= $TMD) { $TMD = $N + 1; }
	    $CAMINO[$i][$N] = $PARENT;
	    for ($j=0;$j<$TDim;$j++) {
		  if ($Menu[$j][id] == $PARENT) { $PARENT = $Menu[$j][parent_id]; $j = $TDim; }
		  }
	    }
      $CAMINO[$i] = array_reverse($CAMINO[$i]);
      }

    // $CAMINO[][] ya guarda, para cada elemento [nodo] de $MENU,
    // el camino desde la raiz hasta el nodo en cuestion

    // Ordena el arreglo $CAMINO[][]

    for ($i=0;$i<$TDim;$i++) {
	  $CAMINO[$i] = array_pad($CAMINO[$i], $TMD, '');
	  }
    array_multisort($CAMINO);
    for ($i=0;$i<$TDim;$i++) {
	  while (!(is_numeric(end($CAMINO[$i]))))  { array_pop($CAMINO[$i]); }
	  }

    // $CAMINO[][] ya esta ordenado...



    // Guarda en $OrderedMenu una version de $Menu sincronizada con $CAMINO

    for ($i=0;$i<$TDim;$i++) {
	  for($k=0;$k<$TDim;$k++) {
	    if ( $Menu[$k][id] == end($CAMINO[$i]) ) {
	      $OrderedMenu[$i] = $Menu[$k];
		  $k = $TDim;
		  }
	    }
	  }

    // Ya tenemos creado $OrderedMenu

    // Guarda en $NormalCAMINO[][] una version normalizada de $CAMINO[][]
    // Normalizada en cuanto a los index de salida para impresion de menu.

    for ($j=0;$j<$TMD;$j++) {
	  $LastIndex = -1;
	  $PREVIOUS = -1;
	  for ($i=0;$i<$TDim;$i++) {
	    if ( count($CAMINO[$i]) > $j ) {
		  if ( $CAMINO[$i][$j] > $PREVIOUS ) { 
		    $LastIndex++; 
		    $PREVIOUS = $CAMINO[$i][$j];
		    }
		  $NormalCAMINO[$i][$j] = $LastIndex;
		  } else {
		  $LastIndex = -1;
		  $PREVIOUS = -1;
		  }
	    }
	  }

    // En $NormalCAMINO[][] ya tenemos una version normalizada de $CAMINO[][]

    for ($i=0;$i<$TDim;$i++) {
      $NDepth = count($NormalCAMINO[$i]);
	  for ($j=0;$j<$NDepth;$j++) {
	    $menulynx .= '.'; // TreeMene does not need this -> $NormalCAMINO[$i][$j];
	    }
	  $menulynx .= sprintf("|%s", $OrderedMenu[$i][WEBPAGE::$lang]);
	  if (!($OrderedMenu[$i][link] == '')) { 
	    $menulynx .= sprintf("|%sindex.php?scr_name=%s",$_CONF[url],$OrderedMenu[$i][link]);
	    } else {
	    $menulynx .= sprintf("|");
   	    }
	  $menulynx .= sprintf("||<br>\n");
	  }
    }
    
  $_SESSION['OrderedMenu'] = $OrderedMenu;
    
  require_once '../phplayersmenu-3.2.0/lib/PHPLIB.php';
  require_once '../phplayersmenu-3.2.0/lib/layersmenu-common.inc.php';
  require_once '../phplayersmenu-3.2.0/lib/treemenu.inc.php';

  $mid = new TreeMenu();

  $mid->setMenuStructureString($menulynx);
  $mid->setIconsize(16, 16);
  $mid->parseStructureForMenu('treemenu1');
  $mid->newTreeMenu('treemenu1');
  return $mid->getTreeMenu('treemenu1');

}
?>