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

#include <stdint.h>
#include "forma_private.h"
#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
#include "forma_conv_str.h"
#endif
#include "forma_conv_function.h"

#if FORMA_SUPPORT_CONV_FUNCTION_ENABLED == 1

static const forma_conv_state_definition_t* forma_conv_function_State ( forma_conv_data_t* data ) ;

const forma_conv_state_definition_t forma_conv_function_STATE = {  //
    .run = forma_conv_function_State, /**/
    } ;

static const forma_conv_state_definition_t* forma_conv_function_State ( forma_conv_data_t* data )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

  /**/
  forma_Function_t* fcn = va_arg( context->ap, forma_Function_t* ) ;
  void* userData = va_arg( context->ap, void* ) ;

  /* process value */
#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
  if ( data->expressionValue )
#endif
  {
#if FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED == 1
    /**/
    char artificialQuote = data->artificialQuote ;
    if ( artificialQuote && 0 != writeChar( context, artificialQuote ) )
    {
      return 0 ;
    }
#endif

    /**/
    if ( 0 != fcn )
    {
      forma_object_t object = { //
          .context = context, /**/
          .writeChar = data->writeChar, /**/
          } ;
      fcn( &object, userData ) ;
    }

#if FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED == 1
    /**/
    if ( artificialQuote && 0 != writeChar( context, artificialQuote ) )
    {
      return 0 ;
    }
#endif
  }
#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
  else if ( 0 != forma_conv_str_WriteSqlNull( context, writeChar ) )
  {
    return 0 ;
  }
#endif

  return &forma_conv_STATE_FINISHED ;
}

#endif /* FORMA_SUPPORT_CONV_FUNCTION_ENABLED */
