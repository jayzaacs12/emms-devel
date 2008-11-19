<?
//----------------------------------------------	
if(!class_exists("TTFButton")) {
class	TTFButton

{
	
	// image Handles
	var $webface = 'button.php'; // php script to be called from browser
	var $cssTheme = "blue";
	
	var	$hNew ;
	var	$hButton;
	var	$hIconL;
	var	$hTxt;
	var $hIconR;

	var $sButton = "themes/%s/buttons/button.png";
	var $sIconL = "" ;
	var $sIconR = "" ;

    var $top_margin = 5 ; 
	var $bottom_margin = 15 ;
	var $left_margin = 23 ;
	var $right_margin = 33;
		
	var	$text = '     ';
	var $ttf_font = "themes/%s/fonts/Verdana.ttf";
	var $font_size = 10 ;
	var $html_color = "#FFFFFF" ;	
	
	function	TTFButton(  $cssTheme = "", $txt = "",  $sButton = "", $sIconL = "", $sIconR = "", $sTTFFont = "", $nFontSize = "", $sHtmlColor = "", $webface = "" )
	{	    
		if( $webface ) 		$this->webface = $webface;
		if( $cssTheme ) 	$this->cssTheme = $cssTheme;

		if( $txt ) 			$this->text = ( ord(trim($txt)) > 127 ) ? $this->gb2utf8($txt) : $txt;
		if( $sButton ) 		{ $this->sButton = $sButton ; } else { $this->sButton = sprintf($this->sButton, $this->cssTheme); }
		
		if( $sTTFFont )  	{ $this->ttf_font = $sTTFFont; } else {  $this->ttf_font = sprintf($this->ttf_font, $this->cssTheme); }

		if( $nFontSize ) 	$this->font_size = $nFontSize ;
		if( $sIconL )    	$this->sIconL = $sIconL ;
		if( $sIconR ) 		$this->sIconR = $sIconR ;
		if( $sHtmlColor ) 	$this->html_color = trim($sHtmlColor) ;
	}
    
	function imgimg($txt = '') 
	{
	    if(!($txt)) { $txt =  $this->txt; }
        return sprintf("<img alt='%s' src='%s?txt=%s'>",$txt,$this->webface,$txt);
    }

	function imglink($href, $txt = '') 
	{
        return sprintf("<a name='%s' href='%s'>%s</a>",$txt,$href,$this->imgimg($txt));
    }

    function imgsubmit($form, $txt = '') 
	{
	   return $this->imglink(sprintf('javascript:document.%s.submit()',$form),$txt);
    }

    function imgreset($form, $txt = '') 
	{
	   return $this->imglink(sprintf('javascript:document.%s.reset()',$form),$txt);
    }

	function	ripper()
	{		
		$this->hButton = $this->imageCreateFromImg( $this->sButton );
		
		$this->txt2img() ;
		
		$nLeft = $this->left_margin ? $this->left_margin : 6 ;
		$nRight = $this->right_margin ? $this->right_margin : 6 ;
		$nTxt = imageSX( $this->hTxt );
		
		$nIconMargin = 4 ;
	        $nIconL = 0;
		if( is_file( $this->sIconL ) ) :
			$this->hIconL = $this->imageCreateFromImg( $this->sIconL ) ;
			$nIconL =  imageSX( $this->hIconL ) + $nIconMargin;
		endif;
	        $nIconR = 0;
		if( is_file( $this->sIconR ) ) :
			$this->hIconR = $this->imageCreateFromImg( $this->sIconR ) ;
			$nIconR =  imageSX( $this->hIconR ) + $nIconMargin;
		endif;
		
		$newX = $nLeft + $nIconL + $nTxt + $nIconR + $nRight;
		$newY = imageSY($this->hButton) ;
		$this->hNew = imageCreate( $newX, $newY );		

		//int ImageCopy (resource dst_im, resource src_im, int dst_x, int dst_y, int src_x, int src_y, int src_w, int src_h)
		
		imageCopy( $this->hNew, $this->hButton,	   0, 0,     0, 0,    $nLeft, $newY );  // Cut Button Left
		imageCopy( $this->hNew, $this->hButton,  $nLeft, 0,     $nLeft, 0,     $nIconL+$nTxt+$nIconR, $newY ); // Background of IconL + Text + IconR

		if( $this->hIconL ) :
			$dst_y = $this->dst_y( $this->hIconL );
			imageCopy( $this->hNew, $this->hIconL,	   $nLeft, $dst_y,     0, 0,     $nIconL-$nIconMargin,   imageSY($this->hIconL)   ); // Copy IconL
		endif;

		$dst_y = $this->dst_y( $this->hTxt ); 
		imageCopy( $this->hNew, $this->hTxt,      $nLeft+$nIconL,  $dst_y,     0, 0,     $nTxt, imageSY($this->hTxt)   ); // Copy Text

		if( $this->hIconR ) :
			$dst_y = $this->dst_y( $this->hIconR ); 
			imageCopy( $this->hNew, $this->hIconR,	   $nLeft+$nIconL+$nTxt+$nIconMargin, $dst_y,     0, 0,     $nIconR, imageSY($this->hIconR) ); // Copy IconR
		endif;

		imageCopy( $this->hNew, $this->hButton, $nLeft+$nIconL+$nTxt+$nIconR, 0, imageSX($this->hButton)-$nRight, 0, $nRight, $newY ); // Cut Button Right
	}
	
	function	dst_y( $hImg )
	{
		$valid_height = imageSY( $this->hButton ) - $this->top_margin - $this->bottom_margin ;
		$dst_y = $this->top_margin+Floor( ( $valid_height - imageSY($hImg) ) / 2 );
		return $dst_y ;
	}
	
	
	function	imageCreateFromImg( $filename )
	{
		$split = split( "\.", basename( trim( $filename ) ) );
		$ext = $split[ count($split) - 1 ] ;
		switch( strtolower( $ext ) )
		{
			case	"gif" :
				return imageCreateFromGif( $filename );
				break;
			case 	"png" :
				return imagecreatefrompng( $filename );
				break;
			case 	"jpg" :
			case 	"jpeg" :
				return imagecreatefromjpeg( $filename );
				break;
		}
	}
	
	
	function	show()
	{
		$this->ripper();
		$split = split( "\.", basename( trim( $this->sButton ) ) );
		$ext = $split[ count($split) - 1 ] ;
		switch( strtolower( $ext ) )
		{
			case	"gif" :
				Header( "Content-Type: image/gif" );
				imageGif( $this->hNew ) ;
				break;
			case 	"png" :
				Header("Content-type: image/png");
				ImagePNG($this->hNew);
				break;
			case 	"jpg" :
			case 	"jpeg" :
				Header("Content-type: image/jpeg");
				ImageJPEG($this->hNew );
				break;
		}
		$this->cleanup();
	}
	
	
	function	cleanup()
	{		
		imageDestroy($this->hNew );
		imageDestroy( $this->hTxt );
		imageDestroy( $this->hButton );
		if( $this->hIconL ) imageDestroy( $this->hIconL );
		if( $this->hIconR ) imageDestroy( $this->hIconR );
		//print "New Y = " . imageSY( $this->hNew ) . " TXT Y = " . imageSY($this->hTxt) . " /2 = " . Floor((imageSY($this->hNew)-imageSY($this->hTxt))/2);
	}


	function	txt2img()
	{
		$hImg_temp = imageCreate( 10, 10 );
		$txt_color = $this->txtcolor( $hImg_temp );
		$p4 = imageTTFText( $hImg_temp, $this->font_size, 0, 0, 0, $txt_color, $this->ttf_font, $this->text );
		imageDestroy( $hImg_temp );
		
		$margin = 4 ;
		$x_size = ($p4[2]-$p4[0]) + $margin ;
		$y_size = $p4[1]-$p4[7] + $margin ;
		$this->hTxt = imageCreate( $x_size, $y_size );
		//$hTxt = imageCreate( 600, 300 );
		$bg_color = imageColorAllocate( $this->hTxt, 200, 200, 200 );
		imageFill( $this->hTxt, 0, 0, $bg_color );
		imageColorTransparent( $this->hTxt, $bg_color );
		$x = 0+floor($margin/2) ;
		$y = $y_size-floor($margin/2);
		$txt_color = $this->txtcolor( $this->hTxt );
		imageTTFText( $this->hTxt, $this->font_size, 0, $x, $y, $txt_color, $this->ttf_font, $this->text );
		//$txt_shadow = imageColorAllocate( $hTxt, 100, 100, 100 );
		//imageTTFText( $hTxt, $this->font_size, 0, $x-1, $y-1, $txt_shadow, $this->ttf_font, $this->text );
		//return $hTxt ;
	}
	
	function	txtcolor( $hImg_temp )
	{
		$hexcolor = ( "#" == substr( $this->html_color, 0, 1 )  ) ? substr( $this->html_color, 1, strlen($this->html_color)-1 ) . "000000" : $this->html_color . "000000" ;
		return imageColorAllocate( $hImg_temp, hexdec(substr($hexcolor,0,2))  ,  hexdec(substr($hexcolor,2,2)),  hexdec(substr($hexcolor,4,2)) );
	}

}

}

?>