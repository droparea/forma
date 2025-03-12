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
#include <time.h>
#include "forma_private.h"
#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
#include "forma_conv_str.h"
#endif
#include "forma_conv_time.h"

#if FORMA_SUPPORT_CONV_TIME_ENABLED == 1

static const forma_conv_state_definition_t* forma_conv_time_State ( forma_conv_data_t* data ) ;

const forma_conv_state_definition_t forma_conv_time_STATE = {  //
    .run = forma_conv_time_State, /**/
    } ;

static int forma_conv_time_WriteNumber ( forma_conv_data_t* data, unsigned value, unsigned count )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

  /**/
  char buffer[ sizeof ( "1234" ) ] ;
  if ( count >= sizeof ( buffer ) )
  {
    count = sizeof ( buffer ) - 1 ;
  }

  /**/
  char* bufferPtr = buffer ;
  unsigned bufferFilled = 0 ;
  if ( 0 == value )
  {
    bufferFilled++ ;
    *bufferPtr++ = '0' ;
  }
  else
  {
    while ( value > 0 )
    {
      if ( count > 0 )
      {
        count-- ;
      }

      bufferFilled++ ;
      unsigned newValue = value / 10 ;
      *bufferPtr++ = ( char ) ( '0' + ( value - newValue * 10 ) ) ;
      value = newValue ;
    }
  }
  char filler = data->filler ;
  while ( count-- > 0 )
  {
    bufferFilled++ ;
    *bufferPtr++ = filler ;
  }

  /**/
  bufferPtr = buffer + bufferFilled ;
  while ( bufferFilled-- > 0 )
  {
    if ( 0 != writeChar( context, *--bufferPtr ) )
    {
      return 0 ;
    }
  }

  return 0 ;
}

static int forma_conv_time_WriteValue ( forma_conv_data_t* data, const struct tm* tm, char c, unsigned count )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

  switch ( c )
  {
    case 0:
    {
      break ;
    }

    case 'd':
    {
      if ( 0 != forma_conv_time_WriteNumber( data, ( unsigned ) tm->tm_mday, count ) )
      {
        return 0 ;
      }

      break ;
    }

    case 'M':
    {
      if ( 0 != forma_conv_time_WriteNumber( data, ( unsigned ) tm->tm_mon + 1, count ) )
      {
        return 0 ;
      }

      break ;
    }

    case 'y':
    {
      if ( 0 != forma_conv_time_WriteNumber( data, ( unsigned ) tm->tm_year + 1900, count ) )
      {
        return 0 ;
      }

      break ;
    }

    case 'h':
    {
      unsigned value = ( unsigned ) tm->tm_hour ;
      if ( 0 == value )
      {
        value = 12 ;
      }
      else if ( value > 12 )
      {
        value -= 12 ;
      }
      if ( 0 != forma_conv_time_WriteNumber( data, value, count ) )
      {
        return 0 ;
      }

      break ;
    }

    case 'H':
    {
      if ( 0 != forma_conv_time_WriteNumber( data, ( unsigned ) tm->tm_hour, count ) )
      {
        return 0 ;
      }

      break ;
    }

    case 'm':
    {
      if ( 0 != forma_conv_time_WriteNumber( data, ( unsigned ) tm->tm_min, count ) )
      {
        return 0 ;
      }

      break ;
    }

    case 's':
    {
      if ( 0 != forma_conv_time_WriteNumber( data, ( unsigned ) tm->tm_sec, count ) )
      {
        return 0 ;
      }

      break ;
    }

    default:
    {
      while ( count-- > 0 )
      {
        if ( 0 != writeChar( context, c ) )
        {
          return 0 ;
        }
      }

      break ;
    }
  }

  return 0 ;
}

static const forma_conv_state_definition_t* forma_conv_time_State ( forma_conv_data_t* data )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

  /**/
  const struct tm* tm = va_arg( context->ap, const struct tm* ) ;

  /* process value */
#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
  if ( data->expressionValue && 0 != tm )
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
#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 0
    if ( 0 != tm )
#endif
    {
      /* select pattern */
      const char* pattern = data->pattern ;
      size_t patternLength ;
      if ( 0 == pattern )
      {
        pattern = FORMA_TIME_DEFAULT_PATTERN ;
        patternLength = sizeof ( FORMA_TIME_DEFAULT_PATTERN - 1 ) ;
      }
      else
      {
        patternLength = data->patternLength ;
      }

      /* process pattern */
      char lastC = 0 ;
      unsigned count = 0 ;
      while ( patternLength-- > 0 )
      {
        char c = *pattern++ ;
        if ( lastC == c )
        {
          count++ ;
          continue ;
        }

        if ( 0 != forma_conv_time_WriteValue( data, tm, lastC, count ) )
        {
          return 0 ;
        }

        lastC = c ;
        count = 1 ;
      }
      if ( 0 != forma_conv_time_WriteValue( data, tm, lastC, count ) )
      {
        return 0 ;
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

#endif /* FORMA_SUPPORT_CONV_TIME_ENABLED */
