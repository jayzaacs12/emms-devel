<?php
class Graph
{
    /* Graph parameters */
    var $title = '';
    var $xname = '';
    var $yname = '';
    var $series = ''; 
	var $plot_colors = '';  
    
    function Graph($title, $xname, $yname, $series, $options = array())
    {
    $options['x-labels']['on'] 		= isset($options['x-labels']['on']) 	? $options['x-labels']['on']    : 1;		  
	$options['x-labels']['extra'] 	= isset($options['x-labels']['extra'])  ? $options['x-labels']['extra'] : '';		  
	$options['x-labels']['alt'] 	= isset($options['x-labels']['alt'])    ? $options['x-labels']['alt']   : '';		  
	$options['y-round']		 	 	= isset($options['y-round'])			? $options['y-round']			: 2;		  
    	
	define("ERROR_MESSAGE", 'DATA NOT AVAILABLE');
    define("CANVAS_WIDTH", 700);
	define("CANVAS_HEIGHT", 280);
    define("MARGIN_LEFT", 65);
    define("LEGEND_MARGIN_LEFT", 500);    
	define("GRAPH_WIDTH", 400);
    define("GRAPH_HEIGHT", 150);
	define("GRAPH_MARGIN_TOP", 50);	
	define("TITLE_MARGIN_TOP", 5);
	define("YLABEL_MARGIN_LEFT", 15);
	define("XLABEL_MARGIN_TOP", 210);
	
	define("PLOT_BASE", GRAPH_MARGIN_TOP + GRAPH_HEIGHT);
	
	$this->title 	= $title;
	$this->xname 	= $xname;
	$this->yname 	= $yname;
	$this->series 	= $series;
	$this->options 	= $options;
	
	$this->text_color[r] = 100;
	$this->text_color[g] = 100;	
	$this->text_color[b] = 100;
	
	$this->canvas_color[r] = 255;
	$this->canvas_color[g] = 255;	
	$this->canvas_color[b] = 255;	
								
	$this->plot_colors[0] = '#0000FF';
	$this->plot_colors[1] = '#FFFF00';
    $this->plot_colors[2] = '#CC0000';
	$this->plot_colors[3] = '#FFAA00';
	$this->plot_colors[4] = '#00CC00';
	$this->plot_colors[5] = '#000066';
	$this->plot_colors[6] = '#FFAA00';
	$this->plot_colors[7] = '#AA8800';
	$this->plot_colors[8] = '#664400';
	$this->plot_colors[9] = '#FFFF00';

    }
    
    function printEmpty($graph) 
	{
	
    $image = Graph::create_canvas($graph);
    
    $V_STEP = 10;
    $H_STEP = GRAPH_WIDTH / 10;
    $H_WIDTH = GRAPH_WIDTH;
    
	$image = Graph::create_grid($image, $H_WIDTH, $H_STEP, $V_STEP, $graph->xname, $graph->yname, $graph->options);
	
	$color_legend = imagecolorallocate($image, 0, 0, 0);    

    imagestring($image, 18, MARGIN_LEFT+60,  (GRAPH_MARGIN_TOP + (GRAPH_HEIGHT / 2)), ERROR_MESSAGE, $color_legend);

     
    //set content type
    header("Content-type: image/jpeg");
    //send image
    Imagejpeg($image, '', 100);
    ImageDestroy($image);
    }
	
    function printPie($graph) 
	{
	
	$series_names = array_keys($graph->series);
    $column_names = array_keys($graph->series[$series_names[0]]);    

    $num = count($column_names);
    
    $values = $graph->series[$series_names[0]]; 
    $sum = array_sum($values);
    
	$image = Graph::create_canvas($graph);
	
	$color_legend = imagecolorallocate($image, $graph->text_color[r], $graph->text_color[g], $graph->text_color[b]);    

	$angle1 = 0;
	
	$x_center = 150;
	$y_center = 150;
	$x_radius = 150;
	$y_radius = 150;
	
	//draw pie
	for ($i=0;$i<$num;$i++) {
		$r = hexdec(substr($graph->plot_colors[$i], 1, 2));
		$g = hexdec(substr($graph->plot_colors[$i], 3, 2));
		$b = hexdec(substr($graph->plot_colors[$i], 5, 2));
		$color[$i] = imagecolorallocate($image, $r, $g, $b);
		$value = ((($graph->series[$series_names[0]][$column_names[$i]]))/($sum));	
		$angle2 = $angle1 + 340 * $value;
    	imagefilledarc($image, $x_center, $y_center, $x_radius, $y_radius, $angle1, $angle2, $color[$i], 4);
		$x_values_pos[$i] = $x_center + 0.7 * $x_radius * cos(Pi()*(($angle1+$angle2)/360))-20;
		$y_values_pos[$i] = $y_center + 0.7 * $y_radius * sin(Pi()*(($angle1+$angle2)/360))-5;	
		imagestring($image, 2, $x_values_pos[$i],  $y_values_pos[$i], round(100*$value,2).'%',  $color_legend);
		$angle1 = $angle2 + 20/$num;
		//print legend
    	imagefilledrectangle($image, LEGEND_MARGIN_LEFT - 80 ,  GRAPH_MARGIN_TOP + (($i)*(GRAPH_HEIGHT / 12)),  LEGEND_MARGIN_LEFT -75,  GRAPH_MARGIN_TOP + (($i)*(GRAPH_HEIGHT / 12)) + 4, $color[$i]);
    	imagestring($image, 2, LEGEND_MARGIN_LEFT-60,  GRAPH_MARGIN_TOP + ($i*(GRAPH_HEIGHT / 12)-5), $column_names[$i], $color_legend);
		}


    $color_canvas = imagecolorallocate($image, $graph->canvas_color[r], $graph->canvas_color[g], $graph->canvas_color[b]);    
	imagefilledarc($image, 150, 150, 50, 50, 0, 360, $color_canvas, 4);
		
		//set content type
		header("Content-type: image/jpeg");
		//send image
		Imagejpeg($image, '', 100);
		ImageDestroy($image);
	}
    
    
    function printBars($graph) 
	{
	
	if (!(is_array($graph->series))) { Graph::printEmpty($graph); }
	
    $image = Graph::create_canvas($graph);

    $series_names = array_keys($graph->series);
    $column_names = array_keys($graph->series[$series_names[0]]);    
    $num = count($column_names);
    
    $values = $graph->series[$series_names[0]]; 
	rsort($values);
    $max = $values[0];
    
    $V_STEP = $max / 10;
    $H_STEP = round(GRAPH_WIDTH / (2*($num+1)), 0);
    $H_WIDTH = 2*($num + 1)* $H_STEP;
    
	$image = Graph::create_grid($image, $H_WIDTH, $H_STEP, $V_STEP, $graph->xname, $graph->yname, $graph->options);
	
	$color_legend = imagecolorallocate($image, $graph->text_color[r], $graph->text_color[g], $graph->text_color[b]);    
 
	
	//draw bars
	for ($i=0;$i<$num;$i++) {
		$r = hexdec(substr($graph->plot_colors[$i], 1, 2));
		$g = hexdec(substr($graph->plot_colors[$i], 3, 2));
		$b = hexdec(substr($graph->plot_colors[$i], 5, 2));
		$color[$i] = imagecolorallocate($image, $r, $g, $b);	
    	imagefilledrectangle($image, MARGIN_LEFT + $H_STEP + (2*$H_STEP*$i), PLOT_BASE - ((GRAPH_HEIGHT * $graph->series[$series_names[0]][$column_names[$i]])/$max), MARGIN_LEFT + $H_STEP + (2*$H_STEP*($i+1)), PLOT_BASE, $color[$i]);
    	imagefilledrectangle($image, LEGEND_MARGIN_LEFT -20 ,  GRAPH_MARGIN_TOP + ($i*(GRAPH_HEIGHT / 10)),  LEGEND_MARGIN_LEFT -18 + (GRAPH_HEIGHT / 30),  GRAPH_MARGIN_TOP + ($i*(GRAPH_HEIGHT / 10)) + 4, $color[$i]);
    	imagestring($image, 2, LEGEND_MARGIN_LEFT,  GRAPH_MARGIN_TOP + ($i*(GRAPH_HEIGHT / 10)) - 5, $column_names[$i], $color_legend);
		
		$x1 = MARGIN_LEFT + $H_STEP + (2*$H_STEP*$i) +1;
		imagestring($image, 2, $x1,  XLABEL_MARGIN_TOP, $graph->series[$series_names[0]][$column_names[$i]],  $color_legend);
		}    	
    
		//set content type
		header("Content-type: image/jpeg");
		//send image
		Imagejpeg($image, '', 100);
		ImageDestroy($image);
    }
    
    function printStdCols($graph) 
	{
    if (!(is_array($graph->series))) { Graph::printEmpty($graph); }
	
	$series_names = array_keys($graph->series);	
    $column_names = array_keys($graph->series[$series_names[0]]);

	$series_num = count($series_names);
	$column_num = count($column_names);
	
	for ($i=0;$i<$column_num;$i++) {
		for ($j=0;$j<$series_num;$j++) {
			if ( is_numeric($graph->series[$series_names[$j]][$column_names[$i]]) ) {
				$values[$i][$j] += $graph->series[$series_names[$j]][$column_names[$i]];
				} else {
				echo "error";
				return "error";
				}
			}
		}
	
    $image = Graph::create_canvas($graph);
    
    $V_STEP = 10;
    $H_STEP = round(GRAPH_WIDTH / (2*($column_num + 1)), 0);
    $H_WIDTH = 2*($column_num + 1)* $H_STEP;
    
	$image = Graph::create_grid($image, $H_WIDTH, $H_STEP, $V_STEP, $graph->xname, $graph->yname, $graph->options);
	
	$color_legend = imagecolorallocate($image, $graph->text_color[r], $graph->text_color[g], $graph->text_color[b]);    

	for ($i=0;$i<$column_num;$i++) {
		$sum = array_sum($values[$i]);
		if ($sum == 0) { $sum = 1; } //avoid div by zero in $top calculation
		$top = 0;
		$bottom = 0;
		for ($j=0;$j<$series_num;$j++) {
			$r = hexdec(substr($graph->plot_colors[$j], 1, 2));
			$g = hexdec(substr($graph->plot_colors[$j], 3, 2));
			$b = hexdec(substr($graph->plot_colors[$j], 5, 2));
			$color[$j] = imagecolorallocate($image, $r, $g, $b);
			
			$top += $values[$i][$j] ;
			
			$x1 = MARGIN_LEFT + $H_STEP + (2*$H_STEP*$i) +1;
			$y2 = PLOT_BASE - GRAPH_HEIGHT * $bottom / $sum;
			$x2 = MARGIN_LEFT + $H_STEP + (2*$H_STEP*($i+1))-1;
			$y1 = PLOT_BASE - GRAPH_HEIGHT * $top / $sum ;
			
    		imagefilledrectangle($image, $x1, $y1, $x2, $y2, $color[$j]);
			$bottom = $top;
			}
		imagestring($image, 2, $x1,  XLABEL_MARGIN_TOP, $column_names[$i],  $color_legend);
		}
		
	//print legend
	for ($i=0;$i<$series_num;$i++) {
		$j = $series_num - $i -1;
		$r = hexdec(substr($graph->plot_colors[$j], 1, 2));
		$g = hexdec(substr($graph->plot_colors[$j], 3, 2));
		$b = hexdec(substr($graph->plot_colors[$j], 5, 2));
		$color[$j] = imagecolorallocate($image, $r, $g, $b);
    	imagefilledrectangle($image, LEGEND_MARGIN_LEFT - 20 ,  GRAPH_MARGIN_TOP + (($i)*(GRAPH_HEIGHT / 12)),  LEGEND_MARGIN_LEFT -15,  GRAPH_MARGIN_TOP + (($i)*(GRAPH_HEIGHT / 12)) + 4, $color[$j]);
    	imagestring($image, 2, LEGEND_MARGIN_LEFT,  GRAPH_MARGIN_TOP + ($i*(GRAPH_HEIGHT / 12)-5), $series_names[$j], $color_legend);
		}

		//set content type
		header("Content-type: image/jpeg");
		//send image
		Imagejpeg($image, '', 100);
		ImageDestroy($image);
	}
	
	function printLines($graph) 
	{

    	if (!(is_array($graph->series))) { Graph::printEmpty($graph); }
    	
 		$series_names = array_keys($graph->series);	
		$series_num = count($series_names);
	
		$x_values = array();
		$y_values = array();
		for ($i=0;$i<$series_num;$i++) {
    		$x_series[$i] = array_keys($graph->series[$series_names[$i]]);
    		$x_values = array_merge($x_values, $x_series[$i]);			
			$y_series[$i] = $graph->series[$series_names[$i]];
			$y_values = array_merge($y_values, $y_series[$i]);
			}

		sort($x_values);
		$x_values = array_values(array_unique($x_values));
		sort($y_values);
		$y_values = array_values(array_unique($y_values));
		if (count($y_values)==1) { 
			$value = $y_values[0];
			$y_values[0] = min(0,$value);  
			$y_values[1] = max(10,$value); 
			}
		
		$image = Graph::create_canvas($graph);
		
		$y_num = count($y_values);
		$x_num = count($x_values);
		  
		$V_STEP = $y_values[$y_num-1] / 10;
    	$H_STEP = round(GRAPH_WIDTH / ($x_num - 1), 0);
    	$H_WIDTH = ($x_num-1) * $H_STEP;
    
		$image = Graph::create_grid($image, $H_WIDTH, $H_STEP, $V_STEP, $graph->xname, $graph->yname, $graph->options);
		
	//prints X-Labels
	  $OFFSET = ($H_WIDTH * $x_values[0])/($x_values[$x_num-1] - $x_values[0]); 	
	  $color_legend = imagecolorallocate($image, $graph->text_color[r], $graph->text_color[g], $graph->text_color[b]);  
	  $num = count($x_values);
	  $x2 = -($H_STEP);
	  for ($i=0;$i<$num;$i++) {
		$x1 = MARGIN_LEFT - $OFFSET + (($H_WIDTH*$x_values[$i])/($x_values[$x_num-1]-$x_values[0]));
		if (($x1-$x2 ) >= $H_STEP) { 			
			$graph->options['x-labels']['on'] ? imagestring($image, 2, $x1,  XLABEL_MARGIN_TOP, $x_values[$i], $color_legend) : '';
			} else {
			$x1 = $x2;
			}
		$x2 = $x1;
		}			
		
		//draw lines
		for ($j=0;$j<$series_num;$j++) {
			$r = hexdec(substr($graph->plot_colors[$j], 1, 2));
			$g = hexdec(substr($graph->plot_colors[$j], 3, 2));
			$b = hexdec(substr($graph->plot_colors[$j], 5, 2));
			$color[$j] = imagecolorallocate($image, $r, $g, $b);
			$x1 = MARGIN_LEFT + (($H_WIDTH * $x_series[$j][0])/($x_values[$x_num-1] - $x_values[0])) - $OFFSET; 
			$y1 = PLOT_BASE - ((GRAPH_HEIGHT * $graph->series[$series_names[$j]][$x_series[$j][0]])/$y_values[$y_num-1]);
			$num = count($x_series[$j]);
			for ($i=0; $i<$num-1; $i++) {
				$x2 = MARGIN_LEFT + (($H_WIDTH * $x_series[$j][$i+1])/($x_values[$x_num-1] - $x_values[0])) - $OFFSET; 
				$y2 = PLOT_BASE - ((GRAPH_HEIGHT * $graph->series[$series_names[$j]][$x_series[$j][$i+1]])/$y_values[$y_num-1]);
				imageline($image, $x1, $y1, $x2, $y2, $color[$j]);
				imagefilledarc($image, $x1, $y1, 5, 5, 0, 360, $color[$j], 4);
				$x1 = $x2;
				$y1 = $y2;
				}
			imagefilledarc($image, $x2, $y2, 5, 5, 0, 360, $color[$j], 4);	
		    imagefilledrectangle($image, LEGEND_MARGIN_LEFT - 20 ,  GRAPH_MARGIN_TOP + (($j)*(GRAPH_HEIGHT / 12)),  LEGEND_MARGIN_LEFT -15,  GRAPH_MARGIN_TOP + (($j)*(GRAPH_HEIGHT / 12)) + 4, $color[$j]);
    	    imagestring($image, 2, LEGEND_MARGIN_LEFT,  GRAPH_MARGIN_TOP + ($j*(GRAPH_HEIGHT / 12)-5), $series_names[$j], $color_legend);
			}		
		
		//set content type
		header("Content-type: image/jpeg");
		//send image
		Imagejpeg($image, '', 100);
		ImageDestroy($image);
	}

	function printBends($graph) 
	{
	if (!(is_array($graph->series))) { Graph::printEmpty($graph); }
		
	$image = Graph::create_canvas($graph);
	
    $series_names = array_keys($graph->series);	
	$series_num = count($series_names);
			
	
	$y_values = array();
	for ($j=0;$j<$series_num;$j++) {
		$x_series[$j] = array_keys($graph->series[$series_names[$j]]);	
		$num = count($x_series[$j]);	
		for ($i=0;$i<$num;$i++) {
			$y_series[$j][$i] = $graph->series[$series_names[$j]][$x_series[$j][$i]];
			}
		$y_values = array_merge($y_values, $y_series[$j]);	
		}

	// string x-series values? 
	$x_values = array();
	for ($j=0;$j<$series_num;$j++) {
		$num = count($x_series[$j]);
		for ($i=0;$i<$num;$i++) {
			$x_series[$j][$i] = floatval($x_series[$j][$i]);
			}
		$x_values = array_merge($x_values, $x_series[$j]);	
		}	
		
	//calculation of active window bounderies
	sort($x_values);
	$x_values = array_values(array_unique($x_values));
	sort($y_values);
	$y_values = array_values(array_unique($y_values));
	
	$y_num = count($y_values);
	$x_num = count($x_values);
	  
	$V_STEP = $y_values[$y_num-1] / 10;
    $H_STEP = round((GRAPH_WIDTH / ($x_num - 1)),0);
    $H_WIDTH = ($x_num-1) * $H_STEP;
    
	$image = Graph::create_grid($image, $H_WIDTH, $H_STEP, $V_STEP, $graph->xname, $graph->yname, $graph->options);

	$OFFSET = ($H_WIDTH * $x_values[0])/($x_values[$x_num-1] - $x_values[0]); 
	$color_legend = imagecolorallocate($image, $graph->text_color[r], $graph->text_color[g], $graph->text_color[b]);    
	
	// prints adjustment points and legend...
	for ($j=0;$j<$series_num;$j++) {
		$r = hexdec(substr($graph->plot_colors[$j], 1, 2));
		$g = hexdec(substr($graph->plot_colors[$j], 3, 2));
		$b = hexdec(substr($graph->plot_colors[$j], 5, 2));
		$color[$j] = imagecolorallocate($image, $r, $g, $b);
		
		imagefilledrectangle($image, LEGEND_MARGIN_LEFT - 20 ,  GRAPH_MARGIN_TOP + (($j)*(GRAPH_HEIGHT / 12)),  LEGEND_MARGIN_LEFT -15,  GRAPH_MARGIN_TOP + (($j)*(GRAPH_HEIGHT / 12)) + 4, $color[$j]);
    	imagestring($image, 2, LEGEND_MARGIN_LEFT,  GRAPH_MARGIN_TOP + ($j*(GRAPH_HEIGHT / 12)-5), $series_names[$j], $color_legend);

		
		$num = count($x_series[$j]);
		for ($i=0;$i<$num;$i++) {
			$x1 = MARGIN_LEFT - $OFFSET + (($H_WIDTH*$x_series[$j][$i])/($x_values[$x_num-1]-$x_values[0]));
   			$y1 = PLOT_BASE - GRAPH_HEIGHT*($y_series[$j][$i]/$y_values[$y_num-1]);
			imagefilledarc($image, $x1, $y1, 5, 5, 0, 360, $color[$j], 4);
			}
		}
		
	//prints X-Labels
	$num = count($x_values);
	$x2 = -($H_STEP);
	for ($i=0;$i<$num;$i++) {
		$x1 = MARGIN_LEFT - $OFFSET + (($H_WIDTH*$x_values[$i])/($x_values[$x_num-1]-$x_values[0]));
		if (($x1-$x2 ) >= $H_STEP) { 			
			imagestring($image, 2, $x1,  XLABEL_MARGIN_TOP, $x_values[$i], $color_legend);
			} else {
			$x1 = $x2;
			}
		$x2 = $x1;
		}		

	
	for ($i=0; $i<$series_num; $i++) {

    	$function_string = Graph::createLagrange($x_series[$i], $y_series[$i]);
    	$parsed_function = Graph::parse_function($function_string);

    	//plot the function
    	$n = count($x_series[$i])-1;
    	$old_x = $x_series[$i][0];
    	$old_y = $y_series[$i][0];
    	  	
    	$PLOT_H_STEP = 0.005*($x_series[$i][$n] - $x_series[$i][0]);
    	
    	for($x = $x_series[$i][0]; $x<$x_series[$i][$n]; $x += $PLOT_H_STEP) {
       		$y = "\$y = ".$parsed_function.";";
       		eval($y);
   			if ($x < $old_x) { $old_x = $x_series[$i][0]; $old_y = $y_series[$i][0];} 
   			$x_norm = MARGIN_LEFT - $OFFSET + (($H_WIDTH*$x)/($x_values[$x_num-1]-$x_values[0]));
   			$y_norm = max((PLOT_BASE - GRAPH_HEIGHT*($y/$y_values[$y_num-1])),(PLOT_BASE - GRAPH_HEIGHT));
   			$y_norm = min($y_norm,PLOT_BASE);

			//only plot from the second time on
   			if($old_x != $x_series[$i][0])
			imageline($image, $old_x_norm, $old_y_norm, $x_norm, $y_norm, $color[$i]);   
   			$old_x = $x;
   			$old_x_norm = $x_norm;
   			$old_y = $y;
   			$old_y_norm = $y_norm;   			
			} 
    	} 
    
	//set content type
	header("Content-type: image/jpeg");
	//send image
	Imagejpeg($image, '', 100);
	ImageDestroy($image);	
    	
 	}
 	
 	function createLagrange($x_vals, $y_vals) 
	{
    $num = count($x_vals);
    for ($i=0; $i<$num; $i++) {
		$y = "";
		for ($j=0; $j<$num; $j++) {
	    	if (!($i == $j)) { $y .= "((x-$x_vals[$j])/($x_vals[$i]-$x_vals[$j]))*"; }
	    	}
		$y .= $y_vals[$i];
		$z[$i] = $y;
		}
	$z = implode("+", $z);
	return $z;
	}
	
	function parse_function($in_string)
	{

    //convert all characters to PHP variables
    $out_string = ereg_replace("([a-zA-Z])", "$\\1", $in_string);

    //return result
    return ($out_string);

	}
	
	function plot($image, $x, $y, $i)
	{
    
    $color = imagecolorallocate($image, 200, 200, 200);  

    
    //set these as static to "remember" the last coordinates
    static $old_x = PLOT_MIN;
    static $old_y = 0;
    
    if ($x < $old_x) { $old_x = PLOT_MIN; $old_y = 0;}
    
    //only plot from the second time on
    if($old_x != PLOT_MIN)

	imageline($image, ($old_x / PLOT_STEP), (PLOT_BASE - $old_y), 
			  ($x / PLOT_STEP), (PLOT_BASE - $y ), $color);
    
    $old_x = $x;
    $old_y = $y;
    
	}

    function create_canvas($graph) 
	{	    
    $image = imagecreate(CANVAS_WIDTH, CANVAS_HEIGHT);
	//allocate canvas color
    $color_canvas = imagecolorallocate($image, $graph->canvas_color[r], $graph->canvas_color[g], $graph->canvas_color[b]);    
    $color_txt = imagecolorallocate($image, $graph->text_color[r], $graph->text_color[g], $graph->text_color[b]);    
	//clear image
    imagefilledrectangle($image, 0, 0, $width-1, $height-1, $color_canvas);
   	imagestring($image, 5, MARGIN_LEFT,  TITLE_MARGIN_TOP, $graph->title,  $color_txt);
    return $image;
    }
    
    function create_grid($image, $width, $step, $scale, $xname, $yname, $options) 
	{	
    $color_grid = imagecolorallocate($image, 200, 200, 200);
    $color_legend = imagecolorallocate($image, 100, 100, 100);
    for ($i=0; $i<=$width; $i+=$step) {
		imageline($image, MARGIN_LEFT + $i, GRAPH_MARGIN_TOP , MARGIN_LEFT + $i, PLOT_BASE, $color_grid);
		}
    for ($i=0; $i<=10; $i++) {
    	imagestring($image, 2, YLABEL_MARGIN_LEFT,  PLOT_BASE - (($i+0.5)*(GRAPH_HEIGHT / 10)), round(($i*$scale),$options['y-round']), $color_legend);
		imageline($image, MARGIN_LEFT, PLOT_BASE - ($i*(GRAPH_HEIGHT / 10)), MARGIN_LEFT + $width,  PLOT_BASE - ($i*(GRAPH_HEIGHT / 10)), $color_grid);
		}	
		
	imagestring($image, 3, YLABEL_MARGIN_LEFT,  GRAPH_MARGIN_TOP - 30, $yname,  $color_legend);
   	imagestring($image, 3, MARGIN_LEFT + (0.9*GRAPH_WIDTH),  XLABEL_MARGIN_TOP + 15, $xname,  $color_legend);
   	imagestring($image, 3, MARGIN_LEFT,  XLABEL_MARGIN_TOP, $options['x-labels']['alt'],  $color_legend);
   	imagestring($image, 3, MARGIN_LEFT,  XLABEL_MARGIN_TOP + 25, $options['x-labels']['extra'],  $color_legend);
		
	return $image;
	}  
	 
}  
?>