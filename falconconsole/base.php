<?php
    require_once('./public/config.inc.php');
    require_once('./public/mysql_class.php');
    require_once('./public/page.php');
    date_default_timezone_set("PRC");
	
	$DB_site = new DB_Sql_vb; 
	$DB_site -> database = $dbname;
	$DB_site -> server = $dbhost;
	$DB_site -> user = $dbuser;
	$DB_site -> password = $dbpass;
	$DB_site -> reporterror = 0;
	$DB_site -> connect();
	$error = $DB_site -> errno;
?>
