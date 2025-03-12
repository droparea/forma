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
#include "forma_conv_net.h"

#if FORMA_SUPPORT_CONV_NET_ENABLED == 1

static const forma_conv_state_definition_t* forma_conv_net_StateIp ( forma_conv_data_t* data ) ;

const forma_conv_state_definition_t forma_conv_net_STATE_IP = {  //
    .run = forma_conv_net_StateIp, /**/
    } ;

static int forma_conv_net_WriteIpv4Byte ( forma_conv_data_t* data, uint8_t byte )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;
  char buffer[ sizeof ( "255" ) ] ;

  if ( byte > 0 )
  {
    unsigned bufferFilled = 0 ;
    char* bufferPtr = buffer ;
    while ( byte > 0 )
    {
      bufferFilled++ ;
      unsigned newByte = byte / 10 ;
      *bufferPtr++ = ( char ) ( '0' + ( byte - newByte * 10 ) ) ;
      byte = newByte ;
    }

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
    if ( bufferFilled < 3 )
    {
      ssize_t precision = data->precision ;
      if ( precision > 0 )
      {
        if ( precision > 3 )
        {
          precision = 3 ;
        }
        precision -= bufferFilled ;
        while ( precision-- > 0 )
        {
          if ( 0 != writeChar( context, '0' ) )
          {
            return -1 ;
          }
        }
      }
    }
#endif

    while ( bufferFilled-- > 0 )
    {
      if ( 0 != writeChar( context, *--bufferPtr ) )
      {
        return -1 ;
      }
    }
  }
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
  else if ( data->precision > 0 )
  {
    unsigned count = 3 ;
    while ( count-- > 0 )
    {
      if ( 0 != writeChar( context, '0' ) )
      {
        return -1 ;
      }
    }
  }
#endif
  else if ( 0 != writeChar( context, '0' ) )
  {
    return -1 ;
  }

  return 0 ;
}

static const forma_conv_state_definition_t* forma_conv_net_StateIp ( forma_conv_data_t* data )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

  /**/
  unsigned count = sizeof(uint32_t) ; // TODO add IPv6 support

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

    unsigned byte ;
    if ( count == 4 )
    {
      uint32_t value = va_arg( context->ap, uint32_t ) ;

#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1
      if ( 1 == data->repeatOrReverseOrOffsetOrMultiParts )
      {
        while ( count-- > 0 )
        {
          byte = ( uint8_t ) ( ( value >> 24 ) & 0xFF ) ;
          value <<= 8 ;

          if ( 0 != forma_conv_net_WriteIpv4Byte( data, byte ) )
          {
            return 0 ;
          }

          if ( count > 0 && 0 != writeChar( context, '.' ) )
          {
            return 0 ;
          }
        }
      }
      else
#endif
      {
        while ( count-- > 0 )
        {
          byte = ( uint8_t ) ( value & 0xFF ) ;
          value >>= 8 ;

          if ( 0 != forma_conv_net_WriteIpv4Byte( data, byte ) )
          {
            return 0 ;
          }

          if ( count > 0 && 0 != writeChar( context, '.' ) )
          {
            return 0 ;
          }
        }
      }
    }
    else
    {
      return 0 ;
    }

#if FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED == 1
    if ( artificialQuote && 0 != writeChar( context, artificialQuote ) )
    {
      return 0 ;
    }
#endif
  }
#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
  else
  {
    if ( count == 4 )
    {
      ( void ) va_arg( context->ap, uint32_t ) ;
    }

    if ( 0 != forma_conv_str_WriteSqlNull( context, writeChar ) )
    {
      return 0 ;
    }
  }
#endif

  return &forma_conv_STATE_FINISHED ;
}

#endif /* FORMA_SUPPORT_CONV_NET_ENABLED */
