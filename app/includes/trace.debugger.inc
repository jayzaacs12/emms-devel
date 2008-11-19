<?php

//trace.debugger.inc ver. 1.2 2007-08-05
// Sample usage:
//trace(WEBPAGE::$scr_name,__FILE__,__LINE__,__FUNCTION__,__CLASS__,__METHOD__,"label",1);

// $eraseFlag = 1 (delete log file before write), $eraseFlag = 0 (append to log file - do not delet it) 


define(TRACE_DEBUGGING,true);
define(LOCAL_PATH,'/debug/');
define(DEBUG_LOGFILE_PATH,$_SERVER['DOCUMENT_ROOT'] . LOCAL_PATH . 'log2.txt');

function trace($variable_to_trace,$file=__FILE__,$line=__LINE__,$function=__FUNCTION__,$class=__CLASS__,$method=__METHOD__,$label="default",$eraseFlag=1) {

if(TRACE_DEBUGGING) {

if(is_array($variable_to_trace)) {

$variable_to_trace=print_r($variable_to_trace,true);}

elseif (is_object($variable_to_trace)) {$variable_to_trace=print_r($variable_to_trace,true);}



//delete existing log file to avoid browsing older logs
if ($eraseFlag) {
if(file_exists(DEBUG_LOGFILE_PATH)) {
unlink(DEBUG_LOGFILE_PATH);
}
}

$output.= "[1]=============================\n";
$output.="file:$file\n";
$output.="line:$line\n";
$output.="function:$function\n";
$output.="class:$class\n";
$output.="method:$method\n";
$output.= "$label:$variable_to_trace\n";
$output.= date("Y-m-d H:i:s")."\n";
$output.= "=============================\n";

error_log($output,3,DEBUG_LOGFILE_PATH);
}
}





function ftrace($variable_to_trace,$label='default',$eraseFlag=1) {

if(TRACE_DEBUGGING) {

if(is_array($variable_to_trace)) {

$variable_to_trace=print_r($variable_to_trace,true);}

elseif (is_object($variable_to_trace)) {$variable_to_trace=print_r($variable_to_trace,true);}



//delete existing log file to avoid browsing older logs

if ($eraseFlag) {
if(file_exists(DEBUG_LOGFILE_PATH)) {
unlink(DEBUG_LOGFILE_PATH);
}
}


$output.="[2]\n";
$output.= "$label:$variable_to_trace\n";
$output.= date("Y-m-d H:i:s")."\n";

error_log($output,3,DEBUG_LOGFILE_PATH);
}
}

function nftrace($variable_to_trace,$eraseFlag=1,$namespace='default',$label='default') {

$debug_logfile_path = $_SERVER['DOCUMENT_ROOT'].LOCAL_PATH.$namespace.'.'.'log2.txt';

if(TRACE_DEBUGGING) {

if(is_array($variable_to_trace)) {

$variable_to_trace=print_r($variable_to_trace,true);}

elseif (is_object($variable_to_trace)) {$variable_to_trace=print_r($variable_to_trace,true);}



//delete existing log file to avoid browsing older logs

if ($eraseFlag) {
if(file_exists($debug_logfile_path)) {
unlink($debug_logfile_path);
}
}


$output.="[2]\n";
$output.= "$label:$variable_to_trace\n";
$output.= date("Y-m-d H:i:s")."\n";

error_log($output,3,$debug_logfile_path);

}
}

?>