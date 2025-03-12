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
#include <string.h>
#include "forma_private.h"
#include "forma_conv_function.h"

/* TODO
 *
 * - add UTF-8 support
 * - additional SQL escaping required (at the moment only SQLite is supported)
 */

#if FORMA_SUPPORT_CONV_STR_ENABLED == 1

#ifndef FORMA_CONV_STR_SQL_NULL
#define FORMA_CONV_STR_SQL_NULL    "NULL"
#endif

typedef int forma_conv_str_Writer_t ( forma_conv_data_t* data, char c ) ;

static const forma_conv_state_definition_t* forma_conv_str_State ( forma_conv_data_t* data ) ;

#if FORMA_SUPPORT_CONV_STR_JSON_ENABLED == 1
static const forma_conv_state_definition_t* forma_conv_str_StateJSON ( forma_conv_data_t* data ) ;
#endif

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
static const forma_conv_state_definition_t* forma_conv_str_StateSQL ( forma_conv_data_t* data ) ;
#endif

const forma_conv_state_definition_t forma_conv_str_STATE = {  //
    .run = forma_conv_str_State, /**/
    } ;

#if FORMA_SUPPORT_CONV_STR_JSON_ENABLED == 1
const forma_conv_state_definition_t forma_conv_str_STATE_JSON = {  //
    .run = forma_conv_str_StateJSON, /**/
    } ;
#endif

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
const forma_conv_state_definition_t forma_conv_str_STATE_SQL = {  //
    .run = forma_conv_str_StateSQL, /**/
    } ;
#endif

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1

int forma_conv_str_WriteSqlNull ( forma_context_t* context, forma_WriteChar_t* writeChar )
{
  int result ;

  if ( 0 != ( result = forma_WriteString( context, writeChar, FORMA_CONV_STR_SQL_NULL ) ) )
  {
    return result ;
  }

  return 0 ;
}

#endif /* FORMA_SUPPORT_CONV_STR_SQL_ENABLED */

#if FORMA_SUPPORT_CONV_STR_JSON_ENABLED == 1

static int forma_conv_str_WriteJSON ( forma_conv_data_t* data, char c )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

  switch ( c )
  {
    case '\\':
    case '\"':
    case '\'':
    {
      if ( 0 != writeChar( context, '\\' ) )
      {
        return -1 ;
      }

      break ;
    }

    case '\b':
    {
      if ( 0 != writeChar( context, '\\' ) )
      {
        return -1 ;
      }
      c = 'b' ;

      break ;
    }

    case '\f':
    {
      if ( 0 != writeChar( context, '\\' ) )
      {
        return -1 ;
      }
      c = 'f' ;

      break ;
    }

    case '\n':
    {
      if ( 0 != writeChar( context, '\\' ) )
      {
        return -1 ;
      }
      c = 'n' ;

      break ;
    }

    case '\r':
    {
      if ( 0 != writeChar( context, '\\' ) )
      {
        return -1 ;
      }
      c = 'r' ;

      break ;
    }

    case '\t':
    {
      if ( 0 != writeChar( context, '\\' ) )
      {
        return -1 ;
      }
      c = 't' ;

      break ;
    }

    default:
    {
#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1
      if ( data->repeatOrReverseOrOffsetOrMultiParts )
      {
        /* NOTICE: do nothing */
        break ;
      }
#endif

      unsigned cValue = ( ( unsigned ) c ) & 0xFF ;
      if ( cValue < 32 )
      {
        if ( 0 != forma_WriteString( context, writeChar, "\\u00" ) )
        {
          return -1 ;
        }

        unsigned positions = 2 ;
        while ( positions-- > 0 )
        {
          unsigned nibble = ( cValue & 0xF0 ) >> 4 ;
          if ( nibble >= 10 )
          {
            nibble += 'A' - 10 ;
          }
          else
          {
            nibble += '0' ;
          }
          c = ( char ) ( c << 4 ) ;

          if ( 0 != writeChar( context, ( char ) ( nibble & 0xFF ) ) )
          {
            return -1 ;
          }
        }

        return 0 ;
      }

      break ;
    }
  }

  if ( 0 != writeChar( context, c ) )
  {
    return -1 ;
  }

  return 0 ;
}

#endif /* FORMA_SUPPORT_CONV_STR_JSON_ENABLED */

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1

static int forma_conv_str_WriteSQL ( forma_conv_data_t* data, char c )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

  if ( data->quote == c )
  {
    if ( 0 != writeChar( context, c ) )
    {
      return -1 ;
    }
  }

  if ( 0 != writeChar( context, c ) )
  {
    return -1 ;
  }

  return 0 ;
}

#endif /* FORMA_SUPPORT_CONV_STR_SQL_ENABLED */

static const forma_conv_state_definition_t* forma_conv_str_Write ( forma_conv_data_t* data, forma_conv_str_Writer_t* writer )
{
  forma_context_t* context = data->context ;
  forma_WriteChar_t* writeChar = data->writeChar ;

  /**/
  char* str = va_arg( context->ap, char* ) ;

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
    if ( 0 != str )
    {
      /**/
      size_t strLength = strlen( str ) ;
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
      ssize_t width = data->width ;
      ssize_t precision = data->precision ;
      if ( precision < 0 )
      {
        precision = SIZE_MAX >> 1 ;
        if ( ( size_t ) width < strLength )
        {
          width = 0 ;
        }
      }
      else if ( width < 0 )
      {
        if ( ( size_t ) precision > strLength )
        {
          precision = strLength ;
        }
        width = precision ;
      }
      else if ( width < precision )
      {
        width = precision ;
      }
#endif

      /**/
      char c ;
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
      forma_MapChar_t* mapChar = data->mapChar ;
      void* mapCharContext = data->mapCharContext ;
#endif
#if FORMA_SUPPORT_FLAG_PATTERN_ENABLED == 1
      const char* pattern = data->pattern ;
      if ( pattern )
      {
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
        if ( width > 0 )
        {
          strLength = strnlen( str, width ) ;
        }
#endif

        size_t patternLength = data->patternLength ;
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
        size_t patternPlaceholders = data->patternPlaceholders ;
        int leftOriented = data->leftOriented ;
#endif
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
        char filler = data->filler ;
#else
        char filler = ' ' ;
#endif
#endif /* FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED */
        while ( patternLength-- > 0 )
        {
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
          bool isFiller ;
#endif /* FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED */
          char p = *pattern++ ;
          if ( '?' != p )
          {
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
            isFiller = false ;
#endif /* FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED */
            c = p ;
          }
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
          else if ( 0 == leftOriented && patternPlaceholders-- > strLength )
          {
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
            isFiller = true ;
            c = filler ;
#endif /* FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED */
          }
#endif
          else if ( strLength > 0 )
          {
            strLength-- ;
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
            isFiller = false ;
#endif /* FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED */
            c = *str++ ;
          }
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
          else
          {
            isFiller = true ;
            c = filler ;
          }
#endif /* FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED */

#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
          /* map char */
          if ( mapChar )
          {
            c = mapChar( mapCharContext, isFiller, c ) ;
            if ( 0 == c )
            {
              return 0 ;
            }
          }
#endif /* FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED */

          /* write char */
          if ( writer )
          {
            if ( 0 != writer( data, c ) )
            {
              return 0 ;
            }
          }
          else if ( 0 != writeChar( context, c ) )
          {
            return 0 ;
          }
        }
      }
      else
#endif /* FORMA_SUPPORT_FLAG_PATTERN_ENABLED */
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
      if ( width >= 0 && ( size_t ) width >= ( strLength = width >= 0 ? strnlen( str, width ) : strLength ) )
      {
        if ( precision < width )
        {
          width -= precision ;
        }
        else
        {
          width -= strLength ;
        }

        if ( 0 == data->leftOriented )
        {
          char filler = data->filler ;
          while ( width-- > 0 )
          {
            c = filler ;
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
            if ( mapChar )
            {
              c = mapChar( mapCharContext, true, c ) ;
              if ( 0 == c )
              {
                return 0 ;
              }
            }
#endif /* FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED */
            if ( 0 != writeChar( context, c ) )
            {
              return 0 ;
            }
          }
        }

        while ( precision-- > 0 && ( c = *str++ ) )
        {
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
          /* map char */
          if ( mapChar )
          {
            c = mapChar( mapCharContext, false, c ) ;
            if ( 0 == c )
            {
              return 0 ;
            }
          }
#endif /* FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED */

          /* write char */
          if ( writer )
          {
            if ( 0 != writer( data, c ) )
            {
              return 0 ;
            }
          }
          else if ( 0 != writeChar( context, c ) )
          {
            return 0 ;
          }
        }

        if ( 1 == data->leftOriented )
        {
          char filler = data->filler ;
          while ( width-- > 0 )
          {
            c = filler ;
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
            /* map char */
            if ( mapChar )
            {
              c = mapChar( mapCharContext, true, c ) ;
              if ( 0 == c )
              {
                return 0 ;
              }
            }
#endif

            /* write char */
            if ( writer )
            {
              if ( 0 != writer( data, c ) )
              {
                return 0 ;
              }
            }
            else if ( 0 != writeChar( context, c ) )
            {
              return 0 ;
            }
          }
        }
      }
      else
#endif /* FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED */
      {
        while ( strLength-- > 0 && ( c = *str++ ) )
        {
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
          /* map char */
          if ( mapChar )
          {
            c = mapChar( mapCharContext, false, c ) ;
            if ( 0 == c )
            {
              return 0 ;
            }
          }
#endif

          /* write char */
          if ( writer )
          {
            if ( 0 != writer( data, c ) )
            {
              return 0 ;
            }
          }
          else if ( 0 != writeChar( context, c ) )
          {
            return 0 ;
          }
        }
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

static const forma_conv_state_definition_t* forma_conv_str_State ( forma_conv_data_t* data )
{
  return forma_conv_str_Write( data, 0 ) ;
}

#if FORMA_SUPPORT_CONV_STR_JSON_ENABLED == 1
static const forma_conv_state_definition_t* forma_conv_str_StateJSON ( forma_conv_data_t* data )
{
  return forma_conv_str_Write( data, forma_conv_str_WriteJSON ) ;
}
#endif

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
static const forma_conv_state_definition_t* forma_conv_str_StateSQL ( forma_conv_data_t* data )
{
  return forma_conv_str_Write( data, forma_conv_str_WriteSQL ) ;
}
#endif

#endif /* FORMA_SUPPORT_CONV_STR_ENABLED */
