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

#include <unistd.h>
#ifndef __dropOS__
#include <errno.h>
#else
#include "hapa/include/hapa.h"
#endif
#include "forma_writer.h"

int forma_writer_BufferChar ( forma_context_t* context, char c )
{
  forma_writer_buffer_t* contextTyped = ( forma_writer_buffer_t* ) context ;

  /* check if end of buffer was reached */
  if ( 0 == contextTyped->bufferSize || 0 == --contextTyped->bufferSize )
  {
    *contextTyped->buffer = 0 ;
    return -ENOMEM ;
  }

  *contextTyped->buffer++ = c ;
  if ( 0 != c )
  {
    contextTyped->bytes++ ;
  }

  return 0 ;
}

void forma_writer_PrintFlush ( forma_writer_print_t* context )
{
  if ( FormaWriterConsoleType_Stdio == context->type )
  {
#if FORMA_CONSOLE_USE_WRITE == 1
    ( void ) write( 1, context->buffer, context->bytes ) ;
#else
    ( void ) fwrite( context->buffer, context->bytes, 1, stdout ) ;
#endif
  }
  else
  {
    char* bufferPtr = context->buffer ;
    unsigned bufferBytes = context->bytes ;
    int bytes ;
    while ( bufferBytes > 0 && ( bytes = write( 1, bufferPtr, bufferBytes ) ) >= 0 )
    {
      bufferPtr += bytes ;
      bufferBytes -= ( unsigned ) bytes ;
    }
  }

  context->bufferPtr = context->buffer ;
  context->bufferSize = sizeof ( context->buffer ) ;
  context->bytes = 0 ;
}

int forma_writer_PrintChar ( forma_context_t* context, char c )
{
  forma_writer_print_t* contextTyped = ( forma_writer_print_t* ) context ;

  /* sanity check */
  if ( 0 == c )
  {
    return 0 ;
  }

  /* store character in buffer */
  *contextTyped->bufferPtr++ = c ;
  contextTyped->bytes++ ;

  /* check if end of buffer was reached */
  if ( 0 == --contextTyped->bufferSize )
  {
    forma_writer_PrintFlush( contextTyped ) ;
  }

  return 0 ;
}

void forma_writer_FileFlush ( forma_writer_file_t* context )
{
  ( void ) fwrite( context->buffer, context->bytes, 1, context->file ) ;

  context->bufferPtr = context->buffer ;
  context->bufferSize = sizeof ( context->buffer ) ;
  context->bytes = 0 ;
}

int forma_writer_FileChar ( forma_context_t* context, char c )
{
  forma_writer_file_t* contextTyped = ( forma_writer_file_t* ) context ;

  /* sanity check */
  if ( 0 == c )
  {
    return 0 ;
  }

  /* store character in buffer */
  *contextTyped->bufferPtr++ = c ;
  contextTyped->bytes++ ;

  /* check if end of buffer was reached */
  if ( 0 == --contextTyped->bufferSize )
  {
    forma_writer_FileFlush( contextTyped ) ;
  }

  return 0 ;
}

void forma_writer_FileDescriptorFlush ( forma_writer_file_descriptor_t* context )
{
  ( void ) write( context->fileDescriptor, context->buffer, context->bytes ) ;

  context->bufferPtr = context->buffer ;
  context->bufferSize = sizeof ( context->buffer ) ;
  context->bytes = 0 ;
}

int forma_writer_FileDescriptorChar ( forma_context_t* context, char c )
{
  forma_writer_file_descriptor_t* contextTyped = ( forma_writer_file_descriptor_t* ) context ;

  /* sanity check */
  if ( 0 == c )
  {
    return 0 ;
  }

  /* store character in buffer */
  *contextTyped->bufferPtr++ = c ;
  contextTyped->bytes++ ;

  /* check if end of buffer was reached */
  if ( 0 == --contextTyped->bufferSize )
  {
    forma_writer_FileDescriptorFlush( contextTyped ) ;
  }

  return 0 ;
}

int forma_writer_OutputChar ( forma_context_t* context, char c )
{
  int result ;
  forma_writer_output_t* contextTyped = ( forma_writer_output_t* ) context ;

  /* check if end of buffer was reached */
  result = contextTyped->outputChar( contextTyped->outputCharContext, c ) ;
  if ( 0 != result )
  {
    return result ;
  }

  return 0 ;
}

int forma_writer_CountChar ( forma_context_t* context, char c )
{
  forma_writer_count_t* contextTyped = ( forma_writer_count_t* ) context ;

  /* count character */
  if ( 0 != c )
  {
    contextTyped->bytes++ ;
  }

  return 0 ;
}
