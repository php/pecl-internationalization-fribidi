--TEST--
fribidi_log2vis() [UTF-8 file]
--SKIPIF--
<?php if (!extension_loaded("fribidi")) print "skip"; ?>
--POST--
--GET--
--FILE--
<?php
	error_reporting (E_ALL);
        mb_internal_encoding("UTF-8");

	$a = fribidi_log2vis("THE dog 123 IS THE biggest", FRIBIDI_AUTO, FRIBIDI_CHARSET_CAP_RTL);
	$b = fribidi_log2vis("THE dog 123 IS THE biggest", FRIBIDI_AUTO, FRIBIDI_CHARSET_UTF8);
	$c = fribidi_log2vis("یونی‌کد برای همه",      FRIBIDI_AUTO, FRIBIDI_CHARSET_UTF8);

	var_dump(array($a, $b, $c));
?>
--EXPECT--
array(3) {
  [0]=>
  string(26) "biggest EHT SI dog 123 EHT"
  [1]=>
  string(26) "THE dog 123 IS THE biggest"
  [2]=>
  string(44) "ﻪﻤﻫ ﯼﺍﺮﺑ ﺪﮐ‌ﯽﻧﻮﯾ"
}
