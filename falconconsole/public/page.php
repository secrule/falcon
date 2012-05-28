<?php
class PageClass {
	public $myde_count; //总记录数
	public $myde_size; //每页记录数
	public $myde_page; //当前页
	public $myde_page_count; //总页数
	public $page_url; //页面url
	public $page_i; //起始页
	public $page_ub; //结束页
	public $page_limit;

	function __construct($myde_count = 0, $myde_size = 1, $myde_page = 1, $page_url) { // 构造函数
		$this -> myde_count = $this -> numeric($myde_count);
		$this -> myde_size = $this -> numeric($myde_size);
		$this -> myde_page = $this -> numeric($myde_page);
		$this -> page_limit = ($this -> myde_page * $this -> myde_size) - $this -> myde_size; //下一页的开始记录
		$this -> page_url = $page_url; //连接的地址  
		// echo "111111111".$this -> page_url;
		if ($this -> myde_page < 1)$this -> myde_page = 1; //当前页小于1的时候，，值赋值为1
		if ($this -> myde_count < 0)$this -> myde_page = 0;
		$this -> myde_page_count = ceil($this -> myde_count / $this -> myde_size); //总页数  
		if ($this -> myde_page_count < 1)
			$this -> myde_page_count = 1;
		if ($this -> myde_page > $this -> myde_page_count)
			$this -> myde_page = $this -> myde_page_count; 
		// $this->page_i=$this->myde_page-2;
		$this -> page_i = $this -> myde_page-2;
		$this -> page_ub = $this -> myde_page + 2; 
		// $this->page_ub=$this->myde_page+2;
		if ($this -> page_i < 1)$this -> page_i = 1;
		if ($this -> page_ub > $this -> myde_page_count) {
			$this -> page_ub = $this -> myde_page_count;
		} 
	} 
	private function numeric($id) { // 判断是否为数字
		if (strlen($id)) {
			if (!ereg("^[0-9]+$", $id)) $id = 1;
		} else {
			$id = 1;
		} 
		return $id;
	} 
	private function page_replace($page) { // 地址替换
		return str_replace("{page}", $page, $this -> page_url);
	} 
	private function myde_home() { // 首页
		if ($this -> myde_page != 1) {
			// echo $this -> myde_page;
			$test = "<li class=\"list\"><a href=\"" . $this -> page_replace(1) . "\" title=\"首页\" ><span class=\"rbg\">首页</span></a></li>\n";
			return $test;
		} else {
			return "<li class=\"list\"><span class=\"rbg\">首页</span></li>\n";
		} 
	} 
	private function myde_prev() { // 上一页
		if ($this -> myde_page != 1) {
			// echo $this -> myde_page;
			return "<li class=\"list\"><a href=\"" . $this -> page_replace($this -> myde_page-1) . "\" title=\"上一页\" ><span class=\"rbg\">上一页</span></a></li>\n";
		} else {
			return "<li class=\"list\"><span class=\"rbg\">上一页</span></li>\n";
		} 
	} 
	private function myde_next() { // 下一页
		if ($this -> myde_page != $this -> myde_page_count) {
			// echo "下一页".($this -> myde_page)+1;
			// echo $this -> myde_page_count;
			// echo $this -> page_replace($this -> myde_page + 1);
			return "<li class=\"list\"><a href=\"" . $this -> page_replace($this -> myde_page + 1) . "\" title=\"下一页\" ><span class=\"rbg\">下一页</span></a></li>\n";
		} else {
			return " <li class=\"list\"><span class=\"rbg\">下一页</span></li>\n";
		} 
	} 
	private function myde_last() { // 尾页
		if ($this -> myde_page != $this -> myde_page_count) {
			// echo $this -> myde_page;
			return "<li class=\"list\"><a href=\"" . $this -> page_replace($this -> myde_page_count) . "\" title=\"尾页\" ><span class=\"rbg\">尾页</span></a></li>\n";
		} else {
			return "    <li class=\"list\"><span class=\"rbg\">尾页</span></li>\n";
		} 
	} 
	function myde_write($id = 'page') { // 输出
		$str = "<div id=\"" . $id . "\" class=\"page\">\n <ul>\n ";
		$str .= " <li class=\"list\"><span class=\"rbg\">总记录:<b>" . $this -> myde_count . "</b></span></li>\n";
		$str .= "    <li class=\"list\"><span class=\"rbg\"><b>" . $this -> myde_page . "</b>/" . $this -> myde_page_count . "</span></li>\n";
		$str .= $this -> myde_home();
		$str .= $this -> myde_prev();
	/*
		for($page_for_i = $this -> page_i;$page_for_i <= $this -> page_ub; $page_for_i++) {
			if ($this -> myde_page == $page_for_i) {
				$str .= "<li class=\"list\"><span class=\"rbg\"><b>" . $page_for_i . "</b></span></li>\n";
			} else {
				$str .= "<li class=\"page_a\"><a href=\"" . $this -> page_replace($page_for_i) . "\" title=\"第" . $page_for_i . "页\">";
				$str .= $page_for_i . "</a></li>\n";
			} 
		}
	      */	
		
		$str .= $this -> myde_next();
		$str .= $this -> myde_last();
		$str .= "<li class=\"pages_input\"><input type=\"text\" value=\"" . $this -> myde_page . "\"";
		$str .= " onmouseover=\"javascript:this.value='';this.focus();\" onkeydown=\"javascript: if(event.keyCode==13){ location='";
		$str .= $this -> page_replace("'+this.value+'") . "';return false;}\" ";
		$str .= " /><span>输入页码回车跳转</span></li>";
		$str .= " </ul></div>";
		return $str;
	} 
} 

?>
