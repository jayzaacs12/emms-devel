<?php
class SPONSOR extends WEBPAGE
{

  /* SPONSOR parameters */
  var $frostdata = array(0 => 'username');
  var $data      = array();

  function SPONSOR($id = '')  //class constructor
  {
  if ($id) {

    $this->data = current(WEBPAGE::$dbh->getAll(sprintf("select * from tblSponsors where id = '%s'",$id)));
    $this->data['verify'] 		  = $this->data['password'];
    $this->data['oldpassword'] 	= $this->data['password'];
    $this->data['zone_id']      = WEBPAGE::$dbh->getAssoc(sprintf("select zone_id,zone_id from tblLinkSponsorsZones where sponsor_id=%s",$id));

    } else {

    $this->data['creator_date'] = date('Y-m-d');
    $this->data['creator_id']   = WEBPAGE::$userID;
    $this->frostdata = array();

    }
  }

  function add()
  { 

  WEBPAGE::$dbh->query(sprintf("insert into tblSponsors  set
                                                         username      = '%s',
                                                         password      = '%s',
                                                         sponsor       = '%s',
                                                         first         = '%s',
                                                         middle        = '%s',
                                                         last          = '%s',
                                                         email         = '%s',
                                                         memo          = '%s',
                                                         creator_id    = '%s',
                                                         creator_date  = '%s',
                                                         editor_id     = '%s',
                                                         editor_date   = '%s',
                                                         active        = '1'",
                                                         $this->data['username'],
                                                         $this->data['password'],
                                                         $this->data['sponsor'],
                                                         $this->data['first'],
                                                         $this->data['middle'],
                                                         $this->data['last'],
                                                         $this->data['email'],
                                                         $this->data['memo'],
                                                         $this->data['creator_id'],
                                                         $this->data['creator_date'],
                                                         $this->data['editor_id'],
                                                         $this->data['editor_date']));
  
     if ($id = mysql_insert_id(WEBPAGE::$dbh->connection)) {
      $this->updateZones($id,$this->data['zone_id']);
     }
   return $id;
  
  }

  function update()
  {
  WEBPAGE::$dbh->query(sprintf("update tblSponsors set
                                                         sponsor       = '%s',
                                                         first         = '%s',
                                                         middle        = '%s',
                                                         last          = '%s',
                                                         email         = '%s',
                                                         memo          = '%s',
                                                         editor_id     = '%s',
                                                         editor_date   = '%s',
                                                         active        = '%s'
                                                   where
                                                         id            =  %s",
                                                         $this->data['sponsor'],
                                                         $this->data['first'],
                                                         $this->data['middle'],
                                                         $this->data['last'],
                                                         $this->data['email'],
                                                         $this->data['memo'],
                                                         $this->data['editor_id'],
                                                         $this->data['editor_date'],
                                                         $this->data['active'],
                                                         $this->data['id']));
  $this->updateZones($this->data['id'],$this->data['zone_id']);
  return $this->data['id'];
  }

  function changePassword($password)
  {
  WEBPAGE::$dbh->query(sprintf("update tblSponsors set password = '%s' where id = '%s'", $password, $this->data['id']));
  return $this->data['id'];
  }

  function loadDonations()
  {
  $data = WEBPAGE::$dbh->getAll(sprintf("select sd.id,
                                        sd.src_amount,
                                        c_src.symbol as src_currency,
                                        sd.conv_amount,
                                        c_conv.symbol as conv_currency,
                                        sd.creator_date
                                      from
                                        tblSponsorsDonations as sd,
                                        tblCurrencys as c_src,
                                        tblCurrencys as c_conv
                                      where
                                        sd.sponsor_id = '%s' and
                                        c_src.id = sd.src_currency_id and
                                        c_conv.id = sd.conv_currency_id order by sd.id desc", $this->data['id']));
  $this->data['donations'] = $data;
  }

  function loadExtendedInfo()
  {
  global $_LABELS;
  global $_CONF;

  $data = current(WEBPAGE::$dbh->getAll(sprintf("
                                             select
                                                CONCAT(uc.first,' ',uc.last) AS creator,
                                                DATE_FORMAT(s.creator_date,'%s') AS f_creator_date,
                                                CONCAT(ue.first,' ',ue.last) AS editor,
                                                DATE_FORMAT(s.editor_date,'%s') AS f_editor_date
                                               from
                                                tblSponsors as s, tblUsers as uc, tblUsers as ue
                                               where
                                                s.id  = '%s' and uc.id = '%s' and ue.id = '%s'",
                                                $_CONF['date_format_mysql'],
                                                $_CONF['date_format_mysql'],
                                                $this->data['id'],
                                                $this->data['creator_id'],
                                                $this->data['editor_id'])));

  $this->data['creator']                  = $data['creator'];
  $this->data['f_creator_date']           = $data['f_creator_date'];
  $this->data['editor']                   = $data['editor'];
  $this->data['f_editor_date']            = $data['f_editor_date'];
  
  $c=0;
	$this->data['buttondata'][$c][id]			  = "SP.SCR.addSponsor";
	$this->data['buttondata'][$c][href]		  = "index.php?scr_name=SP.SCR.addSponsor&id=".$this->data['id'];
	$this->data['buttondata'][$c][alt]		  = $_LABELS['edit'];
	$this->data['buttondata'][$c][onClick]  = "";
	$this->data['buttondata'][$c][ico]		  = "edit";

  $c++;
  $this->data['buttondata'][$c][id]		    = "SP.SCR.changePassword";
  $this->data['buttondata'][$c][href]		  = "index.php?scr_name=SP.SCR.changePassword&id=".$this->data['id'];
  $this->data['buttondata'][$c][alt]		  = $_LABELS['deactivate'];
  $this->data['buttondata'][$c][onClick]	= "";
  $this->data['buttondata'][$c][ico]		  = "user_rem";
  }

  function getDonationTotals()
  {
  return WEBPAGE::$dbh->getAssoc("select s.id,sum(sd.conv_amount) from tblSponsors as s,tblSponsorsDonations as sd where sd.sponsor_id = s.id group by s.id");
  }

  function getDisbursmentTotals()
  {
  return WEBPAGE::$dbh->getAssoc("select s.id,sum(lm.amount) from tblSponsors as s,tblLoansMaster as lm where lm.sponsor_id = s.id group by s.id");
  }

  function getPaymentTotals()
  {
  return WEBPAGE::$dbh->getAssoc("            select
                                                s.id,sum(p.principal)
                                              from
                                                tblSponsors as s,
                                                tblPayments as p,
                                                tblLoansMaster as lm,
                                                tblLoansMasterDetails as lmd
                                              where
                                                lm.sponsor_id = s.id and
                                                lmd.master_id = lm.id and
                                                p.loan_id = lmd.loan_id
                                              group by
                                                s.id");
  }

  function getSponsorsByBalance($balance = 0)
  {
  $data = array();
  $sponsors = WEBPAGE::$dbh->getAssoc("select id,sponsor from tblSponsors");
  $donations = $this->getDonationTotals();
  $disbursments = $this->getDisbursmentTotals();
  $payments = $this->getPaymentTotals();
  foreach($sponsors as $key=>$val) {
    if (($funds = $donations[$key] + $payments[$key] - $disbursments[$key]) >= $balance) { $data[$key] = sprintf("%s",$val); }
    }
  return $data;
  }

  function getSponsorsByZone($zone_id = 0)
  {
  return WEBPAGE::$dbh->getAssoc(sprintf("select s.id,s.sponsor from tblSponsors s, tblLinkSponsorsZones sz where sz.zone_id = %s and s.id = sz.sponsor_id", $zone_id));
  }

function getZones()
  {
  
  return WEBPAGE::$dbh->getAssoc(sprintf("select z.id,concat(pz.short_name,': ',p.program) zone from tblZones z, tblZones pz, tblPrograms p where z.parent_id > 0 and pz.id = z.parent_id and p.id=z.program_id order by zone;"));
  
  }

function updateZones($id,$zones) {
  WEBPAGE::$dbh->query(sprintf("delete from tblLinkSponsorsZones where sponsor_id = %s", $id));
  $values = array(); 
  foreach($zones as $zone_id) {
    $values[] = sprintf("(%s,%s)",$id,$zone_id); 
    }
  if (count($values)) {
    WEBPAGE::$dbh->query(sprintf("insert into tblLinkSponsorsZones (sponsor_id,zone_id) values %s",implode(',',$values))); 
    }
  }

}
?>