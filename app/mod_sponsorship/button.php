<?
require_once '../class/TTFButton.php';
$btn = new TTFButton( $_GET['theme'], $_GET['txt'] ) ;
$btn->show();
?>