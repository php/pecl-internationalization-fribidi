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

/* {{{ internal helper funcations
*/

static inline zend_bool
_direction_is_validate(direction)
{
	switch (direction) {
		case FRIBIDI_PAR_ON:
		case FRIBIDI_PAR_LTR:
		case FRIBIDI_PAR_RTL:
		case FRIBIDI_PAR_WLTR:
		case FRIBIDI_PAR_WRTL:
			return 1;
	}
	return 0;
}

#define _validate_direction(direction) \
{ \
	if (!_direction_is_validate(direction)) { \
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unknown direction."); \
		RETURN_FALSE; \
	} \
}

static inline zend_bool
_charset_is_validate(charset)
{
	switch (charset) {
		case FRIBIDI_CHAR_SET_UTF8:
		case FRIBIDI_CHAR_SET_ISO8859_6:
		case FRIBIDI_CHAR_SET_ISO8859_8:
		case FRIBIDI_CHAR_SET_CP1255:
		case FRIBIDI_CHAR_SET_CP1256:
		case FRIBIDI_CHAR_SET_CAP_RTL:
			return 1;
	}
	return 0;
}

#define _validate_charset(charset) \
{ \
	if (!_charset_is_validate(charset)) { \
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unknown charset."); \
		RETURN_FALSE; \
	} \
}

/* }}} */

/*
+ -----------------------------------------------------------+
| Name: fribidi_log2vis                                      |
| Purpose: convert a logical string to a visual one          |
| Input: 1) The logical string.                              |
|        2) Base direction -                                 |
|           Possible values:                                 |
|             FRIBIDI_AUTO - autodetected by the Unicode     |
|                            Bidirection Algorithm (UBA).    |
|             FRIBIDI_LTR  - left to right.                  |
|             FRIBIDI_RTL  - right to left.                  |
|             FRIBIDI_WLTR - week left to right.             |
|             FRIBIDI_WRTL - week right to left.             |
|        3) Character code being used -                      |
|           Possible values (i.e., charsets supported)       |
|             FRIBIDI_CHARSET_UTF8                           |
|             FRIBIDI_CHARSET_8859_6                         |
|             FRIBIDI_CHARSET_8859_8                         |
|             FRIBIDI_CHARSET_CP1255                         |
|             FRIBIDI_CHARSET_CP1256                         |
|             FRIBIDI_CHARSET_CAP_RTL                        |
|                                                            |
| Output: on success: The visual string.                     |
|         on failure: false                                  |
+------------------------------------------------------------+
*/

/* {{{ proto string fribidi_log2vis(string logical_str, long direction, long charset)
   Convert a logical string to a visual one */
PHP_FUNCTION(fribidi_log2vis)
{
	char *logical_str, *visual_str;
	long charset, direction;
	int  logical_str_len, visual_str_len, ustr_len;
	FriBidiParType base_direction;
	FriBidiLevel   status;
	FriBidiChar    *logical_ustr, *visual_ustr;

	// Read and validate parameters
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sll", &logical_str, &logical_str_len, &direction, &charset) == FAILURE)
		WRONG_PARAM_COUNT;
	_validate_direction(direction);
	_validate_charset(charset);

	// Convert input string to internal Unicode
	logical_ustr = (FriBidiChar*) emalloc(sizeof(FriBidiChar) * logical_str_len);
	ustr_len = fribidi_charset_to_unicode(charset, logical_str, logical_str_len, logical_ustr);

	// Visualize the Unicode string
	base_direction = direction;
	visual_ustr = (FriBidiChar*) emalloc(sizeof(FriBidiChar) * ustr_len);
	status = fribidi_log2vis(logical_ustr, ustr_len, &base_direction, visual_ustr, NULL, NULL, NULL);
	efree(logical_ustr);

	// Return false if FriBidi failed
	if (status == 0) {
		efree(visual_ustr);
		RETURN_FALSE;
	}

	// Convert back from internal Unicode to original character set
	visual_str_len = 4 * ustr_len;	// FriBidi doesn't generate UTF-8 chars longer than 4 bytes
	visual_str = (char *) emalloc(sizeof(char) * visual_str_len);
	visual_str_len = fribidi_unicode_to_charset(charset, visual_ustr, ustr_len, visual_str);
	efree(visual_ustr);

	// Return the result
	RETURN_STRINGL(visual_str, visual_str_len, 0);
}
/* }}} */

/* {{{ proto array fribidi_charset_info(int charset)
   Returns an array containing information about the specified charset */
PHP_FUNCTION(fribidi_charset_info)
{
	long charset;

	// Read and validate parameters
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &charset) == FAILURE)
		WRONG_PARAM_COUNT;
	_validate_charset(charset);

	// Return the result
	array_init(return_value);
	add_assoc_string(return_value, "name",
			(char *)fribidi_char_set_name(charset), 1);
	add_assoc_string(return_value, "title",
			(char *)fribidi_char_set_title(charset), 1);
	if (fribidi_char_set_desc(charset))
		add_assoc_string(return_value, "desc",
				(char *)fribidi_char_set_desc(charset), 1);
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
