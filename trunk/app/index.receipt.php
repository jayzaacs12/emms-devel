<html>
<head>
<style>
.advisor {position: absolute;     left: 500px; top: 660px;}
.xp_pmt_date {position: absolute; left: 500px; top: 40px;}
.client {position: absolute;      left: 160px; top: 145px;}
.xp_pmt {position: absolute;  	  left: 310px; top: 260px;}
.num_pmt {position: absolute;     left: 200px; top: 310px;}
.total {position: absolute;  	  left: 170px; top: 620px;}
</style>
</head>
<body>
<div class="advisor"><?= $_REQUEST['advisor']?></div>
<div class="client"><?= $_REQUEST['client']?></div>
<div class="xp_pmt_date"><?= $_REQUEST['xp_pmt_date'] ?></div>
<div class="num_pmt">Cuota <?= $_REQUEST['num_pmt']?> de <?= $_REQUEST['xp_num_pmt'] ?></div>
<div class="xp_pmt"><?= $_REQUEST['xp_pmt']?></div>
<div class="total"><?= $_REQUEST['xp_pmt']?></div>
</body>
</html>
