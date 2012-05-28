	<h3><font color=#D9773A>当天没有数据</font></h3>
	<table class="mtable" width="100%" border="0" cellspacing="0" cellpadding="0">
        	<thead>
            <tr>
                <td width="102">ID</td>
                <td width="140">服务器IP</td>
                <td width="130">报警内容</td>
                <td width="80">危险等级</td>
                <td width="80">处理结果</td>
                <td width="130">监控时间</td>
                <td>文件详情</td>
            </tr>
            </thead>
			<tr>
				   <td class="ftd"><?php echo $this->data->rowsvirus['id']; ?></td>
				   <td><?php echo $this->data->rowsvirus['ip']; ?></td>
				   <td><?php echo $this->data->rowsvirus['content']; ?></td>
				   <td><?php echo $this->data->rowsvirus['level']; ?></td>
				<?php if($this->data->rowsvirus['remove']=="已处理") {  ?>
				<td><font color=#D9773A><?php echo $this->data->color; ?><?php echo $this->data->rowsvirus['remove']; ?></font></td>
				<?php } else { ?>
				<td><?php echo $this->data->rowsvirus['remove']; ?></td>
				<?php } ?>
			    <td><?php echo $this->data->rowsvirus['date']; ?></td>
			    <td><a href="javascript:;" class="viewbtn" onClick="showdiv('div_<?php echo $this->data->rowsvirus['id']; ?>');">查看源代码</a></td>
            </tr>	
			<tr id="div_<?php echo $this->data->rowsvirus['id']; ?>" style="display:none;">
				<td class="ftd" colspan="7"><div class="codebox"><?php echo htmlspecialchars($this->data->rowsvirus['source']); ?></div></td>
			</tr>
            
        </table>
