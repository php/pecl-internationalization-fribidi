/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Onn Ben-Zvi <onnb@mercury.co.il>                            |
  |          Tal Peer <tal@php.net>                                      |
  |          Behnam Esfahbod <behnam@php.net>                            |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_fribidi.h"

#if HAVE_FRIBIDI

#include "ext/standard/info.h"
#include <fribidi.h>

zend_function_entry fribidi_functions[] = {
	PHP_FE(fribidi_log2vis,	     NULL)
	PHP_FE(fribidi_charset_info, NULL)
	PHP_FE(fribidi_get_charsets, NULL)
	{NULL, NULL, NULL}
};

zend_module_entry fribidi_module_entry = {
	STANDARD_MODULE_HEADER,
	"fribidi",
	fribidi_functions,
	PHP_MINIT(fribidi),
	PHP_MSHUTDOWN(fribidi),
	NULL,
	NULL,
	PHP_MINFO(fribidi),
	PHP_FRIBIDI_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_FRIBIDI
ZEND_GET_MODULE(fribidi)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(fribidi)
{
	/* Charsets */
	REGISTER_LONG_CONSTANT("FRIBIDI_CHARSET_UTF8",    FRIBIDI_CHAR_SET_UTF8,      CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRIBIDI_CHARSET_8859_6",  FRIBIDI_CHAR_SET_ISO8859_6, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRIBIDI_CHARSET_8859_8",  FRIBIDI_CHAR_SET_ISO8859_8, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRIBIDI_CHARSET_CP1255",  FRIBIDI_CHAR_SET_CP1255,    CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRIBIDI_CHARSET_CP1256",  FRIBIDI_CHAR_SET_CP1256,    CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRIBIDI_CHARSET_CAP_RTL", FRIBIDI_CHAR_SET_CAP_RTL,   CONST_CS | CONST_PERSISTENT);

	/* Directions */
	REGISTER_LONG_CONSTANT("FRIBIDI_AUTO", FRIBIDI_PAR_ON,   CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRIBIDI_LTR",  FRIBIDI_PAR_LTR,  CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRIBIDI_RTL",  FRIBIDI_PAR_RTL,  CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRIBIDI_WLTR", FRIBIDI_PAR_WLTR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FRIBIDI_WRTL", FRIBIDI_PAR_WRTL, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(fribidi)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(fribidi)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "FriBidi support", "enabled");
	php_info_print_table_row(2, "FriBidi version", FRIBIDI_VERSION);
	php_info_print_table_row(2, "Extension version", PHP_FRIBIDI_VERSION);
	php_info_print_table_end();
}
/* }}} */

/*
+ -----------------------------------------------------------+
| Name: fribidi_log2vis                                      |
| Purpose: convert a logical string to a visual one          |
| Input: 1) The logical string.                              |
|        2) Base direction -                                 |
|             Possible values:                               |
|             FRIBIDI_AUTO - autodetected by the Unicode     |
|                            Bidirection algorithm.          |
|             FRIBIDI_LTR  - left to right.                  |
|             FRIBIDI_RTL  - right to left.                  |
|             FRIBIDI_WLTR - week left to right.             |
|             FRIBIDI_WRTL - week right to left.             |
|        3) Character code being used -                      |
|             Possible values (i.e., charsets supported)     |
|              FRIBIDI_CHARSET_UTF8                          |
|              FRIBIDI_CHARSET_8859_6                        |
|              FRIBIDI_CHARSET_8859_8                        |
|              FRIBIDI_CHARSET_CP1255                        |
|              FRIBIDI_CHARSET_CP1256                        |
|              FRIBIDI_CHARSET_CAP_RTL                       |
|                                                            |
| Output: on success: The visual string.                     |
|         on failure: FALSE                                  |
+------------------------------------------------------------+
*/

/* {{{ proto string fribidi_log2vis(string logical_str, long direction, long charset)
   Convert a logical string to a visual one */
PHP_FUNCTION(fribidi_log2vis)
{
	char *logical_str, *visual_str;
	int logical_str_len, visual_str_len;
	long charset, direction;
	int u_logical_str_len;
	FriBidiParType base_direction;
	FriBidiChar *u_logical_str, *u_visual_str;

	// Read and validate parameters
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sll", &logical_str, &logical_str_len, &direction, &charset) == FAILURE)
		WRONG_PARAM_COUNT;

	switch (direction) {
		case FRIBIDI_PAR_ON:
		case FRIBIDI_PAR_LTR:
		case FRIBIDI_PAR_RTL:
		case FRIBIDI_PAR_WLTR:
		case FRIBIDI_PAR_WRTL:
			break;
		default:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unknown direction.");
			RETURN_FALSE;
	}

	switch (charset) {
		case FRIBIDI_CHAR_SET_UTF8:
		case FRIBIDI_CHAR_SET_ISO8859_6:
		case FRIBIDI_CHAR_SET_ISO8859_8:
		case FRIBIDI_CHAR_SET_CP1255:
		case FRIBIDI_CHAR_SET_CP1256:
		case FRIBIDI_CHAR_SET_CAP_RTL:
			break;
		default:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unknown charset.");
			RETURN_FALSE;
	}

	// Convert input string to FriBidiChar
	u_logical_str = (FriBidiChar*) emalloc(sizeof(FriBidiChar) * logical_str_len);

	u_logical_str_len = fribidi_charset_to_unicode(charset, logical_str, logical_str_len, u_logical_str);

	// Visualize the FriBidiChar sequence
	base_direction = direction;
	u_visual_str = (FriBidiChar*) emalloc(sizeof(FriBidiChar) * logical_str_len);

	fribidi_log2vis(u_logical_str, u_logical_str_len, &base_direction,
			u_visual_str, NULL, NULL, NULL);

	efree(u_logical_str);

	// Convert back from FriBidiLevel to original character set
	visual_str = (char *) emalloc(sizeof(char) * 4 * u_logical_str_len);

	visual_str_len = fribidi_unicode_to_charset(charset, u_visual_str, u_logical_str_len, visual_str);

	efree(u_visual_str);

	// Return the result
	RETURN_STRINGL(visual_str, visual_str_len, 0);
}
/* }}} */

/* {{{ proto array fribidi_charset_info(int charset)
   Returns an array containing information about the specified charset */
PHP_FUNCTION(fribidi_charset_info)
{
	long charset;
	char *name, *title, *desc;

	// Read and validate parameters
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &charset) == FAILURE)
		WRONG_PARAM_COUNT;

	switch (charset) {
		case FRIBIDI_CHAR_SET_UTF8:
		case FRIBIDI_CHAR_SET_ISO8859_6:
		case FRIBIDI_CHAR_SET_ISO8859_8:
		case FRIBIDI_CHAR_SET_CP1255:
		case FRIBIDI_CHAR_SET_CP1256:
		case FRIBIDI_CHAR_SET_CAP_RTL:
			break;
		default:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unknown charset.");
			RETURN_FALSE;
	}

	// Return the result
	array_init(return_value);

	name  = (char *)fribidi_char_set_name(charset);
	add_assoc_string_ex(return_value, "name", sizeof("name"), name, 1);

	title = (char *)fribidi_char_set_title(charset);
	add_assoc_string_ex(return_value, "title", sizeof("title"), title, 1);

	desc  = (char *)fribidi_char_set_desc(charset);
	if (desc)
		add_assoc_string_ex(return_value, "desc", sizeof("desc"), desc, 1);
}
/* }}} */

/* {{{ proto array fribidi_get_charsets()
   Returns an array containing available charsets */
PHP_FUNCTION(fribidi_get_charsets)
{
	array_init(return_value);

	add_index_string(return_value, FRIBIDI_CHAR_SET_UTF8,      "FRIBIDI_CHARSET_UTF8",    1);
	add_index_string(return_value, FRIBIDI_CHAR_SET_ISO8859_6, "FRIBIDI_CHARSET_8859_6",  1);
	add_index_string(return_value, FRIBIDI_CHAR_SET_ISO8859_8, "FRIBIDI_CHARSET_8859_8",  1);
	add_index_string(return_value, FRIBIDI_CHAR_SET_CP1255,    "FRIBIDI_CHARSET_CP1255",  1);
	add_index_string(return_value, FRIBIDI_CHAR_SET_CP1256,    "FRIBIDI_CHARSET_CP1256",  1);
	add_index_string(return_value, FRIBIDI_CHAR_SET_CAP_RTL,   "FRIBIDI_CHARSET_CAP_RTL", 1);
}
/* }}} */

#endif	/* HAVE_FRIBIDI */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
