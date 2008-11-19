<?php
class BORROWER extends WEBPAGE
{  
  /* BORROWER parameters */
  var $frostdata = array();
  var $data  = array();
  var $fields = array();

  function BORROWER($bt,$bid)  //class constructor
  {
  switch ($bt) {
    case 'B':
      $borrower = new SOCIETY($bid);
      $members = $borrower->getMembers();
      break;
    case 'G':
      $borrower = new SOCIETY($bid);
      $members[$bid] = $borrower->data['name'];
      break;
    case 'I':
      $borrower = new CLIENT($bid);
      $members[$bid] = $borrower->data['name'];
      break;
    default:
      WEBPAGE::redirect('index.php?logout=1');
      break;  
    }  	
  $this->data = $borrower->data;
  $this->data['members'] = $members;
  }
  
}
?>
