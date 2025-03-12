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
#include <unistd.h>
#include <string.h>
#ifndef __dropOS__
#include <errno.h>
#else
#include "hapa/include/hapa.h"
#endif
#include "forma_private.h"
#include "forma_writer.h"
#include "forma_conv.h"

#ifdef __linux__
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

#if !defined va_arg
#define va_arg( ... ) 0
#endif

#ifndef FORMA_FALLTHROUGH
#if __GNUC__ >= 7
#define FORMA_FALLTHROUGH()     __attribute__ ((fallthrough))
#else
#define FORMA_FALLTHROUGH()
#endif
#endif

int forma_WriteString ( forma_context_t* context, forma_WriteChar_t* writeChar, const char* str )
{
  int result ;

  while ( 1 )
  {
    char c = *str++ ;
    if ( 0 == c )
    {
      break ;
    }
    result = writeChar( context, c ) ;
    if ( 0 != result )
    {
      return result ;
    }
  }

  return 0 ;
}

unsigned forma_Buffer ( char* buffer, size_t bufferSize, const char* fmt, ... )
{
  unsigned bytes ;

  va_list ap ;
  va_start( ap, fmt ) ;
  bytes = forma_BufferArgList( buffer, bufferSize, fmt, ap ) ;
  va_end( ap ) ;

  return bytes ;
}

unsigned forma_BufferArgList ( char* buffer, size_t bufferSize, const char* fmt, va_list ap )
{
  if ( 0 == buffer )
  {
    return 0 ;
  }
  if ( 0 == bufferSize )
  {
    return 0 ;
  }

  forma_writer_buffer_t context ;
#ifndef __linux__
  context.context.ap = ap ;
#endif
  context.buffer = buffer ;
  context.bufferSize = ( unsigned ) bufferSize ;
  context.bytes = 0 ;

#ifdef __linux__
  va_copy( context.context.ap, ap ) ;
#endif

  while ( 1 )
  {
    char c = *fmt++ ;
    switch ( c )
    {
      case 0:
      {
        *context.buffer = 0 ;

#ifdef __linux__
        va_end( context.context.ap ) ;
#endif

        return context.bytes ;
      }

      case '%':
      {
        fmt = forma_conv_Process( &context.context, forma_writer_BufferChar, fmt ) ;
        if ( 0 == fmt )
        {
          if ( context.bufferSize > 0 )
          {
            *context.buffer = 0 ;
          }

#ifdef __linux__
          va_end( context.context.ap ) ;
#endif

          return context.bytes ;
        }

        break ;
      }

      default:
      {
        /* check if end of buffer was reached */
        if ( 0 == --context.bufferSize )
        {
          *context.buffer = 0 ;

#ifdef __linux__
          va_end( context.context.ap ) ;
#endif

          return context.bytes ;
        }

        *context.buffer++ = c ;
        context.bytes++ ;

        break ;
      }
    }
  }
}

static unsigned forma_Console ( forma_writer_console_type_t type, const char* fmt, va_list ap )
{
  forma_writer_print_t context ;
#ifndef __linux__
  context.context.ap = ap ;
#endif
  context.type = type ;
  context.bufferPtr = context.buffer ;
  context.bufferSize = FORMA_PRINT_BUFFER_SIZE ;
  context.bytes = 0 ;

#ifdef __linux__
  va_copy( context.context.ap, ap ) ;
#endif

  while ( 1 )
  {
    char c = *fmt++ ;
    switch ( c )
    {
      case 0:
      {
        forma_writer_PrintFlush( &context ) ;

#ifdef __linux__
        va_end( context.context.ap ) ;
#endif

        return context.bytes ;
      }

      case '%':
      {
        fmt = forma_conv_Process( &context.context, forma_writer_PrintChar, fmt ) ;
        if ( 0 == fmt )
        {
          forma_writer_PrintFlush( &context ) ;

#ifdef __linux__
          va_end( context.context.ap ) ;
#endif

          return context.bytes ;
        }

        break ;
      }

      default:
      {
        forma_writer_PrintChar( &context.context, c ) ;
        break ;
      }
    }
  }
}

unsigned forma_Print ( const char* fmt, ... )
{
  unsigned bytes ;

  va_list ap ;
  va_start( ap, fmt ) ;
  bytes = forma_PrintArgList( fmt, ap ) ;
  va_end( ap ) ;

  return bytes ;
}

unsigned forma_PrintArgList ( const char* fmt, va_list ap )
{
  unsigned bytes ;
  bytes = forma_Console( FormaWriterConsoleType_Stdio, fmt, ap ) ;

#ifdef FORMA_PRINT_AUTO_FLUSH
  fflush( stdout ) ;
#endif

  return bytes ;
}

unsigned forma_File ( FILE* file, const char* fmt, ... )
{
  unsigned bytes ;

  va_list ap ;
  va_start( ap, fmt ) ;
  bytes = forma_FileArgList( file, fmt, ap ) ;
  va_end( ap ) ;

  return bytes ;
}

unsigned forma_FileArgList ( FILE* file, const char* fmt, va_list ap )
{
  forma_writer_file_t context ;
#ifndef __linux__
  context.context.ap = ap ;
#endif
  context.bufferPtr = context.buffer ;
  context.bufferSize = FORMA_PRINT_BUFFER_SIZE ;
  context.bytes = 0 ;
  context.file = file ;

#ifdef __linux__
  va_copy( context.context.ap, ap ) ;
#endif

  while ( 1 )
  {
    char c = *fmt++ ;
    switch ( c )
    {
      case 0:
      {
        forma_writer_FileFlush( &context ) ;

#ifdef __linux__
        va_end( context.context.ap ) ;
#endif

        return context.bytes ;
      }

      case '%':
      {
        fmt = forma_conv_Process( &context.context, forma_writer_FileChar, fmt ) ;
        if ( 0 == fmt )
        {
          forma_writer_FileFlush( &context ) ;

#ifdef __linux__
          va_end( context.context.ap ) ;
#endif

          return context.bytes ;
        }

        break ;
      }

      default:
      {
        forma_writer_FileChar( &context.context, c ) ;
        break ;
      }
    }
  }
}

unsigned forma_Write ( const char* fmt, ... )
{
  unsigned bytes ;

  va_list ap ;
  va_start( ap, fmt ) ;
  bytes = forma_WriteArgList( fmt, ap ) ;
  va_end( ap ) ;

  return bytes ;
}

unsigned forma_WriteArgList ( const char* fmt, va_list ap )
{
  return forma_Console( FormaWriterConsoleType_Write, fmt, ap ) ;
}

void forma_Object ( forma_object_t* object, const char* fmt, ... )
{
  va_list ap ;
  va_start( ap, fmt ) ;
  forma_ObjectArgList( object, fmt, ap ) ;
  va_end( ap ) ;
}

void forma_ObjectArgList ( forma_object_t* object, const char* fmt, va_list ap )
{
  int result ;
  forma_context_t* context = object->context ;
  forma_WriteChar_t* writeChar = object->writeChar ;

#ifdef __linux__
  va_list apParent ;
  ( void ) memcpy( &apParent, &context->ap, sizeof ( apParent ) ) ;
  va_copy( context->ap, ap ) ;
#else
  va_list apParent = context->ap ;
  context->ap = ap ;
#endif

  while ( 1 )
  {
    char c = *fmt++ ;
    switch ( c )
    {
      case 0:
      {
#ifdef __linux__
        va_end( context->ap ) ;
        ( void ) memcpy( &context->ap, &apParent, sizeof ( apParent ) ) ;
#else
        context->ap = apParent ;
#endif
        return ;
      }

      case '%':
      {
        fmt = forma_conv_Process( context, writeChar, fmt ) ;
        if ( 0 == fmt )
        {
#ifdef __linux__
          va_end( context->ap ) ;
          ( void ) memcpy( &context->ap, &apParent, sizeof ( apParent ) ) ;
#else
          context->ap = apParent ;
#endif
          return ;
        }

        break ;
      }

      default:
      {
        result = writeChar( context, c ) ;
        if ( 0 != result )
        {
#ifdef __linux__
          va_end( context->ap ) ;
          ( void ) memcpy( &context->ap, &apParent, sizeof ( apParent ) ) ;
#else
          context->ap = apParent ;
#endif
          return ;
        }

        break ;
      }
    }
  }
}

unsigned forma_Output ( forma_OutputChar_t* outputChar, void* outputCharContext, const char* fmt, ... )
{
  unsigned bytes ;

  va_list ap ;
  va_start( ap, fmt ) ;
  bytes = forma_OutputArgList( outputChar, outputCharContext, fmt, ap ) ;
  va_end( ap ) ;

  return bytes ;
}

unsigned forma_OutputArgList ( forma_OutputChar_t* outputChar, void* outputCharContext, const char* fmt, va_list ap )
{
  forma_writer_output_t context ;
#ifndef __linux__
  context.context.ap = ap ;
#endif
  context.bytes = 0 ;
  context.outputChar = outputChar ;
  context.outputCharContext = outputCharContext ;

#ifdef __linux__
  va_copy( context.context.ap, ap ) ;
#endif

  while ( 1 )
  {
    char c = *fmt++ ;
    switch ( c )
    {
      case 0:
      {
#ifdef __linux__
        va_end( context.context.ap ) ;
#endif

        return context.bytes ;
      }

      case '%':
      {
        fmt = forma_conv_Process( &context.context, forma_writer_OutputChar, fmt ) ;
        if ( 0 == fmt )
        {
#ifdef __linux__
          va_end( context.context.ap ) ;
#endif

          return context.bytes ;
        }

        break ;
      }

      default:
      {
        forma_writer_OutputChar( &context.context, c ) ;
        break ;
      }
    }
  }
}

unsigned forma_FileDescriptor ( int fileDescriptor, const char* fmt, ... )
{
  unsigned bytes ;

  va_list ap ;
  va_start( ap, fmt ) ;
  bytes = forma_FileDescriptorArgList( fileDescriptor, fmt, ap ) ;
  va_end( ap ) ;

  return bytes ;
}

unsigned forma_FileDescriptorArgList ( int fileDescriptor, const char* fmt, va_list ap )
{
  forma_writer_file_descriptor_t context ;
#ifndef __linux__
  context.context.ap = ap ;
#endif
  context.bufferPtr = context.buffer ;
  context.bufferSize = FORMA_PRINT_BUFFER_SIZE ;
  context.bytes = 0 ;
  context.bytes = fileDescriptor ;

#ifdef __linux__
  va_copy( context.context.ap, ap ) ;
#endif

  while ( 1 )
  {
    char c = *fmt++ ;
    switch ( c )
    {
      case 0:
      {
        forma_writer_FileDescriptorFlush( &context ) ;

#ifdef __linux__
        va_end( context.context.ap ) ;
#endif

        return context.bytes ;
      }

      case '%':
      {
        fmt = forma_conv_Process( &context.context, forma_writer_FileDescriptorChar, fmt ) ;
        if ( 0 == fmt )
        {
          forma_writer_FileDescriptorFlush( &context ) ;

#ifdef __linux__
          va_end( context.context.ap ) ;
#endif

          return context.bytes ;
        }

        break ;
      }

      default:
      {
        forma_writer_FileDescriptorChar( &context.context, c ) ;
        break ;
      }
    }
  }
}

unsigned forma_Count ( const char* fmt, ... )
{
  unsigned bytes ;

  va_list ap ;
  va_start( ap, fmt ) ;
  bytes = forma_CountArgList( fmt, ap ) ;
  va_end( ap ) ;

  return bytes ;
}

unsigned forma_CountArgList ( const char* fmt, va_list ap )
{
  forma_writer_count_t context ;
#ifndef __linux__
  context.context.ap = ap ;
#endif
  context.bytes = 0 ;

#ifdef __linux__
  va_copy( context.context.ap, ap ) ;
#endif

  while ( 1 )
  {
    char c = *fmt++ ;
    switch ( c )
    {
      case 0:
      {
#ifdef __linux__
        va_end( context.context.ap ) ;
#endif

        return context.bytes ;
      }

      case '%':
      {
        fmt = forma_conv_Process( &context.context, forma_writer_CountChar, fmt ) ;
        if ( 0 == fmt )
        {
#ifdef __linux__
          va_end( context.context.ap ) ;
#endif

          return context.bytes ;
        }

        break ;
      }

      default:
      {
        forma_writer_CountChar( &context.context, c ) ;
        break ;
      }
    }
  }
}
