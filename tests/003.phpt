--TEST--
fribidi_log2vis(), UTF-8
--SKIPIF--
<?php if (!extension_loaded("fribidi")) print "skip"; ?>
--POST--
--GET--
--FILE--
<?php
	error_reporting (E_ALL ^ E_NOTICE);
	$a =  fribidi_log2vis("سلام", FRIBIDI_AUTO, FRIBIDI_CHARSET_UTF8);
	$b =  fribidi_log2vis("سلام به همه", FRIBIDI_AUTO, FRIBIDI_CHARSET_UTF8);

	$d = array($a, $b);
	var_dump($d);
?>
--EXPECT--
array(3) {
  [0]=>
  string(26) "ﺳﺎﻟﻡ"
  [1]=>
  string(26) "ﺳﺎﻟﻡ ﺑﻪ ﻫﻤﻪ"
}
