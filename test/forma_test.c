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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "hapa/include/hapa.h"
#include "../include/forma.h"

/* TODO
 *
 * - change to unit tests
 */

static void forma_test_Function ( forma_object_t* object, void* userData )
{
  unsigned* userDataTyped = ( unsigned* ) userData ;
  unsigned count = *userDataTyped ;
  ( *userDataTyped ) += count ;
  while ( count-- > 0 )
  {
    forma_Object( object, "-%u-", count ) ;
  }
}

static char forma_test_MapChars ( void* context, bool filler, char c )
{
  ( void ) context ;

  if ( filler )
  {
    return '#' ;
  }

  switch ( c )
  {
    case '2':
    {
      return 'X' ;
    }

    case '4':
    {
      return 0 ;
    }
  }

  return c ;
}

static unsigned forma_test_ExecuteWithPrintf ( const char* fmt, ... )
{
  unsigned bytes ;

  printf( "---\n" ) ;
  fflush( stdout ) ;

  va_list ap ;
  va_start( ap, fmt ) ;
  bytes = forma_WriteArgList( fmt, ap ) ;
  vprintf( fmt, ap ) ;
  fflush( stdout ) ;
  va_end( ap ) ;

  return bytes ;
}

static unsigned forma_test_Execute ( const char* fmt, ... )
{
  unsigned bytes ;

  printf( "---\n" ) ;
  fflush( stdout ) ;

  va_list ap ;
  va_start( ap, fmt ) ;
  bytes = forma_WriteArgList( fmt, ap ) ;
  va_end( ap ) ;

  return bytes ;
}

static void forma_test_ExecuteBuffer ( unsigned lineNumber, unsigned bufferSize, const char* fmt, ... )
{
  char buffer[ 64 ] ;
  if ( bufferSize > sizeof ( buffer ) )
  {
    bufferSize = sizeof ( buffer ) ;
  }

  va_list ap ;
  va_start( ap, fmt ) ;
  unsigned bufferFilled = forma_BufferArgList( buffer, bufferSize, fmt, ap ) ;
  va_end( ap ) ;

  forma_test_Execute( "[%5u] BUFFER {%u} '%*.*s'\n", lineNumber, bufferFilled, bufferFilled, bufferFilled, buffer ) ;
}

int main ( void )
{
#if 1
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%u'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%2u'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%-2u'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%2.3u'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%08u'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%0*u'\n", __LINE__, 8, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%8u'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%*u'\n", __LINE__, 8, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%-8u'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%8.10u'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%10.8u'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%*.*u'\n", __LINE__, 10, 8, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%*.8u'\n", __LINE__, 10, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%-10.8u'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%u '%.8u'\n", __LINE__, 12345 ) ;
  forma_test_Execute( "[%5u] %%u '%'u'\n", __LINE__, 12345 ) ;
  forma_test_Execute( "[%5u] %%u '%?'u'\n", __LINE__, 0, 12345 ) ;
#endif

#if 1
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%d'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%d'\n", __LINE__, -12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%2d'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%2d'\n", __LINE__, -12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%-2d'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%-2d'\n", __LINE__, -12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%2.3d'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%2.3d'\n", __LINE__, -12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%08d'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%08d'\n", __LINE__, -12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%8d'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%8d'\n", __LINE__, -12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%-8d'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%-8d'\n", __LINE__, -12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%8.10d'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%8.10d'\n", __LINE__, -12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%10.8d'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%10.8d'\n", __LINE__, -12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%-10.8d'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%-10.8d'\n", __LINE__, -12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%-010.8d'\n", __LINE__, -12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%.8d'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%d '%.8d'\n", __LINE__, -12345 ) ;
  forma_test_Execute( "[%5u] %%d '%'.8d'\n", __LINE__, 12345 ) ;
  forma_test_Execute( "[%5u] %%d '%?'.8d'\n", __LINE__, 0, 12345 ) ;
#endif

#if 1
  forma_test_ExecuteWithPrintf( "[%5u] %%o '%o'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%o '%2o'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%o '%-2o'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%o '%2.3o'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%o '%08o'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%o '%8o'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%o '%-8o'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%o '%8.10o'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%o '%10.8o'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%o '%-10.8o'\n", __LINE__, 12345 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%o '%.8o'\n", __LINE__, 12345 ) ;
  forma_test_Execute( "[%5u] %%o '%'.8o'\n", __LINE__, 12345 ) ;
  forma_test_Execute( "[%5u] %%o '%?'.8o'\n", __LINE__, 0, 12345 ) ;
#endif

#if 1
  forma_test_ExecuteWithPrintf( "[%5u] %%X '%X'\n", __LINE__, 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%X '%X'\n", __LINE__, 0 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%X '%08X'\n", __LINE__, 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%X '%08X'\n", __LINE__, 0 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%X '%8X'\n", __LINE__, 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%X '%8X'\n", __LINE__, 0 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%X '%-8X'\n", __LINE__, 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%X '%-8X'\n", __LINE__, 0 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%X '%04.4X'\n", __LINE__, 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%X '%04.4X'\n", __LINE__, 0 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%X '%#04.4X'\n", __LINE__, 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%X '%#04.4X'\n", __LINE__, 0 ) ;
  forma_test_Execute( "[%5u] %%X '%'#04.4X'\n", __LINE__, 0 ) ;
  forma_test_Execute( "[%5u] %%X '%?'#04.4X'\n", __LINE__, 0, 0 ) ;

  forma_test_ExecuteWithPrintf( "[%5u] %%x '%x'\n", __LINE__, 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%x '%x'\n", __LINE__, 0 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%x '%08x'\n", __LINE__, 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%x '%08x'\n", __LINE__, 0 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%x '%8x'\n", __LINE__, 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%x '%8x'\n", __LINE__, 0 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%x '%-8x'\n", __LINE__, 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%x '%-8x'\n", __LINE__, 0 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%x '%04.4x'\n", __LINE__, 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%x '%04.4x'\n", __LINE__, 0 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%x '%#04.4x'\n", __LINE__, 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%x '%#04.4x'\n", __LINE__, 0 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%x '%#02.2x'\n", __LINE__, 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%x '%#02.2x'\n", __LINE__, 0 ) ;

  forma_test_ExecuteWithPrintf( "[%5u] %%p '%p'\n", __LINE__, ( void* ) 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%p '%p'\n", __LINE__, ( void* ) 0 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%p '%#04.4p'\n", __LINE__, ( void* ) 0xDEAD ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%p '%#04.4p'\n", __LINE__, ( void* ) 0 ) ;
#endif

#if 1
  uint8_t valueHex[ ] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F } ;
  forma_test_Execute( "[%3u] %%H '%*H'\n", __LINE__, 3, valueHex ) ;
  forma_test_Execute( "[%3u] %%H '%~*H'\n", __LINE__, 3, valueHex ) ;
  forma_test_Execute( "[%3u] %%H '%:*H'\n", __LINE__, 3, valueHex ) ;
  forma_test_Execute( "[%3u] %%H '%~:*H'\n", __LINE__, 3, valueHex ) ;
  forma_test_Execute( "[%3u] %%H '%:*.*H'\n", __LINE__, 16, 8, valueHex ) ;
  forma_test_Execute( "[%3u] %%H '%~:*.*H'\n", __LINE__, 16, 8, valueHex ) ;

  forma_test_Execute( "[%3u] %%D '%~*.*D'\n", __LINE__, 16, 8, valueHex, " " ) ;
#endif

#if 1
  unsigned functionData = 3 ;
  forma_Print( "[%3u] %%F '%F'\n", __LINE__, forma_test_Function, &functionData ) ;
#endif

#if 1
  forma_test_ExecuteWithPrintf( "[%3u] %%c '%c'\n", __LINE__, '#' ) ;
  forma_test_ExecuteWithPrintf( "[%3u] %%c '%10c'\n", __LINE__, '#' ) ;
  forma_test_ExecuteWithPrintf( "[%3u] %%c '%-10c'\n", __LINE__, '#' ) ;
  forma_test_Execute( "[%3u] %%c '%~10c'\n", __LINE__, '#' ) ;
#endif

#if 1
  size_t valueCharArray = ( 'A' << 0 ) | ( 'B' << 8 ) | ( 'C' << 16 ) | ( 'D' << 24 ) ;
  forma_test_Execute( "[%5u] %%C '%4C'\n", __LINE__, &valueCharArray ) ;
  forma_test_Execute( "[%5u] %%C '%*C'\n", __LINE__, 2, &valueCharArray ) ;
  forma_test_Execute( "[%5u] %%C '%~4C'\n", __LINE__, &valueCharArray ) ;

  forma_test_Execute( "[%5u] %%C '%4.6C'\n", __LINE__, &valueCharArray ) ;
  forma_test_Execute( "[%5u] %%C '%~4.6C'\n", __LINE__, &valueCharArray ) ;
  forma_test_Execute( "[%5u] %%C '%-4.6C'\n", __LINE__, &valueCharArray ) ;
  forma_test_Execute( "[%5u] %%C '%-~4.6C'\n", __LINE__, &valueCharArray ) ;

  forma_test_Execute( "[%5u] %%C '%#4.6zC'\n", __LINE__, &valueCharArray ) ;
  forma_test_Execute( "[%5u] %%C '%#~4.6zC'\n", __LINE__, &valueCharArray ) ;
  forma_test_Execute( "[%5u] %%C '%#-4.6zC'\n", __LINE__, &valueCharArray ) ;
  forma_test_Execute( "[%5u] %%C '%#-~4.6zC'\n", __LINE__, &valueCharArray ) ;

  forma_test_Execute( "[%5u] %%C '%'4.6C'\n", __LINE__, &valueCharArray ) ;
  forma_test_Execute( "[%5u] %%C '%'~4.6C'\n", __LINE__, &valueCharArray ) ;
  forma_test_Execute( "[%5u] %%C '%'-4.6C'\n", __LINE__, &valueCharArray ) ;
  forma_test_Execute( "[%5u] %%C '%'-~4.6C'\n", __LINE__, &valueCharArray ) ;

  forma_test_Execute( "[%5u] %%C '%'-~04.6C'\n", __LINE__, &valueCharArray ) ;
  forma_test_Execute( "[%5u] %%C '%'~04.6C'\n", __LINE__, &valueCharArray ) ;
  forma_test_Execute( "[%5u] %%C '%=_'~4.6C'\n", __LINE__, &valueCharArray ) ;

  forma_test_Execute( "[%5u] %%C '%|=_'~4.6C'\n", __LINE__, forma_test_MapChars, 0, &valueCharArray ) ;
#endif

#if 1
  uint32_t ip = 0x6800a8c0 ;
  forma_test_Execute( "[%5u] %%I '%I'\n", __LINE__, ip ) ;
  forma_test_Execute( "[%5u] %%I '%~I'\n", __LINE__, ip ) ;
  forma_test_Execute( "[%5u] %%I '%?~I'\n", __LINE__, 0, ip ) ;
  forma_test_Execute( "[%5u] %%I '%'~I'\n", __LINE__, 0 ) ;
  forma_test_Execute( "[%5u] %%I '%.3I'\n", __LINE__, ip ) ;
  forma_test_Execute( "[%5u] %%I '%'~.3I'\n", __LINE__, 0 ) ;
  forma_test_Execute( "[%5u] %%I '%'~.3I'\n", __LINE__, 1 ) ;
#endif

#if 1
  const char* valueBase64 = "Man is distinguished, not only by his reason, but by this singular passion from other animals, "
      "which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable "
      "generation of knowledge, exceeds the short vehemence of any carnal pleasure." ;
  forma_test_Execute( "[%5u] %%B '%*B'\n", __LINE__, strlen( valueBase64 ), valueBase64 ) ;
  forma_test_Execute( "[%5u] %%B '%'*B'\n", __LINE__, strlen( valueBase64 ), valueBase64 ) ;
  forma_test_Execute( "[%5u] %%B '%~*B'\n", __LINE__, 2, strlen( valueBase64 ), valueBase64, strlen( valueBase64 ), valueBase64 ) ;
  forma_test_Execute( "[%5u] %%B '%?*B' %d\n", __LINE__, 0, strlen( valueBase64 ), valueBase64, 1 ) ;
  forma_test_Execute( "[%5u] %%B '%?~*B' %d\n", __LINE__, 0, 2, strlen( valueBase64 ), valueBase64, strlen( valueBase64 ), valueBase64, 1 ) ;

  forma_test_Execute( "[%5u] %%P '%*P' %d\n", __LINE__, strlen( valueBase64 ), "ADDON", valueBase64, 1 ) ;
  forma_test_Execute( "[%5u] %%P '%*P' %d\n", __LINE__, 1, "ADDON", valueBase64, 1 ) ;
#endif

#if 1
  forma_test_ExecuteWithPrintf( "[%5u] %%s '%s' %d\n", __LINE__, "string", 1 ) ;
  forma_test_Execute( "[%5u] %%s '%?s' %d\n", __LINE__, 0, "string", 1 ) ;
  forma_test_Execute( "[%5u] %%s '%'s' %d\n", __LINE__, "string", 1 ) ;
  forma_test_Execute( "[%5u] %%s '%|s' %d\n", __LINE__, forma_test_MapChars, 0, "string2", 1 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%s '%0.0s' %d\n", __LINE__, "string", 1 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%s '%1s' %d\n", __LINE__, "string", 1 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%s '%10s' %d\n", __LINE__, "string", 1 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%s '%-10s' %d\n", __LINE__, "string", 1 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%s '%1.2s' %d\n", __LINE__, "string", 1 ) ; // TODO
  forma_test_ExecuteWithPrintf( "[%5u] %%s '%.2s' %d\n", __LINE__, "string", 1 ) ; // TODO
  forma_test_ExecuteWithPrintf( "[%5u] %%s '%.10s' %d\n", __LINE__, "string", 1 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%s '%10.10s' %d\n", __LINE__, "string", 1 ) ;
  forma_test_ExecuteWithPrintf( "[%5u] %%s '%-10.10s' %d\n", __LINE__, "string", 1 ) ;

  forma_test_Execute( "[%5u] %%s '%_+???.???_s'\n", __LINE__, "abcd" ) ;
  forma_test_Execute( "[%5u] %%s '%|_+???.???_s'\n", __LINE__, forma_test_MapChars, 0, "abcd" ) ;
  forma_test_Execute( "[%5u] %%s '%|-_+???.???_s'\n", __LINE__, forma_test_MapChars, 0, "abcd" ) ;

  forma_test_Execute( "[%5u] %%J '%J' %d\n", __LINE__, "abc\tstring\x19", 1 ) ;

  forma_test_Execute( "[%5u] %%E 'INSERT INTO Test VALUES ( %E )'", __LINE__, "test" ) ;
  forma_test_Execute( "[%5u] %%E 'INSERT INTO Test VALUES ( %'E )'", __LINE__, "t\'est" ) ;
  forma_test_Execute( "[%5u] %%E 'INSERT INTO Test VALUES ( %\"E )'", __LINE__, "t\"est" ) ;
#endif

#if 1
  time_t valueTimestamp = time( 0 ) ;
  struct tm* valueTmA = gmtime( &valueTimestamp ) ;
  struct tm valueTm[ 2 ] ;
  valueTm[ 0 ].tm_mday = 1 ;
  valueTm[ 0 ].tm_mon = 1 ;
  valueTm[ 0 ].tm_year = 2025 - 1900 ;
  valueTm[ 0 ].tm_min = 1 ;
  valueTm[ 0 ].tm_hour = 1 ;
  valueTm[ 0 ].tm_sec = 1 ;
  valueTm[ 1 ].tm_mday = 11 ;
  valueTm[ 1 ].tm_mon = 11 ;
  valueTm[ 1 ].tm_year = 2025 - 1900 ;
  valueTm[ 1 ].tm_min = 11 ;
  valueTm[ 1 ].tm_hour = 11 ;
  valueTm[ 1 ].tm_sec = 11 ;
  forma_test_Execute( "[%5u] %%T '%?_d.M.y H:m:s_T'\n", __LINE__, 0, valueTmA ) ;
  forma_test_Execute( "[%5u] %%T '%_d.M.y H:m:s_T'\n", __LINE__, valueTmA ) ;
  forma_test_Execute( "[%5u] %%T '%_d.M.y H:m:s_T'\n", __LINE__, &valueTm[ 0 ] ) ;
  forma_test_Execute( "[%5u] %%T '%_d.M.y H:m:s_T'\n", __LINE__, &valueTm[ 1 ] ) ;
  forma_test_Execute( "[%5u] %%T '%_dd.MM.yyyy HH:mm:ss_T'\n", __LINE__, &valueTm[ 0 ] ) ;
  forma_test_Execute( "[%5u] %%T '%_dd.MM.yyyy HH:mm:ss_T'\n", __LINE__, &valueTm[ 1 ] ) ;
  forma_test_Execute( "[%5u] %%T '%=0_dd.MM.yyyy HH:mm:ss_T'\n", __LINE__, &valueTm[ 0 ] ) ;
  forma_test_Execute( "[%5u] %%T '%=0_dd.MM.yyyy HH:mm:ss_T'\n", __LINE__, &valueTm[ 1 ] ) ;
  forma_test_Execute( "[%5u] %%T '%=0_dd.MM.yyyy hh:mm:ss_T'\n", __LINE__, &valueTm[ 0 ] ) ;
  forma_test_Execute( "[%5u] %%T '%=0_dd.MM.yyyy hh:mm:ss_T'\n", __LINE__, &valueTm[ 1 ] ) ;
  for ( valueTm[ 1 ].tm_hour = 0 ; valueTm[ 1 ].tm_hour < 24 ; valueTm[ 1 ].tm_hour++ )
  {
    forma_test_Execute( "[%5u] %%T %d >> '%=0_dd.MM.yyyy hh:mm:ss_T'\n", __LINE__, valueTm[ 1 ].tm_hour, &valueTm[ 1 ] ) ;
  }
#endif

#if 1
  forma_test_ExecuteBuffer( __LINE__, 0, "%s", "0123456789" ) ;
  forma_test_ExecuteBuffer( __LINE__, 1, "%s", "0123456789" ) ;
  forma_test_ExecuteBuffer( __LINE__, 2, "%s", "0123456789" ) ;
  forma_test_ExecuteBuffer( __LINE__, 10, "%s", "0123456789" ) ;
  forma_test_ExecuteBuffer( __LINE__, 20, "%s", "0123456789" ) ;
#endif

  return 0 ;
}
