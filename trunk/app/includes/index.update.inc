<?php
// if auto_cron = 1 este script no se puede correr, ademas de que el script
// que realmente actualiza (index.cron.php) no le hara caso

if ($_CONF['auto_cron'] == '1') { WEBPAGE::redirect('index.php?logout=1'); }

require_once 'HTML/Template/ITX.php';
require_once 'class/TTFButton.php';

$btn = new TTFButton() ;

$tpl = new HTML_Template_ITX('./templates');
$tpl->loadTemplateFile('index.update.tpl');

$tpl->setVariable('html_title_client_name',	strip_tags($_CONF['client_name']));
$tpl->setVariable('client_name',	$_CONF['client_name']);
$tpl->setVariable('db_outdated',		$_LABELS['db_outdated']);
$tpl->setVariable('db_outdated_inf',		sprintf($_LABELS['db_outdated_inf'],WEBPAGE::$runDate));
$tpl->setVariable('db_outdated_wrn',		$_LABELS['db_outdated_wrn']);
$tpl->setVariable('copyright',		$_CONF['copyright']);
$tpl->setVariable('update_button',		$btn->imglink('index.cron.php',$_LABELS['submit']));

$tpl->setVariable('emms_version',		sprintf('%s-%s',$_CONF['version'],$_CONF['update']));

$tpl->show();
?>