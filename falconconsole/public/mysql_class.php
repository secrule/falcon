<?php
error_reporting(7);
class DB_Sql_vb {
	public $database = "";

	public $link_id = 0;
	public $query_id = 0;
	public $record = array();

	public $errdesc = "";
	public $errno = 0;
	public $reporterror = 1;

	public $server = "";
	public $user = "";
	public $password = "";

	function connect() {
		// connect to db server
		if (0 == $this -> link_id) {
			if ($this -> password == "") {
				$this -> link_id = mysql_connect($this -> server, $this -> user);
			} else {
				$this -> link_id = mysql_connect($this -> server, $this -> user, $this -> password);
			} 
			if (!$this -> link_id) {
				$this -> halt("Link-ID == 错误, 连接失败");
			} 
			if ($this -> database != "") {
				if (!mysql_select_db($this -> database, $this -> link_id)) {
					$this -> halt("无法使用数据库 " . $this -> database);
				} 
			} 
		} 
	} 

	public function geterrdesc() {
		$this -> error = mysql_error();
		return $this -> error;
	} 

	public function geterrno() {
		$this -> errno = mysql_errno();
		return $this -> errno;
	} 

	public function select_db($database = "") {
		// select database
		if ($database != "") {
			$this -> database = $database;
		} 

		if (!mysql_select_db($this -> database, $this -> link_id)) {
			$this -> halt("无法使用数据库 " . $this -> database);
		} 
	} 

	public function query($query_string) {
		global $query_count, $showqueries, $explain, $querytime; 
		// do query
		if ($showqueries) {
			echo "查询: $query_string\n";

			global $pagestarttime;
			$pageendtime = microtime();
			$starttime = explode(" ", $pagestarttime);
			$endtime = explode(" ", $pageendtime);

			$beforetime = $endtime[0] - $starttime[0] + $endtime[1] - $starttime[1];

			echo "早于时间: $beforetime\n";
		} 

	 $this -> query_id = mysql_query($query_string, $this -> link_id);
		if (!$this -> query_id) {
			$this -> halt("无效 SQL: " . $query_string);
		} 

		$query_count++;

		if ($showqueries) {
			$pageendtime = microtime();
			$starttime = explode(" ", $pagestarttime);
			$endtime = explode(" ", $pageendtime);

			$aftertime = $endtime[0] - $starttime[0] + $endtime[1] - $starttime[1];
			$querytime += $aftertime - $beforetime;

			echo "晚于时间: $aftertime\n";

			if ($explain and substr(trim(strtoupper($query_string)), 0, 6) == "SELECT") {
				$explain_id = mysql_query("EXPLAIN $query_string", $this -> link_id);
				echo "</pre>\n";
				echo "
 <table width=100% border=1 cellpadding=2 cellspacing=1>
 <tr>
 <td><b>表</b></td>
 <td><b>类型</b></td>
 <td><b>可能字段</b></td>
 <td><b>字段</b></td>
 <td><b>字段长度</b></td>
 <td><b>属性</b></td>
 <td><b>行</b></td>
 <td><b>额外</b></td>
 </tr>\n";
				while ($array = mysql_fetch_array($explain_id)) {
					echo "
 <tr>
 <td>$array[table]&nbsp;</td>
 <td>$array[type]&nbsp;</td>
 <td>$array[possible_keys]&nbsp;</td>
 <td>$array[key]&nbsp;</td>
 <td>$array[key_len]&nbsp;</td>
 <td>$array[ref]&nbsp;</td>
 <td>$array[rows]&nbsp;</td>
 <td>$array[Extra]&nbsp;</td>
 </tr>\n";
				} 
				echo "</table>\n<BR><hr>\n";
				echo "\n<pre>";
			} else {
				echo "\n<hr>\n\n";
			} 
		} 

		return $this -> query_id;
	} 

	public function fetch_array($query_id = -1, $query_string = "") {
		// retrieve row
		if ($query_id != -1) {
			$this -> query_id = $query_id;
		} 
		if (isset($this -> query_id)) {
			$this -> record = mysql_fetch_array($this -> query_id);
		} else {
			if (!empty($query_string)) {
				$this -> halt("无效查询ID (" . $this -> query_id . ") 在此查询: $query_string");
			} else {
				$this -> halt("无效查询ID " . $this -> query_id . " 已指定");
			} 
		} 

		return $this -> record;
	} 

	public function free_result($query_id = -1) {
		// retrieve row
		if ($query_id != -1) {
			$this -> query_id = $query_id;
		} 
		// return @mysql _free_result($this->query_id);
	} 

	public function query_first($query_string) {
		// does a query and returns first row
		$query_id = $this -> query($query_string);
		$returnarray = $this -> fetch_array($query_id, $query_string);
		$this -> free_result($query_id);
		return $returnarray;
	} 

	protected function data_seek($pos, $query_id = -1) {
		// goes to row $pos
		if ($query_id != -1) {
			$this -> query_id = $query_id;
		} 
		return mysql_data_seek($this -> query_id, $pos);
	} 
    public function num_rows($query_id = -1) {
		// returns number of rows in query
		if ($query_id != -1) {
			$this -> query_id = $query_id;
		} 
		return mysql_num_rows($this -> query_id);
	} 

	public function num_fields($query_id = -1) {
		// returns number of fields in query
		if ($query_id != -1) {
			$this -> query_id = $query_id;
		} 
		return mysql_num_fields($this -> query_id);
	} 

	public  function insert_id() {
		// returns last auto_increment field number assigned
		return mysql_insert_id($this -> link_id);
	} 

	public function halt($msg) {
		$this -> errdesc = mysql_error();
		$this -> errno = mysql_errno(); 
		// prints warning message when there is an error
		global $technicalemail;

		if ($this -> reporterror == 1) {
			$message = "数据库错误 $this->appname: $msg\n";
			$message .= "MySQL 错误: $this->errdesc\n";
			$message .= "MySQL 错误号: $this->errno\n";
			$message .= "日期: " . date("l dS of F Y h:i:s A") . "\n";
			$message .= "脚本: " . getenv("REQUEST_URI") . "\n";
			$message .= "附注: " . getenv("HTTP_REFERER") . "\n";

			echo "\n<!-- $message -->\n";

			echo "</td></tr></table>\n<p>数据库好像出现了一个小问题.\n";
			echo "请按浏览器的后退按钮返回重试.</p>";
			echo "如果问题依然存在，请联系我们.</p>";
			die("");
		} 
	} 
} 
   function doqueries() {
	global $DB_site, $query, $explain, $onvservers;
	while (list($key, $val) = each($query)) {
		echo "<p>$explain[$key]</p>\n";
		echo "<!-- " . htmlspecialchars($val) . " -->\n\n";
		flush();
		if ($onvservers == 1 and substr($val, 0, 5) == "ALTER") {
			$DB_site -> reporterror = 0;
		} 
		$DB_site -> query($val);
		if ($onvservers == 1 and substr($val, 0, 5) == "ALTER") {
			$DB_site -> errno = 0;
			$DB_site -> link_id = 0;
			@mysql_close();
			$DB_site -> connect();
			if ($_GET['step'] != 4) {
				$DB_site -> reporterror = 1;
			} 
		} 
	} 

	unset ($query);
	unset ($explain);
} 

?>
