--TEST--
fribidi_log2vis() [Paragraph directions]
--SKIPIF--
<?php if (!extension_loaded("fribidi")) print "skip"; ?>
--POST--
--GET--
--FILE--
<?php
	error_reporting (E_ALL);

	$a = fribidi_log2vis("ABC def", FRIBIDI_AUTO, FRIBIDI_CHARSET_CAP_RTL);
	$b = fribidi_log2vis("ABC def", FRIBIDI_LTR,  FRIBIDI_CHARSET_CAP_RTL);
	$c = fribidi_log2vis("ABC def", FRIBIDI_RTL,  FRIBIDI_CHARSET_CAP_RTL);
	$d = fribidi_log2vis("ABC def", FRIBIDI_WLTR, FRIBIDI_CHARSET_CAP_RTL);
	$e = fribidi_log2vis("ABC def", FRIBIDI_WRTL, FRIBIDI_CHARSET_CAP_RTL);
	var_dump(array($a, $b, $c, $d, $e));

	$a = fribidi_log2vis("abc DEF", FRIBIDI_AUTO, FRIBIDI_CHARSET_CAP_RTL);
	$b = fribidi_log2vis("abc DEF", FRIBIDI_LTR,  FRIBIDI_CHARSET_CAP_RTL);
	$c = fribidi_log2vis("abc DEF", FRIBIDI_RTL,  FRIBIDI_CHARSET_CAP_RTL);
	$d = fribidi_log2vis("abc DEF", FRIBIDI_WLTR, FRIBIDI_CHARSET_CAP_RTL);
	$e = fribidi_log2vis("abc DEF", FRIBIDI_WRTL, FRIBIDI_CHARSET_CAP_RTL);
	var_dump(array($a, $b, $c, $d, $e));

	$a = fribidi_log2vis("123 456", FRIBIDI_AUTO, FRIBIDI_CHARSET_CAP_RTL);
	$b = fribidi_log2vis("123 456", FRIBIDI_LTR,  FRIBIDI_CHARSET_CAP_RTL);
	$c = fribidi_log2vis("123 456", FRIBIDI_RTL,  FRIBIDI_CHARSET_CAP_RTL);
	$d = fribidi_log2vis("123 456", FRIBIDI_WLTR, FRIBIDI_CHARSET_CAP_RTL);
	$e = fribidi_log2vis("123 456", FRIBIDI_WRTL, FRIBIDI_CHARSET_CAP_RTL);
	var_dump(array($a, $b, $c, $d, $e));

?>
--EXPECT--
array(5) {
  [0]=>
  string(7) "def CBA"
  [1]=>
  string(7) "CBA def"
  [2]=>
  string(7) "def CBA"
  [3]=>
  string(7) "def CBA"
  [4]=>
  string(7) "def CBA"
}
array(5) {
  [0]=>
  string(7) "abc FED"
  [1]=>
  string(7) "abc FED"
  [2]=>
  string(7) "FED abc"
  [3]=>
  string(7) "abc FED"
  [4]=>
  string(7) "abc FED"
}
array(5) {
  [0]=>
  string(7) "123 456"
  [1]=>
  string(7) "123 456"
  [2]=>
  string(7) "456 123"
  [3]=>
  string(7) "123 456"
  [4]=>
  string(7) "456 123"
}
