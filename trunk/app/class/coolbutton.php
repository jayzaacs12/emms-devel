<?php
class COOlBUTTON
{
    /* COOLBUTTON parameters */
    var $id = '';
    var $ico = '';
    var $alt = '';
    var $href = '';
    var $onClick = '';

    function COOLBUTTON($data)
    {
	$this->id = $data[id];
	$this->ico = $data[ico];
	$this->alt = $data[alt];
	$this->href = $data[href];
	$this->onClick = $data[onClick];
    }

    function printbutton($button)
    {	
	printf ("<a href=\"%s\" onClick=\"%s\" title=\"%s\"><IMG style='border-width:0' id=%s src=\"img/icons/%s.gif\" alt=\"%s\" onmouseover=\"document.all.%s.src='img/icons/%s_on.gif'; self.status='%s'; return true\" onmouseout=\"document.all.%s.src='img/icons/%s.gif'; self.status=''; return true\"></A>", $button->href, $button->onClick, $button->alt, $button->id, $button->ico, $button->alt, $button->id, $button->ico, $button->alt, $button->id, $button->ico );
    }
    
    function getButton($button)
    {
    /*
	return sprintf("<a href='%s' onClick='%s' title='%s'>
					<img style='border-width:0;margin:0px' id='%s' src='%s%s/icons/%s.png' alt='%s' 
					onmouseover=\"this.style.margin='0px';this.style.width='32px';this.style.height='32px';self.status='%s'; return true\" 
					onmouseout=\"this.style.margin='0px';this.style.width='32px';this.style.height='32px';self.status=''; return true\">", $button->href,$button->onClick,$button->alt,$button->id,WEBPAGE::_THEMES_PATH,WEBPAGE::$theme,$button->ico,$button->alt,$button->alt);	
    */
	return sprintf("<a href='%s' onClick='%s' title='%s'>
					<img style=\"border-width:0;margin:'0px'\" id='%s' src='%s%s/icons/%s.png' alt='%s'
					onmouseover=\"self.status='%s'; return true\" 
					onmouseout=\"self.status=''; return true\">", $button->href,$button->onClick,$button->alt,$button->id,WEBPAGE::_THEMES_PATH,WEBPAGE::$theme,$button->ico,$button->alt,$button->alt);	
	    }    


}
?>