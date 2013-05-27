--TEST--
fribidi_log2vis() [non-strings and empty strings]
--SKIPIF--
<?php if (!extension_loaded("fribidi")) print "skip"; ?>
--POST--
--GET--
--FILE--
<?php
	error_reporting (E_ALL);

	$a = fribidi_log2vis(null,  FRIBIDI_AUTO, FRIBIDI_CHARSET_CAP_RTL);
	$b = fribidi_log2vis(false, FRIBIDI_AUTO, FRIBIDI_CHARSET_CAP_RTL);
	$c = fribidi_log2vis(true,  FRIBIDI_AUTO, FRIBIDI_CHARSET_CAP_RTL);
	$d = fribidi_log2vis(0,     FRIBIDI_AUTO, FRIBIDI_CHARSET_CAP_RTL);
	$e = fribidi_log2vis(1,     FRIBIDI_AUTO, FRIBIDI_CHARSET_CAP_RTL);
	$f = fribidi_log2vis('',    FRIBIDI_AUTO, FRIBIDI_CHARSET_CAP_RTL);
	$g = fribidi_log2vis("",    FRIBIDI_AUTO, FRIBIDI_CHARSET_CAP_RTL);

	var_dump(array($a, $b, $c, $d, $e, $f, $g));
?>
--EXPECT--
array(7) {
  [0]=>
  string(0) ""
  [1]=>
  string(0) ""
  [2]=>
  string(1) "1"
  [3]=>
  string(1) "0"
  [4]=>
  string(1) "1"
  [5]=>
  string(0) ""
  [6]=>
  string(0) ""
}
