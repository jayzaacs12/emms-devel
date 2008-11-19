<?
require_once 'class/TTFButton.php';
$btn = new TTFButton( isset($_GET['theme']) ?  $_GET['theme'] : '', $_GET['txt'] ) ;
$btn->show();
?>