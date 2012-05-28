<?php       
session_start();
require_once 'phplot.php';
	$data = array(     
		array('正常文件',0),
		array('可疑文件',intval($_GET[countKeywords])),
		array('后门文件',intval($_GET[countVirus])),
	);	
$plot = new PHPlot(350,280);
$plot->SetTTFPath('./public');
$plot->SetDefaultTTFont('SIMHEI.TTF');
$plot->SetUseTTF(True);
	$plot->SetImageBorderType('plain');
	$plot->SetPlotType('pie');
	$plot->SetDataType('text-data-single');
	$plot->SetPlotBorderType('full');
	$plot->SetBackgroundColor('#ffffcc');
	$plot->SetDrawPlotAreaBackground(True);
	$plot->SetPlotBgColor('#ffffff');
	$plot->SetDataValues($data);
	$plot->SetDataColors(array('#73BF60', '#FFe500', '#FF3300'));
	$plot->SetTitle("当天新增文件分析[$_GET[countNew]]");
	$plot->SetTitleColor('#D9773A');
	foreach ($data as $row)
	$plot->SetLegend(implode(': ', $row));
	$plot->Setshading(15);
	$plot->SetDataBorderColors('black');
	$plot->DrawGraph();
?>
