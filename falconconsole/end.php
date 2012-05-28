<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8" />
<title>Falcon Installer</title>
<link rel="stylesheet" type="text/css" href="html/style.css" />
</head>
<body>
	<div class="wrap">
    	<div class="step4">
		<?php
            require "base.php";
            if(isset($_POST['login'])&&ctype_alnum($_POST['username'])){
            $username = empty($_POST['username'])?'':trim($_POST['username']);
            $password = empty($_POST['password'])?'':trim($_POST['password']);
            $sql = "insert into $table1(username,password)values('$username','$password')";
            $DB_site ->query($sql);
            if(mysql_affected_rows()>0){
            $_SESSION['username'] = $sid;
			echo "<b>恭喜，安装成功！</b>";
            echo "<a class=\"btn4\" href='login.php'>安装成功，请点击登录</a>";
            }
        }
		?>
        </div> 
        <?php
		include "footer.php" ;
        ?>
	</div>
</body>
</html>
