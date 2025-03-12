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

#ifndef __FORMA_CONV_STR_H__
#define __FORMA_CONV_STR_H__

#include "forma_conv.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if FORMA_SUPPORT_CONV_STR_ENABLED == 1

  extern const forma_conv_state_definition_t forma_conv_str_STATE ;

  extern const forma_conv_state_definition_t forma_conv_str_STATE_JSON ;

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
  extern const forma_conv_state_definition_t forma_conv_str_STATE_SQL ;
#endif

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
  int forma_conv_str_WriteSqlNull ( forma_context_t* context, forma_WriteChar_t* writeChar ) ;
#endif

#endif /* FORMA_SUPPORT_CONV_STR_ENABLED */

#ifdef __cplusplus
}
#endif

#endif /* __FORMA_CONV_STR_H__ */
