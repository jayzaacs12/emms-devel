<?php
$tpl->setVariable('riskPortfolio', 			          $_LABELS['riskPortfolio']);
$tpl->setVariable('riskByAdvisor', 			          $_LABELS['RP.SCR.riskByAdvisor']);
$tpl->setVariable('riskByBranch',  			          $_LABELS['RP.SCR.riskByBranch']);
$tpl->setVariable('riskByProgram', 			          $_LABELS['RP.SCR.riskByProgram']);

$tpl->setVariable('clientPortfolio', 			      $_LABELS['clientPortfolio']);
$tpl->setVariable('clientByAdvisor', 			      $_LABELS['RP.SCR.clientByAdvisor']);
$tpl->setVariable('clientByBranch',  			      $_LABELS['RP.SCR.clientByBranch']);
$tpl->setVariable('clientByProgram', 			      $_LABELS['RP.SCR.clientByProgram']);

$tpl->setVariable('loanPortfolio', 			          $_LABELS['loanPortfolio']);
$tpl->setVariable('loanByAdvisor', 			          $_LABELS['RP.SCR.loanByAdvisor']);
$tpl->setVariable('loanByBranch',  			          $_LABELS['RP.SCR.loanByBranch']);
$tpl->setVariable('loanByProgram', 			          $_LABELS['RP.SCR.loanByProgram']);
$tpl->setVariable('loanByClient', 			          $_LABELS['RP.SCR.loanByClient']);

$tpl->setVariable('activeLoanPortfolio', 			  $_LABELS['activeLoanPortfolio']);
$tpl->setVariable('activeLoanPortfolioByLoan', 	      $_LABELS['RP.SCR.activeLoanPortfolioByLoan']);

$tpl->setVariable('cancelledLoanPortfolio', 		  $_LABELS['cancelledLoanPortfolio']);
$tpl->setVariable('cancelledLoanPortfolioByLoan', 	  $_LABELS['RP.SCR.cancelledLoanPortfolioByLoan']);

$tpl->setVariable('loanWriteOff', 			          $_LABELS['loanWriteOff']);
$tpl->setVariable('loanWriteOffByAdvisor',            $_LABELS['RP.SCR.loanWriteOffByAdvisor']);
$tpl->setVariable('loanWriteOffByBranch',             $_LABELS['RP.SCR.loanWriteOffByBranch']);
$tpl->setVariable('loanWriteOffByProgram',            $_LABELS['RP.SCR.loanWriteOffByProgram']);

$tpl->setVariable('writeOffPortfolio', 			      $_LABELS['writeOffPortfolio']);
//$tpl->setVariable('writeOffPortfolio', 		      $_LABELS['RP.SCR.writeOffPortfolio']);
$tpl->setVariable('writeOffHistory',  			      $_LABELS['RP.SCR.writeOffHistory']);

$tpl->setVariable('recovery', 				          $_LABELS['recovery']);
$tpl->setVariable('recoveryByAdvisor', 			      $_LABELS['RP.SCR.recoveryByAdvisor']);
$tpl->setVariable('recoveryByBranch',  			      $_LABELS['RP.SCR.recoveryByBranch']);
$tpl->setVariable('recoveryByProgram', 			      $_LABELS['RP.SCR.recoveryByProgram']);

$tpl->setVariable('clientIOM', 				          $_LABELS['clientIOM']);
$tpl->setVariable('clientIOMByAdvisor', 		      $_LABELS['RP.SCR.clientIOMByAdvisor']);
$tpl->setVariable('clientIOMByBranch',  		      $_LABELS['RP.SCR.clientIOMByBranch']);
$tpl->setVariable('clientIOMByProgram', 		      $_LABELS['RP.SCR.clientIOMByProgram']);

$tpl->setVariable('delinquencyPortfolio', 		      $_LABELS['delinquencyPortfolio']);
$tpl->setVariable('delinquencyByAdvisor', 		      $_LABELS['RP.SCR.delinquencyByAdvisor']);
$tpl->setVariable('delinquencyByBranch',  		      $_LABELS['RP.SCR.delinquencyByBranch']);
$tpl->setVariable('delinquencyByProgram', 		      $_LABELS['RP.SCR.delinquencyByProgram']);
$tpl->setVariable('delinquencyByClient', 		      $_LABELS['RP.SCR.delinquencyByClient']);
$tpl->setVariable('delinquencyByGroup', 		      $_LABELS['RP.SCR.delinquencyByGroup']);

$tpl->setVariable('operationalLoss', 			      $_LABELS['operationalLoss']);
$tpl->setVariable('operationalLossByAdvisor', 		  $_LABELS['RP.SCR.operationalLossByAdvisor']);
$tpl->setVariable('operationalLossByBranch',  		  $_LABELS['RP.SCR.operationalLossByBranch']);
$tpl->setVariable('operationalLossByProgram', 		  $_LABELS['RP.SCR.operationalLossByProgram']);
?>
