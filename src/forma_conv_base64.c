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
#include "forma_conv_base64.h"

#if FORMA_SUPPORT_CONV_BASE64_ENABLED == 1

static const forma_conv_state_definition_t* forma_conv_base64_StateArray ( forma_conv_data_t* data ) ;

static const forma_conv_state_definition_t* forma_conv_base64_StatePEM ( forma_conv_data_t* data ) ;

const forma_conv_state_definition_t forma_conv_base64_STATE_ARRAY = {  //
    .run = forma_conv_base64_StateArray, /**/
    } ;

const forma_conv_state_definition_t forma_conv_base64_STATE_PEM = {  //
    .run = forma_conv_base64_StatePEM, /**/
    } ;

static char forma_conv_base64_WriteChar ( unsigned value )
{
  if ( value <= 25 )
  {
    return ( char ) ( 'A' + value ) ;
  }
  else if ( value <= 51 )
  {
    return ( char ) ( 'a' + ( value - 26 ) ) ;
  }
  else if ( value <= 61 )
  {
    return ( char ) ( '0' + ( value - 52 ) ) ;
  }
  else if ( value == 62 )
  {
    return '+' ;
  }
  else if ( value == 63 )
  {
    return '/' ;
  }
  else
  {
    return '=' ;
  }
}

static int forma_conv_base64_WriteArray ( forma_conv_data_t* data, ssize_t* lineCharsOut )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

  /**/
  size_t width = data->width ;
#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1
  size_t countParts ;
  if ( data->repeatOrReverseOrOffsetOrMultiParts )
  {
    countParts = width ;
    if ( 0 == countParts )
    {
      return 0 ;
    }

    width = ( size_t ) va_arg( context->ap, unsigned ) ;
  }
  else
  {
    countParts = 1 ;
  }
#endif /* FORMA_SUPPORT_FLAG_TILDE_ENABLED */

  /**/
  uint8_t* buffer = va_arg( context->ap, uint8_t* ) ;
#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
  if ( !data->expressionValue )
  {
#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1
    while ( --countParts > 0 )
    {
      width = va_arg( context->ap, unsigned ) ;
      buffer = va_arg( context->ap, uint8_t* ) ;
    }
#endif

    return 0 ;
  }
#endif /* FORMA_SUPPORT_CONV_STR_SQL_ENABLED */

  /**/
  ssize_t maxCharsPerLine = 64 ;
  ssize_t precision = data->precision ;
  if ( precision > 0 )
  {
    maxCharsPerLine = precision ;
    if ( 0 == maxCharsPerLine )
    {
      maxCharsPerLine = 64 ;
    }
  }
  ssize_t bytes = 0 ;
  ssize_t lastByte = 0 ;
  ssize_t lineChars = 0 ;
  uint8_t byte = 0 ;
#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1
  countParts-- ;
#endif
  while ( bytes < 4 )
  {
#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1
    if ( countParts > 0 && 0 == width )
    {
      countParts-- ;
      width = va_arg( context->ap, unsigned ) ;
      buffer = va_arg( context->ap, uint8_t* ) ;
    }
#endif

    if ( bytes < 3 )
    {
      if ( width > 0 )
      {
        byte = *buffer++ ;
      }
      else
      {
        byte = 0 ;
      }
    }

    switch ( ++bytes )
    {
      case 1:
      {
        if ( 0 != writeChar( context, forma_conv_base64_WriteChar( ( unsigned ) ( byte >> 2 ) ) ) )
        {
          return 0 ;
        }

        /* store last byte */
        lastByte = ( uint8_t ) ( ( byte & 0x03 ) << 4 ) ;

        /* increase line characters */
        lineChars++ ;

        break ;
      }

      case 2:
      {
        if ( 0 != writeChar( context, forma_conv_base64_WriteChar( ( unsigned ) ( lastByte | byte >> 4 ) ) ) )
        {
          return 0 ;
        }

        /* store last byte */
        if ( 0 == width )
        {
          lastByte = 0xFF ;
        }
        else
        {
          lastByte = ( uint8_t ) ( ( byte & 0x0F ) << 2 ) ;
        }

        /* increase line characters */
        lineChars++ ;

        break ;
      }

      case 3:
      {
        if ( 0 != writeChar( context, forma_conv_base64_WriteChar( ( unsigned ) ( lastByte | byte >> 6 ) ) ) )
        {
          return 0 ;
        }

        /* increase line characters */
        lineChars++ ;

        break ;
      }

      case 4:
      {
        if ( 0 == width )
        {
          if ( 0 != writeChar( context, '=' ) )
          {
            return 0 ;
          }
        }
        else if ( 0 != writeChar( context, forma_conv_base64_WriteChar( ( unsigned ) ( byte & 0x3F ) ) ) )
        {
          return 0 ;
        }

        /* increase line characters */
        lineChars++ ;

        /* reset bytes */
#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1
        if ( width > 1 || countParts > 0 )
#else
        if ( width > 1 )
#endif
        {
          bytes = 0 ;
        }

        break ;
      }

      default:
      {
        return 0 ;
      }
    }

    /**/
    if ( 3 != bytes && width > 0 )
    {
      width-- ;
    }

    /* insert line feed */
    if ( maxCharsPerLine == lineChars && ( width > 0 || bytes < 4 ) )
    {
      if ( 0 != forma_WriteString( context, writeChar, FORMA_NEW_LINE ) )
      {
        return 0 ;
      }
      lineChars = 0 ;
    }
  }

  if ( lineCharsOut )
  {
    *lineCharsOut = lineChars ;
  }

  return 0 ;
}

static const forma_conv_state_definition_t* forma_conv_base64_StateArray ( forma_conv_data_t* data )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

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
    if ( 0 != forma_conv_base64_WriteArray( data, 0 ) )
    {
      return 0 ;
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
  else
  {
    if ( 0 != forma_conv_base64_WriteArray( data, 0 ) )
    {
      return 0 ;
    }

    if ( 0 != forma_conv_str_WriteSqlNull( context, writeChar ) )
    {
      return 0 ;
    }
  }
#endif

  return &forma_conv_STATE_FINISHED ;
}

static const forma_conv_state_definition_t* forma_conv_base64_StatePEM ( forma_conv_data_t* data )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

  /**/
  const char* addonValue = va_arg( context->ap, const char* ) ;

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

    /* write begin */
    if ( 0 != forma_WriteString( context, writeChar, "-----BEGIN" ) )
    {
      return 0 ;
    }
    if ( addonValue )
    {
      if ( 0 != writeChar( context, ' ' ) )
      {
        return 0 ;
      }
      if ( 0 != forma_WriteString( context, writeChar, addonValue ) )
      {
        return 0 ;
      }
    }
    if ( 0 != forma_WriteString( context, writeChar, "-----" FORMA_NEW_LINE ) )
    {
      return 0 ;
    }

    /**/
    ssize_t lineChars ;
    if ( 0 != forma_conv_base64_WriteArray( data, &lineChars ) )
    {
      return 0 ;
    }

    /* insert line feed */
    if ( 0 != lineChars )
    {
      if ( 0 != forma_WriteString( context, writeChar, FORMA_NEW_LINE ) )
      {
        return 0 ;
      }
    }

    /* write end */
    if ( 0 != forma_WriteString( context, writeChar, "-----END" ) )
    {
      return 0 ;
    }
    if ( addonValue )
    {
      if ( 0 != writeChar( context, ' ' ) )
      {
        return 0 ;
      }
      if ( 0 != forma_WriteString( context, writeChar, addonValue ) )
      {
        return 0 ;
      }
    }
    if ( 0 != forma_WriteString( context, writeChar, "-----" ) )
    {
      return 0 ;
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
  else
  {
    if ( 0 != forma_conv_base64_WriteArray( data, 0 ) )
    {
      return 0 ;
    }

    if ( 0 != forma_conv_str_WriteSqlNull( context, writeChar ) )
    {
      return 0 ;
    }
  }
#endif

  return &forma_conv_STATE_FINISHED ;
}

#endif /* FORMA_SUPPORT_CONV_BASE64_ENABLED */
