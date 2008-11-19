<?php
/********************************
Trace Debugger Library ver. 1.5

********************************/


// Activating trace debugging true/false

define('TRACE_DEBUGGING',true);

/* Defining path to debug log file - 

Example:
public_html/promerica/debug/

should be:
define(LOCAL_PATH,'/promerica/debug/');

*/
define('LOCAL_PATH','/app/mod_sponsorship/debug/');
// define(DEBUG_LOGFILE_PATH,$_SERVER['DOCUMENT_ROOT'] . LOCAL_PATH);

function trace($variable_to_trace,$eraseFlag=1,$label='default',$namespace='default') {
if(TRACE_DEBUGGING) {

// Defining log filename

$debug_logfile_path = $_SERVER['DOCUMENT_ROOT'].LOCAL_PATH.$namespace.'.'.'log2.txt';

// Getting data from the variable being debugged

if(is_array($variable_to_trace)) {

$data=print_r($variable_to_trace,true);}

elseif (is_object($variable_to_trace)) {$data=print_r(get_object_vars($variable_to_trace),true);}

elseif (is_bool($variable_to_trace)) {

    $data=var_export($variable_to_trace,true);
} 


elseif (is_resource($variable_to_trace)) {

    $data=var_export(get_resource_type($variable_to_trace),true);
}

else {
 
 $data=var_export($variable_to_trace,true);
 
}

//delete existing log file to avoid browsing older logs
if ($eraseFlag) {
if(file_exists($debug_logfile_path)) {
unlink($debug_logfile_path);
}
}

// find caller data (file,line)

$location = debug_backtrace();

		while($d = array_pop($location)) { $count++;
			if ((strToLower($d['function']) == 'trace') || (strToLower(@$d['class']) == 'trace')) {
				break; echo 'estoy aqui';
				}
}


// Formatting log output

if (is_scalar($variable_to_trace)) {


if (isset($d['file'])) {
$output.="file:".$d['file']." >";
}

if (isset($d['line'])) {
$output.="line:".$d['line']." >";
}


if (isset($d['function'])) {
$output.="function:".$d['function']." >";
}


if (isset($d['class'])) {
$output.="class:".$d['class']." >";
}

if (isset($d['method'])) {
$output.="method:".$d['method']." >";
}

$output.="$label:$data"."\n";

} else {
$output.= "=============***================\n";
$output.="levels:".$count."\n";
if (isset($d['file'])) {
$output.="file:".$d['file']."\n";
}

if (isset($d['line'])) {
$output.="line:".$d['line']."\n";
}


if (isset($d['function'])) {
$output.="function:".$d['function']."\n";
}


if (isset($d['class'])) {
$output.="class:".$d['class']."\n";
}

if (isset($d['method'])) {
$output.="method:".$d['method']."\n";
}


// if (isset($location['label'])) {
// 
// $label = $location['label'];} else {
// 
// $label = $namespace;
// 
// }




$output.= "$label:$data\n";
$output.= date("Y-m-d H:i:s")."\n";
$output.= "======********************======\n";
}


error_log($output,3,$debug_logfile_path);
}
}

?>