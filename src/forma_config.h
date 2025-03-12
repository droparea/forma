/**
 * Copyright by Jens Goetze <jens.goetze@droparea.de>
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 */

#ifndef __FORMA_CONFIG_H__
#define __FORMA_CONFIG_H__

#ifdef DROPOS_CONFIG_FILE
#include DROPOS_CONFIG_FILE
#endif

#ifndef FORMA_SUPPORT_FLAG_PATTERN_ENABLED
#define FORMA_SUPPORT_FLAG_PATTERN_ENABLED            1
#endif

#ifndef FORMA_SUPPORT_FLAG_TILDE_ENABLED
#define FORMA_SUPPORT_FLAG_TILDE_ENABLED              1
#endif

#ifndef FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED
#define FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED            1
#endif

#ifndef FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED
#define FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED   1
#endif

#ifndef FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED
#define FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED     1
#endif

#ifndef FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED
#define FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED        1
#endif

#ifndef FORMA_SUPPORT_CONV_BASE64_ENABLED
#define FORMA_SUPPORT_CONV_BASE64_ENABLED             1
#endif
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED != 1
#undef FORMA_SUPPORT_CONV_BASE64_ENABLED
#define FORMA_SUPPORT_CONV_BASE64_ENABLED             0
#endif

#ifndef FORMA_SUPPORT_CONV_CHAR_ENABLED
#define FORMA_SUPPORT_CONV_CHAR_ENABLED               1
#endif

#ifndef FORMA_SUPPORT_CONV_FLOAT_ENABLED
#define FORMA_SUPPORT_CONV_FLOAT_ENABLED              1
#endif

#ifndef FORMA_SUPPORT_CONV_FUNCTION_ENABLED
#define FORMA_SUPPORT_CONV_FUNCTION_ENABLED           1
#endif

#ifndef FORMA_SUPPORT_CONV_HEX_ENABLED
#define FORMA_SUPPORT_CONV_HEX_ENABLED                1
#endif

#ifndef FORMA_SUPPORT_CONV_INT_ENABLED
#define FORMA_SUPPORT_CONV_INT_ENABLED                1
#endif

#ifndef FORMA_SUPPORT_CONV_NET_ENABLED
#define FORMA_SUPPORT_CONV_NET_ENABLED                1
#endif

#ifndef FORMA_SUPPORT_CONV_STR_ENABLED
#define FORMA_SUPPORT_CONV_STR_ENABLED                1
#endif

#ifndef FORMA_SUPPORT_CONV_STR_SQL_ENABLED
#define FORMA_SUPPORT_CONV_STR_SQL_ENABLED            1
#endif

#ifndef FORMA_SUPPORT_CONV_STR_JSON_ENABLED
#define FORMA_SUPPORT_CONV_STR_JSON_ENABLED           1
#endif

#ifndef FORMA_SUPPORT_CONV_TIME_ENABLED
#define FORMA_SUPPORT_CONV_TIME_ENABLED               1
#endif

#ifndef FORMA_PRINT_BUFFER_SIZE
#define FORMA_PRINT_BUFFER_SIZE                       128
#endif

#ifndef FORMA_NEW_LINE
#define FORMA_NEW_LINE                                "\r\n"
#endif

#ifndef FORMA_TIME_DEFAULT_PATTERN
#define FORMA_TIME_DEFAULT_PATTERN                    "Y-M-D h:m:s"
#endif

#ifndef FORMA_CONSOLE_USE_WRITE
#define FORMA_CONSOLE_USE_WRITE                       0
#endif

#endif /* __FORMA_H__ */
