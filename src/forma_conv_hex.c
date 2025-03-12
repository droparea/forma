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
#include "forma_conv_hex.h"

#if FORMA_SUPPORT_CONV_HEX_ENABLED == 1

static const forma_conv_state_definition_t* forma_conv_hex_StateInt ( forma_conv_data_t* data ) ;

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1

static const forma_conv_state_definition_t* forma_conv_hex_StateArray ( forma_conv_data_t* data ) ;

static const forma_conv_state_definition_t* forma_conv_hex_StateDump ( forma_conv_data_t* data ) ;

#endif

const forma_conv_state_definition_t forma_conv_hex_STATE_INT = {  //
    .run = forma_conv_hex_StateInt, /**/
    } ;

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1

const forma_conv_state_definition_t forma_conv_hex_STATE_ARRAY = {  //
    .run = forma_conv_hex_StateArray, /**/
    } ;

const forma_conv_state_definition_t forma_conv_hex_STATE_DUMP = {  //
    .run = forma_conv_hex_StateDump, /**/
    } ;

#endif

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1

#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1

static int forma_conv_hex_WriteOffset ( forma_context_t* context, forma_WriteChar_t* writeChar, size_t offset )
{
  int result ;

  result = writeChar( context, '[' ) ;
  if ( 0 != result )
  {
    return result ;
  }

  unsigned length = sizeof(size_t) << 1 ;
  while ( length-- > 0 )
  {
    int value = ( int ) ( ( offset ) >> ( sizeof(size_t) * 8 - 4 ) ) ;
    offset <<= 4 ;

    int c = ( char ) value ;
    if ( c <= 9 )
    {
      c += ( char ) ( '0' & 0xFF ) ;
    }
    else
    {
      c += ( char ) ( ( 'A' - 10 ) & 0xFF ) ;
    }

    result = writeChar( context, ( char ) c ) ;
    if ( 0 != result )
    {
      return result ;
    }
  }

  result = forma_WriteString( context, writeChar, "]  " ) ;
  if ( 0 != result )
  {
    return result ;
  }

  return 0 ;
}

#endif /* FORMA_SUPPORT_FLAG_TILDE_ENABLED */

static int forma_conv_hex_WriteArray ( forma_conv_data_t* data, const uint8_t* hexData, const char* separator )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

  /* sanity check */
  if ( 0 == hexData )
  {
    return 0 ;
  }

#if FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED == 1
  /**/
  char artificialQuote = data->artificialQuote ;
  if ( artificialQuote && 0 != writeChar( context, artificialQuote ) )
  {
    return -1 ;
  }
#endif

  /**/
  ssize_t width = data->width ;
  ssize_t precision = data->precision ;
  if ( precision < 0 )
  {
    precision = data->precision = SIZE_MAX >> 1 ;
  }
  size_t offset = 0 ;
#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1
  int repeatOrReverseOrOffsetOrMultiParts = data->repeatOrReverseOrOffsetOrMultiParts ;
  if ( repeatOrReverseOrOffsetOrMultiParts )
  {
    if ( 0 != forma_conv_hex_WriteOffset( context, writeChar, offset ) )
    {
      return 0 ;
    }
  }
#endif
  char hex10 = data->hex10 ;
  while ( width > 0 )
  {
    int hexValue = *hexData++ ;
    offset++ ;

    unsigned count = 2 ;
    while ( count-- > 0 )
    {
      int c = ( char ) ( ( hexValue >> 4 ) & 0x0F ) ;
      if ( c <= 9 )
      {
        c += ( char ) ( '0' & 0xFF ) ;
      }
      else
      {
        c += ( char ) ( ( hex10 - 10 ) & 0xFF ) ;
      }

      if ( 0 != writeChar( context, ( char ) ( c & 0xFF ) ) )
      {
        return -1 ;
      }

      hexValue <<= 4 ;
    }

    if ( width > 1 )
    {
      if ( 0 == --precision )
      {
        precision = data->precision ;
        if ( 0 != writeChar( context, '\n' ) )
        {
          return -1 ;
        }
#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1
        if ( repeatOrReverseOrOffsetOrMultiParts )
        {
          if ( 0 != forma_conv_hex_WriteOffset( context, writeChar, offset ) )
          {
            return -1 ;
          }
        }
#endif
      }
      else if ( separator && 0 != forma_WriteString( context, writeChar, separator ) )
      {
        return -1 ;
      }
    }

    width-- ;
  }

#if FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED == 1
  /**/
  if ( artificialQuote && 0 != writeChar( context, artificialQuote ) )
  {
    return -1 ;
  }
#endif

  return 0 ;
}

static const forma_conv_state_definition_t* forma_conv_hex_StateArray ( forma_conv_data_t* data )
{
  forma_context_t* context = data->context ;

  /* get value */
  const uint8_t* hex = va_arg( context->ap, uint8_t* ) ;

  /* process value */
#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
  if ( data->expressionValue )
#endif
  {
    const char* separator = data->featureColon ? ":" : 0 ;

    if ( forma_conv_hex_WriteArray( data, hex, separator ) )
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

static const forma_conv_state_definition_t* forma_conv_hex_StateDump ( forma_conv_data_t* data )
{
  forma_context_t* context = data->context ;

  /* get values */
  uint8_t* hex = va_arg( context->ap, uint8_t* ) ;
  const char* separator = va_arg( context->ap, char* ) ;

  /* process value */
#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
  if ( data->expressionValue )
#endif
  {
    if ( forma_conv_hex_WriteArray( data, hex, separator ) )
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

#endif /* FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED */

static const forma_conv_state_definition_t* forma_conv_hex_StateInt ( forma_conv_data_t* data )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

  /* get value */
#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 0
  uint32_t value ;
  ssize_t length = 32 ;
#else
  uint64_t value ;
  ssize_t length = data->length ;
  if ( 32 == length )
#endif /* FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED */
  {
    value = va_arg( context->ap, unsigned ) ;
  }
#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 1
  else
  {
    value = va_arg( context->ap, uint64_t ) ;
  }
#endif /* FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED */

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
    const char* forcedPrefix = data->forcedPrefix ;
    if ( forcedPrefix && 0 != forma_WriteString( context, writeChar, forcedPrefix ) )
    {
      return 0 ;
    }

    /**/
    ssize_t count = length >> 2 ;
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
    ssize_t precision = data->precision ;
    if ( precision < 0 )
    {
      precision = SIZE_MAX >> 1 ;
    }
    ssize_t width = data->width ;
    if ( 0 == value )
    {
      count-- ;
      if ( width > 0 )
      {
        width-- ;
      }
    }
    char filler = data->filler ;
    ssize_t fillAtEnd = 0 ;
#endif
    uint64_t mask = 32 == length ? ( ( uint64_t ) 0xF << ( 32 - 4 ) ) : ( ( uint64_t ) 0xF << ( 64 - 4 ) ) ;
    while ( count > 0 )
    {
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
      if ( count <= precision && 0 != ( value & mask ) )
#else
      if ( 0 != ( value & mask ) )
#endif
      {
        break ;
      }

      value <<= 4 ;
      count-- ;

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
      if ( 0 != data->leftOriented )
      {
        fillAtEnd++ ;
      }
      else if ( width > count )
      {
        if ( 0 != writeChar( context, filler ) )
        {
          return 0 ;
        }
      }
#endif
    }

    /**/
    if ( 0 == value )
    {
      if ( 0 != writeChar( context, '0' ) )
      {
        return 0 ;
      }
    }
    else
    {
      char hex10 = data->hex10 ;
      while ( count-- > 0 )
      {
        int c = ( char ) ( ( value >> ( length - 4 ) ) & 0x0F ) ;
        if ( c <= 9 )
        {
          c += ( char ) ( '0' & 0xFF ) ;
        }
        else
        {
          c += ( char ) ( ( hex10 - 10 ) & 0xFF ) ;
        }

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
        if ( count < precision )
        {
          if ( 0 != writeChar( context, ( char ) ( c & 0xFF ) ) )
          {
            return 0 ;
          }
        }
#endif

        value <<= 4 ;
      }
    }

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
    /**/
    while ( fillAtEnd-- > 0 )
    {
      if ( 0 != writeChar( context, ' ' ) )
      {
        return 0 ;
      }
    }
#endif

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

#endif /* FORMA_SUPPORT_CONV_HEX_ENABLED */
