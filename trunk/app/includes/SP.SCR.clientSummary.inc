<?php

  $data2 = WEBPAGE::$dbh->getAll(sprintf("select sp.sponsor from tblSponsors sp where sp.id =%s",$_REQUEST['id']));

  $data1 = WEBPAGE::$dbh->getAll(sprintf("
            select
              z.zone,
              count(c.id) qty,
              c.gender,
              round(AVG(YEAR(NOW())-YEAR(c.birthdate))) age,
              round(avg(c.dependants),1) dep
            from
              tblClients c,
              tblLoans l,
              tblZones z,
              tblLoansMasterDetails lmd,
              tblLoansMaster lm
            where
              c.id               = l.client_id and
              z.id               = l.zone_id and
              lmd.loan_id        = l.id and
              lm.id              = lmd.master_id and
              lm.sponsor_id      = %s and
              YEAR(c.birthdate) < YEAR(NOW())-10 and
              YEAR(NOW())-YEAR(c.birthdate) < 90
            group by l.zone_id,c.gender",$_REQUEST['id']));

  foreach($data1 as $key=>$val) {

    $mrow[$val['zone']]['zone']  = '';
    $mrow[$val['zone']]['qty_F'] = '';
    $mrow[$val['zone']]['qty_M'] = '';
    $mrow[$val['zone']]['age_F'] = '';
    $mrow[$val['zone']]['age_M'] = '';
    $mrow[$val['zone']]['dep_F'] = '';
    $mrow[$val['zone']]['dep_M'] = '';

    }

  foreach($data1 as $key=>$val) {

    $mrow[$val['zone']]['zone']  = $val['zone'];
    $mrow[$val['zone']][sprintf('qty_%s',$val['gender'])] = $val['qty'];
    $mrow[$val['zone']][sprintf('age_%s',$val['gender'])] = $val['age'];
    $mrow[$val['zone']][sprintf('dep_%s',$val['gender'])] = $val['dep'];

  }

  $head = array(
    'zone'  => $_LABELS['tblZones.zone'],
    'qty_F' => $_LABELS['qty_F'],
    'qty_M' => $_LABELS['qty_M'],
    'age_F' => $_LABELS['age_F'],
    'age_M' => $_LABELS['age_M'],
    'dep_F' => $_LABELS['dep_F'],
    'dep_M' => $_LABELS['dep_M']);

  $total = array();

  foreach ($mrow as $key=>$val) {
    $total['zone']  = $_LABELS['total'];
    $total['qty_F']  += $val['qty_F'];
    $total['qty_M']  += $val['qty_M'];

    $total['age_F']   = '-';
    $total['age_M']   = '-';

    $total['age_F_total']  += $val['qty_F']*$val['age_F'];
    $total['age_M_total']  += $val['qty_M']*$val['age_M'];

    $total['dep_F']  = '-';
    $total['dep_M']  = '-';

    $total['dep_F_total']  += $val['qty_F']*$val['dep_F'];
    $total['dep_M_total']  += $val['qty_M']*$val['dep_M'];
    }

  $total['age_F'] = $total['age_F_total'] ? round($total['age_F_total']/$total['qty_F']) : '-';
  $total['age_M'] = $total['age_M_total'] ? round($total['age_M_total']/$total['qty_M']) : '-';
  
  $total['dep_F'] = $total['dep_F_total'] ? round($total['dep_F_total']/$total['qty_F'],1) : '-';
  $total['dep_M'] = $total['dep_M_total'] ? round($total['dep_M_total']/$total['qty_M'],1) : '-';


  unset($total['age_F_total']);
  unset($total['age_M_total']);

  unset($total['dep_F_total']);
  unset($total['dep_M_total']);

  $mrow[] = $total;

  $tpl->setVariable('sponsor_name_label',$_LABELS['tblSponsors.sponsor']);
  $tpl->setVariable('sponsor_name_value',$data2[0]['sponsor']);
  $tpl->setVariable('chart', count($mrow)>1 ? WEBPAGE::printchart($mrow,$head) : $_LABELS['noData']);
?>
