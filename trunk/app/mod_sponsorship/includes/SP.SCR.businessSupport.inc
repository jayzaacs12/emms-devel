<?php
$show_gohome = true;

  $extra['qty']['header']		       = 'qty';
  $extra['qty']['pack']		         = '%s';
  $extra['qty']['align']		       = 'right';

  $extra['type']['header']	       = 'tblBusinessTypes.type';
  $extra['type']['pack']	         = '%s';
  $extra['type']['align']	         = 'left';

  $extra['description']['header']  = 'description';
  $extra['description']['pack']	   = '%s';
  $extra['description']['align']   = 'left';

  $extra['param']['ref']		       = WEBPAGE::$scr_name;

  $mrow = WEBPAGE::$dbh->getAll(sprintf("select
                                           count(l.business_id) qty,
                                           bt.type,
                                           bt.description
                                         from
                                           tblLoans l,
                                           tblLoansMaster lm,
                                           tblLoansMasterDetails lmd,
                                           tblBusinessTypes bt,
                                           tblBusiness b
                                         where
                                           lmd.loan_id   = l.id and
                                           lm.id         = lmd.master_id and
                                           lm.sponsor_id = %s and
                                           b.id          = l.business_id and
                                           bt.id         = b.type_id
                                         group by
                                           bt.id
                                         order by
                                           qty desc",WEBPAGE::$userID));

  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache);
  require_once '../includes/BS.SCR.pager.inc';

?>
