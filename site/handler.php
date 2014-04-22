<?php 
$executable = "btree";
$result = array("action"=>array("success"=>"Invalid command"));
$instruction = (isset($_POST['str'])) ? $_POST['str'] : null;
$instrArr = ($instruction) ? preg_split('#\s+#', $instruction, null, PREG_SPLIT_NO_EMPTY) : null;
$codes = array("new", "run", "insert", "delete", "select", "quit");
// echo $instrArr[0];
$instrCode = (array_search($instrArr[0], $codes)) ? $instrArr[0] : false;
$instrArg = ($instrCode!="quit" && $instrCode) ? $instrArr[1] : "";
$action = (isset($_POST['action'])) ? $_POST['action'] : null;
if ($action=="start"){
	//$res = exec("./"+$executable+" 60000");
	$res = true;
	$result["action"]["success"] = $res;
}else if($instrCode!=false){
	// try {
	// 	$res = exec($instrCode+" "+$instrArg);
	// } catch (Exception $e) {
		$res = ($instrCode." ".strval($instrArg));
	// }
	$result["action"]["success"] = $res;
	// try {
	// 	$res = exec("getJson");
	// } catch (Exception $e) {
		$res = '[
	        {
	          "keys": ["32", "64", "128"], 
	          "children": [
	            {
	              "keys": ["16", "24", "30"],
	              "children": []
	            }
	          ]
	        }
	      ]';
	// }
	$result["action"]["result"] = json_decode($res);
}
echo json_encode($result);
?>