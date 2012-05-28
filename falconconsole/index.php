<?php
/**
* Falcon-Web Server File Monitoring Program Under Linux Environment
* Copyright (C) 2012 Secrule.com
* 
* Licensed under the terms of the GNU General Public License:
* http://www.opensource.org/licenses/gpl-license.php
* @license  GPLv3
*  
* @version  0.1
* @author   
* @modify   2012/5/28, @VinDong
 */
session_start();
ob_start();
$sid = session_id();
if(isset($_SESSION['username']) && ($_SESSION['username']==$sid)){
require_once('base.php');
include('vemplator.php');
include('html/top.html');
include('html/left.html'); 
include('html/index.html');

$connect = mysql_connect($dbhost, $dbuser, $dbpass) or die ("数据库连接失败");
 $t = new vemplator();
 ?>
<div class="main">
    	<div class="main_top"></div>
		<div id="main">
<?php
 if(trim($_GET['action'] == 'vk')){
/*
	
   	        $ntime = date('Y-m-d');
	        $sql = "select countVK from $table3 where date like '%$ntime%'";
			$DB_site -> query($sql);
			$row = $DB_site -> fetch_array();
		    $countvk = $row[0];    
		    $t = new vemplator();
            $t->assign('countvk',$countvk);
			echo $t->output('html/vk.html');
*/
}else if (trim($_GET['action']) == 'virus'){ 
    
   	        $ntime = date('Y-m-d');
	        $sql = "select count(id) from $table2 where date like '%$ntime%' and content like '%发现后门文件%'"; 
	        $DB_site -> query($sql);
			$row = $DB_site -> fetch_array();
		$countvirus = $row[0];
		$sqlvirus = "select id,ip,content,source,level,remove,date from $table2 where date like '%$ntime%' and content like '%后门文件%'";
		$DB_site -> query($sqlvirus);
		while ($rowvirus = $DB_site -> fetch_array()) {
				if ($rowvirus['level']==1){
					$newlevel = "低";
				}elseif($rowvirus['level'] == 2){
					$newlevel = "中";
				}elseif($rowvirus['level'] == 3){
					$newlevel = "高";
				}else{
					$newlevel = "NULL";
				}

				if ($rowvirus['remove'] == 0){
					$newremove = "未隔离";
				}elseif ($rowvirus['remove'] == 1){
					$newremove = "已隔离";
				}else {
					$newremove = "NULL";
				}
				$rowsvirus[] = array("id" =>$rowvirus['id'],
				"ip" => trim($rowvirus['ip']),
				"content" => $rowvirus['content'],
				"source" => trim($rowvirus['source']),
				"level" => $newlevel,
				"remove" => $newremove,
				"date" => $rowvirus['date']
				 );
		}
		if (is_array($rowsvirus)){
		$t = new vemplator();
		$t->assign('all',$rowsvirus);
            	$t->assign('countvirus', $countvirus);
		echo $t->output('html/virus.html');
		}else {
			echo $t->output('html/nodata.html');
		}
}else if(trim($_GET['action']) == 'keyword'){
   	        $ntime = date('Y-m-d');
	        $sql = "select count(id) from $table2 where date like '%$ntime%' and content like '%发现新增可疑文件%'";
			$DB_site -> query($sql);
			$row = $DB_site -> fetch_array();
			$countkeywords = $row[0];
		$sqlkeyword = "select id,ip,content,source,level,remove,date from $table2 where date like '%$ntime%' and content like '%可疑文件%'";
		$DB_site -> query($sqlkeyword);
		while ($rowkeyword = $DB_site -> fetch_array()) {
				if ($rowkeyword['level']==1){
					$newlevel = "低";
				}elseif($rowkeyword['level'] == 2){
					$newlevel = "中";
				}elseif($rowkeyword['level'] == 3){
					$newlevel = "高";
				}else{
					$newlevel = "NULL";
				}

				if ($rowkeyword['remove'] == 0){
					$newremove = "未隔离";
				}elseif ($rowkeyword['remove'] == 1){
					$newremove = "已隔离";
				}else {
					$newremove = "NULL";
				}
				$rowskeyword[] = array("id" =>$rowkeyword['id'],
				"ip" => trim($rowkeyword['ip']),
				"content" => $rowkeyword['content'],
				"source" => trim($rowkeyword['source']),
				"level" => $newlevel,
				"remove" => $newremove,
				"date" => $rowkeyword['date']
				 );
		}
		if (is_array($rowskeyword)){
		$t = new vemplator();
		$t->assign('all',$rowskeyword);
            	$t->assign('countkeyword', $countkeywords);
		echo $t->output('html/keyword.html');
		}else {
			echo $t->output('html/nodata.html');
		}

}else if(trim($_GET['action']) == 'countdel'){
		$ntime = date('Y-m-d');
		$sql = "select count(id) from $table2 where date like '%$ntime%' and content like '%文件被删除%'";
		$DB_site -> query($sql);
			$row = $DB_site -> fetch_array();
			$countdel = $row[0];
		$sqlcountdel = "select id,ip,content,source,level,remove,date from $table2 where date like '%$ntime%' and content like '%文件被删除%'";
		$DB_site -> query($sqlcountdel);
		while ($rowcountdel = $DB_site -> fetch_array()) {
				if ($rowcountdel['level']==1){
					$newlevel = "低";
				}elseif($rowcountdel['level'] == 2){
					$newlevel = "中";
				}elseif($rowcountdel['level'] == 3){
					$newlevel = "高";
				}else{
					$newlevel = "NULL";
				}

				if ($rowcountdel['remove'] == 0){
					$newremove = "未隔离";
				}elseif ($rowcountdel['remove'] == 1){
					$newremove = "已隔离";
				}else {
					$newremove = "NULL";
				}
				$rowscountdel[] = array("id" =>$rowcountdel['id'],
				"ip" => trim($rowcountdel['ip']),
				"content" => $rowcountdel['content'],
				"source" => trim($rowcountdel['source']),
				"level" => $newlevel,
				"remove" => $newremove,
				"date" => $rowcountdel['date']
				 );
		}
		if (is_array($rowscountdel)){
		$t = new vemplator();
		$t->assign('all',$rowscountdel);
            	$t->assign('countdel', $countdel);
		echo $t->output('html/delete.html');
		}else {
			echo $t->output('html/nodata.html');
		}
}else if(trim($_GET['action']) == 'countmodify'){
		$ntime = date('Y-m-d');
		$sql = "select count(id) from $table2 where date like '%$ntime%' and content like '%文件被修改%'";
		$DB_site -> query($sql);
			$row = $DB_site -> fetch_array();
			$countmodify = $row[0];
		$sqlcountmodify = "select id,ip,content,source,level,remove,date from $table2 where date like '%$ntime%' and content like '%文件被修改%'";
		$DB_site -> query($sqlcountmodify);
		while ($rowcountmodify = $DB_site -> fetch_array()) {
				if ($rowcountmodify['level']==1){
					$newlevel = "低";
				}elseif($rowcountmodify['level'] == 2){
					$newlevel = "中";
				}elseif($rowcountmodify['level'] == 3){
					$newlevel = "高";
				}else{
					$newlevel = "NULL";
				}

				if ($rowcountmodify['remove'] == 0){
					$newremove = "未隔离";
				}elseif ($rowcountmodify['remove'] == 1){
					$newremove = "已隔离";
				}else {
					$newremove = "NULL";
				}
				$rowscountmodify[] = array("id" =>$rowcountmodify['id'],
				"ip" => trim($rowcountmodify['ip']),
				"content" => $rowcountmodify['content'],
				"source" => trim($rowcountmodify['source']),
				"level" => $newlevel,
				"remove" => $newremove,
				"date" => $rowcountmodify['date']
				 );
		}
		if (is_array($rowscountmodify)){
		$t = new vemplator();
		$t->assign('all',$rowscountmodify);
            	$t->assign('countmodify', $countmodify);
		echo $t->output('html/modify.html');
		}else {
			echo $t->output('html/nodata.html');
		}

}else if(trim($_GET['action']) =='count'){ 
	$ntime = date('Y-m-d');

	$sql1 = "select count(id) from $table2 where date like '%$ntime%' and content like '%文件被删除%'";
	$DB_site -> query($sql1);
	$row1 = $DB_site -> fetch_array();
	$countdel = $row1[0];

	$sql2 = "select count(id) from $table2 where date like '%$ntime%' and content like '%文件被修改%'";
	$DB_site -> query($sql2);
	$row2 = $DB_site -> fetch_array();
	$countmodify = $row2[0];

        $sql3 = "select count(id) from $table2 where date like '%$ntime%' and content like '%发现后门文件%'";
        $DB_site -> query($sql3);
	$row3 = $DB_site -> fetch_array();
	    //var_dump($row);
	// $countNew = is_null($row['countNew'])? 0 : $row['countNew'];
	// $countKeywords = is_null($row['countKeywords'])? 0 : $row['countKeywords'];
	 $countVirus = $row3[0];

	$sql4 = "select count(id) from $table2 where date like '%$ntime%' and content like '%发现新增可疑文件%'";
	$DB_site -> query($sql4);
	$row4 = $DB_site -> fetch_array();
	$countKeywords = $row4[0];

	 $t = new vemplator();
	 $t->assign('countdel',$countdel);
	 $t->assign('countmodify',$countmodify);
	// $t->assign('countnew',$countNew);
	 $t->assign('countvirus',$countVirus);
	 $t->assign('countkeyword',$countKeywords);
	echo $t->output('html/count.html');
}else if(trim($_GET['action']) =='exit'){
			 $_SESSION = array();
			 session_destroy();
		     setcookie('PHPSESSID','',time()-3600,'/','',0,0);
			 header('Location: login.php');
			 ob_end_flush();
			 
}else{ 
			
			$page = $_GET['page']?intval($_GET['page']):"1";
			$sql = "SELECT `id` FROM $table2";
			mysql_query("set names 'utf-8'");
			$DB_site -> query($sql);
			$query = mysql_db_query($dbname, $sql);
		    $totail = $DB_site->num_rows();
			$number = $pagenumber; 
			$my_page = new PageClass($totail, $number, $_GET['page'], '?page={page}');
			$sql = "SELECT * FROM $table2 order by `id` DESC LIMIT " . $my_page -> page_limit . "," . $my_page -> myde_size;
			$DB_site -> query($sql);
			while ($row = $DB_site ->fetch_array()) {
				if ($row['level']==1){
					$newlevel = "低";
				}elseif($row['level'] == 2){
					$newlevel = "中";
				}elseif($row['level'] == 3){
					$newlevel = "高";
				}else{
					$newlevel = "NULL";
				}

				if ($row['remove'] == 0){
					$newremove = "未隔离";
				}elseif ($row['remove'] == 1){
					$newremove = "已隔离";
				}else {
					$newremove = "NULL";
				}
			$rows[] = array("id" =>$row['id'],
						  "ip" => trim($row['ip']),
						  "content" => $row['content'],
						  "source" => trim($row['source']),
						  "level" => $newlevel,
						  "remove" => $newremove,
						  "date" => $row['date']
						  );
			}
			$pag = $my_page -> myde_write();
			$t->assign('all',$rows);
			$t->assign('page',$pag);
			echo $t->output('html/main.html');
   
 }
}else{
		header("Location: login.php");
}
?>
</div>
 <div class="main_bottom"></div>
	</div>
<script type="text/javascript">
function showdiv(id){
	var dd=document.getElementById(id);
	if(dd.style.display=='none'){
		dd.style.display='';
	}else{
		dd.style.display='none';
	}
}
window.onload=function(){
	try{
		var HH;
		if(document.getElementById("main").clientHeight == 0){
			HH=document.getElementById("main").offsetHeight;
		}else{
			HH=document.getElementById("main").clientHeight;
		}
		if(HH<document.getElementById("menu").clientHeight){
			document.getElementById("main").style.height=document.getElementById("menu").offsetHeight-26+"px";
		}
	}catch(e){
	}
}
</script>
<?php
include "footer.php" ;
?>
</body>
</html>
