<?
$tpl->setVariable('javascript',          "<SCRIPT type='text/javascript' src='CS.functions.js'></SCRIPT>");
$tpl->setVariable('welcome',             sprintf($welcome, $_SESSION['_authsession']['data']['first']));
$tpl->setVariable('shortcuts',           $_LABELS['quick_jumps']);
$tpl->setVariable('editProfile',         $_LABELS['SP.SCR.editProfile']);
$tpl->setVariable('changePassword',      $_LABELS['SP.SCR.changePassword']);
$tpl->setVariable('financialSummary',    $_LABELS['SP.SCR.financialSummary']);
$tpl->setVariable('donationsHistory',    $_LABELS['SP.SCR.donationsHistory']);
$tpl->setVariable('portfolioSummary',    $_LABELS['SP.SCR.portfolioSummary']);
$tpl->setVariable('clientSummary',       $_LABELS['SP.SCR.clientSummary']);
$tpl->setVariable('loanDefaults',        $_LABELS['tblLoanTypes.id']);
$tpl->setVariable('disbursmentHistory',  $_LABELS['SP.SCR.disbursmentHistory']);
$tpl->setVariable('businessSupport',     $_LABELS['SP.SCR.businessSupport']);
$tpl->setVariable('browseLoans',         $_LABELS['SP.SCR.browseLoans']);
$tpl->setVariable('portfolioAtRisk',     $_LABELS['SP.SCR.portfolioAtRisk']);
$tpl->setVariable('writeOffHistory',     $_LABELS['SP.SCR.writeOffHistory']);
$tpl->setVariable('browseGroups',        $_LABELS['SP.SCR.browseGroups']);
$tpl->setVariable('browseClients',       $_LABELS['SP.SCR.browseClients']);
$tpl->setVariable('accessLog',           $_LABELS['SP.SCR.accessLog']);
$tpl->setVariable('logout',              $_LABELS['logout']);
$tpl->setVariable('mktime',              mktime());
$tpl->setVariable('lang',                WEBPAGE::$lang);
?>


