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
#include "forma_config.h"
#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
#include "forma_conv_str.h"
#endif
#include "forma_conv_int.h"

#if FORMA_SUPPORT_CONV_INT_ENABLED == 1

static const forma_conv_state_definition_t* forma_conv_int_StateSigned ( forma_conv_data_t* data ) ;

static const forma_conv_state_definition_t* forma_conv_int_StateUnsigned ( forma_conv_data_t* data ) ;

const forma_conv_state_definition_t forma_conv_int_STATE_SIGNED = {  //
    .run = forma_conv_int_StateSigned, /**/
    } ;

const forma_conv_state_definition_t forma_conv_int_STATE_UNSIGNED = {  //
    .run = forma_conv_int_StateUnsigned, /**/
    } ;

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1

static ssize_t forma_conv_int_UpdateWidthPrecision ( forma_conv_data_t* data, int addMinus, char** bufferPtr, ssize_t bufferFilled )
{
  ssize_t width = data->width ;
  ssize_t precision = data->precision ;
  if ( precision >= 0 && precision >= width )
  {
    width = 0 ;

    if ( bufferFilled > precision )
    {
      if ( 0 == data->leftOriented )
      {
        ( *bufferPtr ) -= bufferFilled - precision ;
      }
      bufferFilled = precision ;
      if ( bufferFilled > 0 && addMinus )
      {
        bufferFilled-- ;
      }
      precision = 0 ;
    }
    else
    {
      precision -= bufferFilled ;
    }
  }
  else if ( width >= 0 )
  {
    if ( bufferFilled > width )
    {
      if ( 0 == data->leftOriented )
      {
        ( *bufferPtr ) -= bufferFilled - width ;
      }
      bufferFilled = width ;
      if ( bufferFilled > 0 && addMinus )
      {
        bufferFilled-- ;
      }
      width = 0 ;
    }
    else if ( precision >= 0 )
    {
      precision -= bufferFilled ;
      width -= bufferFilled + precision ;
    }
    else
    {
      width -= bufferFilled ;
    }
  }

  data->width = width ;
  data->precision = precision ;

  return bufferFilled ;
}

#endif /* FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED */

static int forma_conv_int_WriteBuffer ( forma_conv_data_t* data, int addMinus, char* bufferPtr, ssize_t bufferFilled )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

#if FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED == 1
  /**/
  char artificialQuote = data->artificialQuote ;
  if ( artificialQuote && 0 != writeChar( context, artificialQuote ) )
  {
    return -1 ;
  }
#endif

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
  /**/
  bufferFilled = forma_conv_int_UpdateWidthPrecision( data, addMinus, &bufferPtr, bufferFilled ) ;
  ssize_t width = data->width ;
  ssize_t precision = data->precision ;

  /**/
  if ( 0 == data->leftOriented && width > 0 )
  {
    char filler = data->filler ;
    if ( addMinus )
    {
      if ( '0' == filler )
      {
        if ( 0 != writeChar( context, '-' ) )
        {
          return -1 ;
        }

        addMinus = 0 ;
      }

      width-- ;
    }
    while ( width-- > 0 )
    {
      if ( 0 != writeChar( context, filler ) )
      {
        return -1 ;
      }
    }
  }
#endif

  /**/
  if ( addMinus )
  {
    if ( 0 != writeChar( context, '-' ) )
    {
      return -1 ;
    }

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
    if ( width > 0 )
    {
      width-- ;
    }
#endif
  }

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
  /**/
  if ( precision > 0 )
  {
    while ( precision-- > 0 )
    {
      if ( 0 != writeChar( context, '0' ) )
      {
        return -1 ;
      }
    }
  }
#endif

  /**/
  while ( bufferFilled-- > 0 )
  {
    if ( 0 != writeChar( context, *--bufferPtr ) )
    {
      return -1 ;
    }
  }

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
  /**/
  if ( 0 != data->leftOriented && width > 0 )
  {
    while ( width-- > 0 )
    {
      if ( 0 != writeChar( context, ' ' ) )
      {
        return -1 ;
      }
    }
  }
#endif

#if FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED == 1
  /**/
  if ( artificialQuote && 0 != writeChar( context, artificialQuote ) )
  {
    return -1 ;
  }
#endif

  return 0 ;
}

static const forma_conv_state_definition_t* forma_conv_int_StateSigned ( forma_conv_data_t* data )
{
  forma_context_t* context = data->context ;

  /* fill buffer */
#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 1
  char buffer[ sizeof ( "-18446744073709551615" ) ] ;
#else
  char buffer[ sizeof ( "-2147483647" ) ] ;
#endif
  char* bufferPtr = buffer ;
  ssize_t bufferFilled = 0 ;
  int addMinus ;
#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 1
  if ( 32 == data->length )
#endif
  {
    int value = va_arg( context->ap, int ) ;

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED
    if ( data->expressionValue )
#endif
    {
      if ( 0 == value )
      {
        addMinus = 0 ;
        bufferFilled++ ;
        *bufferPtr++ = '0' ;
      }
      else
      {
        if ( value < 0 )
        {
          addMinus = 1 ;
          value = 0 - value ;
        }
        else
        {
          addMinus = 0 ;
        }

        int32_t divider = ( int32_t ) data->divider ;
        while ( value > 0 )
        {
          bufferFilled++ ;
          int32_t newValue = value / divider ;
          *bufferPtr++ = ( char ) ( '0' + ( value - newValue * divider ) ) ;
          value = newValue ;
        }
      }
    }
  }
#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 1
  else
  {
    int64_t value = va_arg( context->ap, int64_t ) ;

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
    if ( data->expressionValue )
#endif
    {
      if ( 0 == value )
      {
        addMinus = 0 ;
        bufferFilled++ ;
        *bufferPtr++ = '0' ;
      }
      else
      {
        if ( value < 0 )
        {
          addMinus = 1 ;
          value = 0 - value ;
        }
        else
        {
          addMinus = 0 ;
        }

        int64_t divider = ( int64_t ) data->divider ;
        while ( value > 0 )
        {
          bufferFilled++ ;
          int64_t newValue = ( int64_t ) ( value / divider ) ;
          *bufferPtr++ = ( char ) ( '0' + ( value - newValue * divider ) ) ;
          value = newValue ;
        }
      }
    }
  }
#endif /* FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED */

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED
  if ( data->expressionValue )
#endif
  {
    if ( 0 != forma_conv_int_WriteBuffer( data, addMinus, bufferPtr, bufferFilled ) )
    {
      return 0 ;
    }
  }
#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
  else if ( 0 != forma_conv_str_WriteSqlNull( context, data->writeChar ) )
  {
    return 0 ;
  }
#endif

  return &forma_conv_STATE_FINISHED ;
}

static const forma_conv_state_definition_t* forma_conv_int_StateUnsigned ( forma_conv_data_t* data )
{
  forma_context_t* context = data->context ;

  /* fill buffer */
#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 1
  char buffer[ sizeof ( "-18446744073709551615" ) ] ;
#else
  char buffer[ sizeof ( "-2147483647" ) ] ;
#endif
  char* bufferPtr = buffer ;
  ssize_t bufferFilled = 0 ;
#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 1
  if ( 32 == data->length )
#endif
  {
    unsigned value = va_arg( context->ap, unsigned ) ;

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
    if ( data->expressionValue )
#endif
    {
      if ( 0 == value )
      {
        bufferFilled++ ;
        *bufferPtr++ = '0' ;
      }
      else
      {
        uint32_t divider = ( uint32_t ) data->divider ;
        while ( value > 0 )
        {
          bufferFilled++ ;
          uint32_t newValue = value / divider ;
          *bufferPtr++ = ( char ) ( '0' + ( value - newValue * divider ) ) ;
          value = newValue ;
        }
      }
    }
  }
#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 1
  else
  {
    uint64_t value = va_arg( context->ap, uint64_t ) ;

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
    if ( data->expressionValue )
#endif
    {
      if ( 0 == value )
      {
        bufferFilled++ ;
        *bufferPtr++ = '0' ;
      }
      else
      {
        uint64_t divider = ( uint64_t ) data->divider ;
        while ( value > 0 )
        {
          bufferFilled++ ;
          uint64_t newValue = ( uint64_t ) ( value / divider ) ;
          *bufferPtr++ = ( char ) ( '0' + ( value - newValue * divider ) ) ;
          value = newValue ;
        }
      }
    }
  }
#endif /* FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED */

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
  if ( data->expressionValue )
#endif
  {
    if ( 0 != forma_conv_int_WriteBuffer( data, 0, bufferPtr, bufferFilled ) )
    {
      return 0 ;
    }
  }
#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
  else if ( 0 != forma_conv_str_WriteSqlNull( context, data->writeChar ) )
  {
    return 0 ;
  }
#endif

  return &forma_conv_STATE_FINISHED ;
}

#endif /* FORMA_SUPPORT_CONV_INT_ENABLED */
