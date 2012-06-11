<?php require "base.php"; ?>
<!DOCTYPE html>
<html>
	<head>
		<title>Falcon-安装程序</title>
		<meta charset="utf-8">
		<link rel="stylesheet" type="text/css" href="html/style.css" />
	</head>
	<body>
	<div class="wrap">
<?php

if (empty($_GET['step'])) {
	$_GET['step'] = 1;
} 

if ($_GET['step'] == 1) {
	/**
	 * // ------------------------------ -------- ------------------------------ //
	 * STEP 1
	 * // ------------------------------ -------- ------------------------------ //
	 */
	?><div class="step1"><?php

	echo "<b>请根据实际情况配置config.inc.php 文件.</b>";

	echo "<a href=\"install.php?step=2\">点击这里开始连接数据库 --></a>";
	?></div><?php
} // end step 1

if ($_GET['step'] == 2) {
	/**
	 * // ------------------------------ -------- ------------------------------ //
	 * STEP 2
	 * // ------------------------------ -------- ------------------------------ //
	 */
?><div class="step2"><?php
	echo "<b class=\"try\">开始尝试连接数据库...</b>"; 
	$DB_site = new DB_Sql_vb; 
	$DB_site -> database = $dbname;
	$DB_site -> server = $dbhost;
	$DB_site -> user = $dbuser;
	$DB_site -> password = $dbpass; 
	$DB_site -> reporterror = 0;
	$DB_site -> connect(); 
	$errno = $DB_site -> errno;
 
	if ($DB_site -> link_id != 0) {
		if ($errno != 0) {
				echo "<p class=\"error\">";
				if ($errno == 1049) {
					
					echo "<b>您指定的数据库不存在. 现在尝试创建它...</b>";
					$DB_site -> query("CREATE DATABASE $dbname");
					echo "<b>尝试再次连接...</b>";
					$DB_site -> select_db($dbname);
	
					$errno = $DB_site -> geterrno();
	
					if ($errno == 0) {
						echo "<b>连接成功!</b>";
						echo "</p>";
						echo "<a class=\"btn3\" href=\"install.php?step=3\">点击这里继续 -></a></p>";
					} else {
						echo "</p>";
						echo "<b class=\"error2\">再次连接失败! 请确定数据库和服务器配置正确后再次尝试.</b>";
						exit;
					} 
				 } else {
					echo "<p>连接失败: 数据库意外错误.</p>";
					echo "<p>错误号码: " . $DB_site -> errno . "</p>";
					echo "<p>错误描述: " . $DB_site -> errdesc . "</p>";
					echo "<p>请确定数据库和服务器配置正确后再次尝试.</p>";
					exit;
				 }
				 
			} else {
			// succeeded! yay!
			   echo "<b class=\"succuss\">连接成功! 数据库存在</b>";
			   echo "<a class=\"btn1\" href=\"install.php?step=3\">开始导入数据 --></a>";
			   echo "<a class=\"btn2\" href=\"install.php?step=3&reset=1\">点击这里清空数据库并导入数据 --></a>";
			} 
	} else {
	  echo "<p>点击这里继续. 请返回上一步确认您输入的所有资料都正确.</p>";
	  exit;
	} 
}
?></div>

<?php // end step 2
if ($_GET['step'] >= 3) {
	
	$DB_site = new DB_Sql_vb; 
	$DB_site -> database = $dbname;
	$DB_site -> server = $dbhost;
	$DB_site -> user = $dbuser;
	$DB_site -> password = $dbpass; 
	$DB_site -> connect(); 
} 

if ($_GET['step'] == 3) {
	echo "<div class=\"step3\">";
	
	/**
	 * // ------------------------------ -------- ------------------------------ //
	 * STEP 3
	 * // ------------------------------ -------- ------------------------------ //
	 */
echo "<div class=\"creat\">";
	if ($_GET['reset'] == 1) {
		echo "<p>正在重置数据库...";
		$result = $DB_site -> query("SHOW tables");
		while ($currow = $DB_site -> fetch_array($result)) {
			$DB_site -> query("DROP TABLE IF EXISTS $currow[0]");
		} 

		echo "已成功</p>";
	} 

	$DB_site -> reporterror = 0;

  $query[] = "CREATE TABLE $table1 (
`id` INT( 3 ) NOT NULL AUTO_INCREMENT PRIMARY KEY ,
`username` VARCHAR( 30 ) NOT NULL ,
`password` VARCHAR( 30 ) NOT NULL
) ENGINE = MYISAM ";

$explain[] = "创建 $table1 数据表";



$query[] = "CREATE TABLE $table2 (
`id` INT( 11 ) NOT NULL AUTO_INCREMENT PRIMARY KEY ,
`ip` VARCHAR( 40 ) NULL DEFAULT NULL ,
`content` VARCHAR( 100 ) NULL DEFAULT NULL ,
`source` TEXT( 65500 ) NULL DEFAULT NULL ,
`level` INT( 11 ) NOT NULL DEFAULT '1',
`remove` INT( 11 ) NOT NULL DEFAULT '0',
`date` DATETIME NULL DEFAULT NULL
) ENGINE = MYISAM 
";

$explain[] = "创建 $table2 数据表";

    $time= date('Y-m-d H:m:s');
	$query[] = "INSERT INTO $table2 VALUES (1,'127.0.0.1', '这是一条测试数据', 'this is sourcecode', '1','0','$time')"; 
	doqueries();
	echo "</div>";
	if ($DB_site -> errno != 0) {
		echo "<div class=\"err\">";
		echo "<p>安装数据表中脚本报告错误. 仅在您确定错误不严重的情况下继续下一步.</p>";
		echo "<p>错误为:</p>";
		echo "<p>错误号: " . $DB_site -> errno . "</p>";
		echo "<p>错误描述: " . $DB_site -> errdesc . "</p>";
		echo "</div>";
	} else {
		echo "<div class=\"done\"><p>数据表创建成功.</p>";
		
?>
	<form action="end.php" method="POST">
	 请输入管理员用户名：<input class="inputbox" type="text" name="username"><br>
	 &nbsp;&nbsp;请输入管理员密码： <input class="inputbox" type="password" name="password"><br>
     <input type="submit" class="btn4" name="login" value="下一步" >	 
	 </form>
     </div>
	<?php	
	} 
	
	echo "</div>";
} // end step 3

include "footer.php";
  ?>
</body>
</html>
