<?php       
session_start();
require_once 'phplot.php';
echo $_GET[countKeywords];
$data = array(     
		array('新增文件',intval($_GET[countnew])),
		array('删除文件',intval($_GET[countdel])),
		array('修改文件',intval($_GET[countmodify])),
	);	
$plot = new PHPlot(350,280);
$plot->SetTTFPath('./public');
$plot->SetDefaultTTFont('SIMHEI.TTF');
$plot->SetUseTTF(True);
	$plot->SetImageBorderType('plain');
	$plot->SetPlotType('bars');
	$plot->SetDataType('text-data');
	$plot->SetPlotBorderType('full');
	$plot->SetBackgroundColor('#ffffcc');
	$plot->SetDrawPlotAreaBackground(True);
	$plot->SetPlotBgColor('#ffffff');
	$plot->SetDataValues($data);
	$plot->SetTitle("新增文件数:$_GET[countnew] 删除文件数:$_GET[countdel] 修改文件数:$_GET[countmodify]");
	$plot->SetTitleColor('#D9773A');
	foreach ($data as $row)
	$plot->Setshading(10);
	$plot->SetDataBorderColors('black');
	$plot->DrawGraph();
?>
