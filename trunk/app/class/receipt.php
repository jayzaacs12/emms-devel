<?php
class RECEIPT extends WEBPAGE
{

  function RECEIPT($id = '')  //class constructor
  {
  if ($id) {

    $this->data = current(SQL::select('tblReceipts r, tblUsers u','r.*, u.username',sprintf('r.id=%s and u.id = r.user_id',$id)));

    } else {

    $this->data = array();

    }
  }

  function loadPayments($id = '')
  {
  if (!($id)) {$id = $this->data['id'];}

  $fields = 'p.*,l.loan_code,concat(left(c.first,1),"."," ",left(c.last,14)) client';
  $param = sprintf("rp.receipt_id = '%s' AND p.id = rp.payment_id AND l.id = p.loan_id AND c.id = l.client_id",$id);

  $table = 'tblLinkReceiptsPayments AS rp, tblPayments AS p, tblLoans l, tblClients c';
  $this->data['payments']['normal'] = SQL::getAssoc($table,$fields,$param);

  $table = 'tblLinkReceiptsPayments AS rp, tblPaymentsRollback AS p, tblLoans l, tblClients c';
  $this->data['payments']['rollback'] = SQL::getAssoc($table,$fields,$param);
  }

}
?>
