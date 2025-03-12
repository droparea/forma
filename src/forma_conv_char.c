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
#include "forma_conv_char.h"

#if FORMA_SUPPORT_CONV_CHAR_ENABLED == 1

static const forma_conv_state_definition_t* forma_conv_char_StateValue ( forma_conv_data_t* data ) ;

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
static const forma_conv_state_definition_t* forma_conv_char_StateArray ( forma_conv_data_t* data ) ;
#endif

const forma_conv_state_definition_t forma_conv_char_STATE_VALUE = {  //
    .run = forma_conv_char_StateValue, /**/
    } ;

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
const forma_conv_state_definition_t forma_conv_char_STATE_ARRAY = {  //
    .run = forma_conv_char_StateArray, /**/
    } ;
#endif

static const forma_conv_state_definition_t* forma_conv_char_StateValue ( forma_conv_data_t* data )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

  /**/
  int value = va_arg( context->ap, int ) ;

  /**/
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
    value &= 0xFF ;
    if ( value != 0 )
    {
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
      ssize_t width = data->width ;
#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1
      if ( 1 == data->repeatOrReverseOrOffsetOrMultiParts )
      {
        while ( width-- > 0 )
        {
          if ( 0 != writeChar( context, ( char ) value ) )
          {
            return 0 ;
          }
        }
      }
      else
#endif /* FORMA_SUPPORT_FLAG_TILDE_ENABLED */
#endif /* FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED */
      {
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
        if ( 0 == data->leftOriented )
        {
          while ( width-- > 1 )
          {
            if ( 0 != writeChar( context, ' ' ) )
            {
              return 0 ;
            }
          }
        }
#endif /* FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED */

        if ( 0 != writeChar( context, ( char ) value ) )
        {
          return 0 ;
        }

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
        if ( 1 == data->leftOriented )
        {
          while ( width-- > 1 )
          {
            if ( 0 != writeChar( context, ' ' ) )
            {
              return 0 ;
            }
          }
        }
#endif /* FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED */
      }
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

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1

static const forma_conv_state_definition_t* forma_conv_char_StateArray ( forma_conv_data_t* data )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

  /**/
  char* value = va_arg( context->ap, char* ) ;

  /**/
#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
  if ( data->expressionValue )
#endif
  {
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
    forma_MapChar_t* mapChar = data->mapChar ;
    void* mapCharContext = data->mapCharContext ;
#endif

#if FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED == 1
    /**/
    char artificialQuote = data->artificialQuote ;
    if ( artificialQuote && 0 != writeChar( context, artificialQuote ) )
    {
      return 0 ;
    }
#endif

    ssize_t width = data->width ;
    if ( width < 0 )
    {
      width = 0 ;
    }
    ssize_t precision = data->precision ;
    if ( precision < 0 )
    {
      precision = 0 ;
    }
    size_t countFillIn = precision > 0 && precision > width ? precision - width : 0 ;

    if ( 0 == data->leftOriented )
    {
      char filler = data->filler ;
      while ( countFillIn-- > 0 )
      {
        char c = filler ;
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
        if ( mapChar )
        {
          c = mapChar( mapCharContext, true, c ) ;
          if ( 0 == c )
          {
            return 0 ;
          }
        }
#endif
        if ( 0 != writeChar( context, c ) )
        {
          return 0 ;
        }
      }
    }

#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1
    if ( 1 == data->repeatOrReverseOrOffsetOrMultiParts )
    {
      value += width ;
      while ( width-- > 0 )
      {
        char c = ( char ) *--value ;
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
        if ( mapChar )
        {
          c = mapChar( mapCharContext, false, c ) ;
          if ( 0 == c )
          {
            return 0 ;
          }
        }
#endif
        if ( 0 != writeChar( context, c ) )
        {
          return 0 ;
        }
      }
    }
    else
#endif
    {
      while ( width-- > 0 )
      {
        char c = ( char ) *value++ ;

#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
        if ( mapChar )
        {
          c = mapChar( mapCharContext, false, c ) ;
          if ( 0 == c )
          {
            return 0 ;
          }
        }
#endif
        if ( 0 != writeChar( context, c ) )
        {
          return 0 ;
        }
      }
    }

    if ( 0 != data->leftOriented )
    {
      char filler = data->filler ;
      while ( countFillIn-- > 0 )
      {
        char c = filler ;
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
        if ( mapChar )
        {
          c = mapChar( mapCharContext, true, c ) ;
          if ( 0 == c )
          {
            return 0 ;
          }
        }
#endif
        if ( 0 != writeChar( context, c ) )
        {
          return 0 ;
        }
      }
    }

#if FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED == 1
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

#endif /* FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED */

#endif /* FORMA_SUPPORT_CONV_CHAR_ENABLED */
