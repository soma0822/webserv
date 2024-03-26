<?php
  $n = 0;


  if(isset($_GET['clear'])){
        //cookieの消去 
    setcookie( "visit_count" ,"",time() - 3600);
  }else{
    
    if (isset($_COOKIE["visit_count"])){
    //cookieがある
      $n = $_COOKIE["visit_count"] +1;
    }
    else{
    //初めての訪問
      $n = 1;
    }
    setcookie( "visit_count" ,$n,time() + 3600*24*1);

  }

?>
<HTML>
<HEAD>
　<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
　<TITLE>PHPでcookieを使った訪問回数の記録方法</TITLE>
</HEAD>
<BODY>
<CENTER>
<TABLE border='1'  width='90%' bgcolor='#FDF5E6'>

<th>
このページの訪問回数
</th>

<tr>
<td>

<form action=""  method="get">
<?php
   echo " <input type='text' size='10' disabled='disabled' value='" . $n . " 回訪問'>";
?>
 <input type="submit" name="clear" value="クリア">
 <input type="submit" name="" value="数え始める">

</form>
<br>
24時間以内にページを更新したり、再び入りなおすと、回数が増えていきます。

</td>
</tr>




</TABLE>

</CENTER>

</BODY>
</HTML>

