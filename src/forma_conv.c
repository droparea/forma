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

#include "forma_conv.h"
#include "forma_conv_base64.h"
#include "forma_conv_char.h"
#include "forma_conv_float.h"
#include "forma_conv_function.h"
#include "forma_conv_hex.h"
#include "forma_conv_int.h"
#include "forma_conv_net.h"
#include "forma_conv_str.h"
#include "forma_conv_time.h"

static const forma_conv_state_definition_t* forma_conv_FormatToState ( forma_conv_data_t* data, char c ) ;

static const forma_conv_state_definition_t* forma_conv_StateIdentify ( forma_conv_data_t* data ) ;

static const forma_conv_state_definition_t* forma_conv_StateCustomFiller ( forma_conv_data_t* data ) ;

#if FORMA_SUPPORT_FLAG_PATTERN_ENABLED == 1
static const forma_conv_state_definition_t* forma_conv_StatePattern ( forma_conv_data_t* data ) ;
#endif

static const forma_conv_state_definition_t* forma_conv_StateWidth ( forma_conv_data_t* data ) ;

static const forma_conv_state_definition_t* forma_conv_StatePrecision ( forma_conv_data_t* data ) ;

#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 1
static const forma_conv_state_definition_t* forma_conv_StateLengthModifier ( forma_conv_data_t* data ) ;
#endif

static const forma_conv_state_definition_t* forma_conv_StateConversionEscape ( forma_conv_data_t* data ) ;

const forma_conv_state_definition_t forma_conv_STATE_FINISHED = { //
    .run = 0, /**/
    } ;

static const forma_conv_state_definition_t forma_conv_STATE_IDENTIFY = { //
    .run = forma_conv_StateIdentify, /**/
    } ;

#if FORMA_SUPPORT_FLAG_PATTERN_ENABLED == 1
static const forma_conv_state_definition_t forma_conv_STATE_PATTERN = { //
    .run = forma_conv_StatePattern, /**/
    } ;
#endif

static const forma_conv_state_definition_t forma_conv_STATE_CUSTOM_FILLER = { //
    .run = forma_conv_StateCustomFiller, /**/
    } ;

static const forma_conv_state_definition_t forma_conv_STATE_WIDTH = { //
    .run = forma_conv_StateWidth, /**/
    } ;

static const forma_conv_state_definition_t forma_conv_STATE_PRECISION = { //
    .run = forma_conv_StatePrecision, /**/
    } ;

#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 1
static const forma_conv_state_definition_t forma_conv_STATE_LENGTH_MODIFIER = { //
    .run = forma_conv_StateLengthModifier, /**/
    } ;
#endif

static const forma_conv_state_definition_t forma_conv_STATE_CONVERSION_ESCAPE = { //
    .run = forma_conv_StateConversionEscape, /**/
    } ;

static const forma_conv_state_definition_t* forma_conv_StateWidth ( forma_conv_data_t* data )
{
  forma_context_t* context = data->context ;

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
  size_t width = 0 ;
#endif
  while ( 1 )
  {
    char c = *data->fmt++ ;
    if ( 0 == c )
    {
      return 0 ;
    }

    if ( c >= '0' && c <= '9' )
    {
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
      width *= 10 ;
      width += ( unsigned ) ( c - '0' ) ;
#endif
      continue ;
    }
    if ( '*' == c )
    {
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
      width = ( size_t ) va_arg( context->ap, unsigned ) ;
#else
      ( void ) va_arg( context->ap, unsigned ) ;
#endif
      c = *data->fmt++ ;
    }
    if ( '.' == c )
    {
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
      data->width = width ;
#endif

      return &forma_conv_STATE_PRECISION ;
    }

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
    data->width = width ;
#endif

    return forma_conv_FormatToState( data, c ) ;
  }
}

static const forma_conv_state_definition_t* forma_conv_StatePrecision ( forma_conv_data_t* data )
{
  forma_context_t* context = data->context ;
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
  size_t precision = 0 ;
#endif
  while ( 1 )
  {
    char c = *data->fmt++ ;
    if ( 0 == c )
    {
      return 0 ;
    }

    if ( c >= '0' && c <= '9' )
    {
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
      precision *= 10 ;
      precision += ( unsigned ) ( c - '0' ) ;
#endif
      continue ;
    }
    if ( '*' == c )
    {
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
      precision = ( size_t ) va_arg( context->ap, unsigned ) ;
#else
      ( void ) va_arg( context->ap, unsigned ) ;
#endif
      c = *data->fmt++ ;
    }

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
    data->precision = precision ;
#endif

    return forma_conv_FormatToState( data, c ) ;
  }
}

#if FORMA_SUPPORT_FLAG_PATTERN_ENABLED == 1

static const forma_conv_state_definition_t* forma_conv_StatePattern ( forma_conv_data_t* data )
{
  if ( data->pattern )
  {
    return 0 ;
  }

  data->pattern = data->fmt ;
  char c ;
  while ( ( c = *data->fmt++ ) )
  {
    if ( '\\' == c )
    {
      c = *data->fmt++ ;
      if ( 0 == c )
      {
        return 0 ;
      }
    }
    else if ( '_' == c )
    {
      return &forma_conv_STATE_IDENTIFY ;
    }
    else if ( '?' == c )
    {
      data->patternPlaceholders++ ;
    }

    data->patternLength++ ;
  }

  return 0 ;
}

#endif /* FORMA_SUPPORT_FLAG_PATTERN_ENABLED */

static const forma_conv_state_definition_t* forma_conv_StateCustomFiller ( forma_conv_data_t* data )
{
  char c = *data->fmt++ ;

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1 || FORMA_SUPPORT_CONV_TIME_ENABLED == 1
  data->filler = c ;
  if ( 0 == data->filler )
  {
    return 0 ;
  }
#else
  ( void ) c ;
#endif

  return &forma_conv_STATE_IDENTIFY ;
}

static const forma_conv_state_definition_t* forma_conv_StateIdentify ( forma_conv_data_t* data )
{
  // TODO add "I"
  // TODO add " "
  char c = *data->fmt++ ;
  switch ( c )
  {
    case 0:
    {
      return 0 ;
    }

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1

    case '?':
    {
      data->expressionValue = va_arg( data->context->ap, int ) ;
      break ;
    }

#endif /* FORMA_SUPPORT_CONV_STR_SQL_ENABLED */

    case '-':
    {
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
      data->leftOriented = 1 ;
#endif
      break ;
    }

#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1

    case '~':
    {
      data->repeatOrReverseOrOffsetOrMultiParts = 1 ;
      break ;
    }

#endif /* FORMA_SUPPORT_FLAG_TILDE_ENABLED */

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1

    case '.':
    {
      return &forma_conv_STATE_PRECISION ;
    }

#endif /* FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED */

    case '\'':
    case '"':
    {
      data->quote = c ;
#if FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED == 1
      data->artificialQuote = c ;
#endif

      break ;
    }

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1

    case '*':
    {
      fflush( stdout ) ;
      data->fmt-- ;
      return &forma_conv_STATE_WIDTH ;
    }

#endif /* FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED */

    case '=':
    {
      return &forma_conv_STATE_CUSTOM_FILLER ;
    }

#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1

    case '|':
    {
      forma_context_t* context = data->context ;
      data->mapChar = va_arg( context->ap, forma_MapChar_t* ) ;
      data->mapCharContext = va_arg( context->ap, void* ) ;

      break ;
    }

#endif /* FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED */

#if FORMA_SUPPORT_FLAG_PATTERN_ENABLED == 1

    case '_':
    {
      return &forma_conv_STATE_PATTERN ;
    }

#endif /* FORMA_SUPPORT_FLAG_PATTERN_ENABLED */

#if FORMA_SUPPORT_CONV_HEX_ENABLED == 1

    case ':':
    {
      data->featureColon = 1 ;
      break ;
    }

    case '#':
    {
      data->forcedPrefix = "0x" ;
      break ;
    }

#endif /* FORMA_SUPPORT_CONV_HEX_ENABLED */

    default:
    {
      if ( c >= '0' && c <= '9' )
      {
        if ( '0' == c )
        {
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1 || FORMA_SUPPORT_CONV_TIME_ENABLED == 1
          data->filler = '0' ;
#endif
        }
        else
        {
          data->fmt-- ;
        }

        return &forma_conv_STATE_WIDTH ;
      }

      return forma_conv_FormatToState( data, c ) ;
    }
  }

  return &forma_conv_STATE_IDENTIFY ;
}

static const forma_conv_state_definition_t* forma_conv_FormatToState ( forma_conv_data_t* data, char c )
{
  ( void ) data ; /* NOTICE: suppress warnings */

  switch ( c )
  {
    case '%':
    {
      return &forma_conv_STATE_CONVERSION_ESCAPE ;
    }

#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 1

    case 'l':
    case 'z':
    {
      return &forma_conv_STATE_LENGTH_MODIFIER ;
    }

#endif

#if 0 // TODO implement me - see printf

    case 'n':
    {
    }

#endif

#if 0 // TODO implement me - see printf

    case 'm':
    {
    }

#endif

#if FORMA_SUPPORT_CONV_STR_ENABLED == 1

    case 's':
    {
      return &forma_conv_str_STATE ;
    }

#if 0 // TODO implement me - see printf

    case 'S':
    {
    }

#endif

#if FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1

    case 'E': // TODO conflict with printf
    {
      return &forma_conv_str_STATE_SQL ;
    }

#endif

#if FORMA_SUPPORT_CONV_STR_JSON_ENABLED == 1

    case 'J':
    {
      return &forma_conv_str_STATE_JSON ;
    }

#endif

#endif /* FORMA_SUPPORT_CONV_STR_ENABLED */

#if FORMA_SUPPORT_CONV_HEX_ENABLED == 1

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1

    case 'h': // TODO conflict with printf (length modifier)
    {
      data->hex10 = 'a' ;
      return &forma_conv_hex_STATE_ARRAY ;
    }

    case 'H':
    {
      return &forma_conv_hex_STATE_ARRAY ;
    }

    case 'D':
    {
      return &forma_conv_hex_STATE_DUMP ;
    }

#endif /* FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED */

    case 'x':
    {
      data->hex10 = 'a' ;
      return &forma_conv_hex_STATE_INT ;
    }

    case 'X':
    {
      return &forma_conv_hex_STATE_INT ;
    }

    case 'p':
    {
#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 1
      data->length = sizeof(void*) * 8 ;
#endif
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
      data->filler = '0' ;
      if ( data->width < 0 )
      {
        data->width = sizeof(void*) << 1 ;
      }
      if ( data->precision < 0 )
      {
        data->precision = sizeof(void*) << 1 ;
      }
#endif

      return &forma_conv_hex_STATE_INT ;
    }

#endif /* FORMA_SUPPORT_CONV_HEX_ENABLED */

#if FORMA_SUPPORT_CONV_INT_ENABLED == 1

    case 'o':
    {
      data->divider = 8 ;
      return &forma_conv_int_STATE_UNSIGNED ;
    }

    case 'd':
    case 'i':
    {
      return &forma_conv_int_STATE_SIGNED ;
    }

    case 'u':
    {
      return &forma_conv_int_STATE_UNSIGNED ;
    }

#endif /* FORMA_SUPPORT_CONV_INT_ENABLED */

#if FORMA_SUPPORT_CONV_CHAR_ENABLED == 1

    case 'c':
    {
      return &forma_conv_char_STATE_VALUE ;
    }

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
    case 'C': // TODO conflict with printf
    {
      return &forma_conv_char_STATE_ARRAY ;
    }
#endif

#endif /* FORMA_SUPPORT_CONV_CHAR_ENABLED */

#if FORMA_SUPPORT_CONV_BASE64_ENABLED == 1

    case 'P':
    {
      return &forma_conv_base64_STATE_PEM ;
    }

    case 'B':
    {
      return &forma_conv_base64_STATE_ARRAY ;
    }

#endif /* FORMA_SUPPORT_CONV_BASE64_ENABLED */

#if FORMA_SUPPORT_CONV_NET_ENABLED == 1

    case 'I': // TODO conflict with printf
    {
      return &forma_conv_net_STATE_IP ;
    }

#endif

#if FORMA_SUPPORT_CONV_FLOAT_ENABLED == 1

    case 'f':
      // TODO conflict with function: - case 'F':
    case 'g':
    case 'G':
    case 'e':
      // TODO conflict with SQL - case 'E':
    case 'a':
    case 'A':
    {
      return &forma_conv_float_STATE ;
    }

#endif /* FORMA_SUPPORT_CONV_FLOAT_ENABLED */

#if FORMA_SUPPORT_CONV_TIME_ENABLED == 1

    case 'T':
    {
      return &forma_conv_time_STATE ;
    }

#endif /* FORMA_SUPPORT_CONV_TIME_ENABLED */

#if FORMA_SUPPORT_CONV_FUNCTION_ENABLED == 1

    case 'F': // TODO conflict with printf
    {
      return &forma_conv_function_STATE ;
    }

#endif

    default:
    {
      return 0 ;
    }
  }
}

static const forma_conv_state_definition_t* forma_conv_StateConversionEscape ( forma_conv_data_t* data )
{
  if ( 0 != data->writeChar( data->context, '%' ) )
  {
    return 0 ;
  }

  return &forma_conv_STATE_FINISHED ;
}

#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 1

static const forma_conv_state_definition_t* forma_conv_StateLengthModifier ( forma_conv_data_t* data )
{
  size_t length = 0 ;
  while ( 1 )
  {
    char c = *data->fmt++ ;
    if ( 0 == c )
    {
      return 0 ;
    }

    // TODO add "hh" -> conflict with conversion
    // TODO add "h" -> conflict with conversion
    // TODO add "q"
    // TODO add "L"
    // TODO add "j"
    // TODO add "Z"
    // TODO add "t"
    switch ( c )
    {
      case 'l':
      {
        if ( length >= 64 )
        {
          return 0 ;
        }
        length += 32 ;
        continue ;
      }

      case 'z':
      {
        if ( 0 != length )
        {
          length = sizeof(size_t) * 8 ;
        }

        break ;
      }

      default:
      {
        break ;
      }
    }

    data->length = length ;

    return forma_conv_FormatToState( data, c ) ;
  }
}

#endif /* FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED */

const char* forma_conv_Process ( forma_context_t* context, forma_WriteChar_t* writeChar, const char* fmt )
{
  forma_conv_data_t data = { //
      .context = context, /**/
      .writeChar = writeChar, /**/
      .fmt = fmt, /**/
#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 1
          .length = 32, /**/
#endif
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
          .width = -1, /**/
          .precision = -1, /**/
#endif
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
          .leftOriented = 0, /**/
#endif
#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1
          .repeatOrReverseOrOffsetOrMultiParts = 0, /**/
#endif
#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1
          .mapChar = 0, /**/
          .mapCharContext = 0, /**/
#endif
#if FORMA_SUPPORT_FLAG_PATTERN_ENABLED == 1
          .pattern = 0, /**/
          .patternLength = 0, /**/
          .patternPlaceholders = 0, /**/
#endif
#if FORMA_SUPPORT_CONV_INT_ENABLED == 1
          .divider = 10, /**/
#endif
#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1 || FORMA_SUPPORT_CONV_TIME_ENABLED == 1
          .filler = ' ', /**/
#endif
#if FORMA_SUPPORT_CONV_STR_ENABLED == 1 && FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1
          .expressionValue = 1, /**/
          .quote = '\'', /**/
#endif
#if FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED == 1
          .artificialQuote = 0, /**/
#endif
#if FORMA_SUPPORT_CONV_HEX_ENABLED == 1
          .featureColon = 0, /**/
          .hex10 = 'A', /**/
          .forcedPrefix = 0, /**/
#endif
      } ;
  forma_conv_StateRun_t* stateRun = forma_conv_StateIdentify ;
  do
  {
    const forma_conv_state_definition_t* stateDefinition = stateRun( &data ) ;
    if ( 0 == stateDefinition )
    {
      return 0 ;
    }

    stateRun = stateDefinition->run ;
  }
  while ( stateRun ) ;

  return data.fmt ;
}
