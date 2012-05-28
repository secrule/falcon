<?php
     session_start();
     require_once('base.php');
     require_once('vemplator.php');	 
    	$t = new vemplator();
	$DB_site = new DB_Sql_vb; 
	$DB_site -> database = $dbname;
	$DB_site -> server = $dbhost;
	$DB_site -> user = $dbuser;
	$DB_site -> password = $dbpass;
	$DB_site -> reporterror = 0;
	$DB_site -> connect();
	$error = $DB_site -> errno;
 
	$t = new vemplator();
	
    if(isset($_POST['submit'])&&ctype_alnum($_POST['username'])){			  
		  $username = empty($_POST['username'])?'':trim($_POST['username']);
		  $password = empty($_POST['password'])?'':trim($_POST['password']);
		  $sql = "select username,password from $table1 where username='$username'and password='$password'";
		  $DB_site -> query($sql);
		  if($DB_site -> num_rows()){
		  $sid = session_id();
		  $_SESSION['username'] = $sid;
	      header('Location: index.php');
		  }else{
				$message = "登录失败,密码或用户名错！";
			    $t->assign('message',$message);
		  }	  
      }
    echo $t->output('html/login.html');             
	include "footer.php" ;
?>
</div>
</body>
</html>
