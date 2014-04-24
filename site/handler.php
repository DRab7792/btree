<?php 
$file = "/var/www/btree/site/webInstr";
$instructions = (file_get_contents("php://input"))? file_get_contents("php://input"):"quit" ;
if (function_exists("file_put_contents")){
	$write_file = (file_put_contents($file, $instructions)==false)? "error": "all good";
}else{
	$write_file = "Function not available";
}
exec ("./btree webInstr jsonRes");
$res = file_get_contents("jsonRes");
echo $res;
?>
