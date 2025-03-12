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

#ifndef __FORMA_WRITER_H__
#define __FORMA_WRITER_H__

#include "forma_config.h"
#include <stdio.h>
#include <stdint.h>
#include "forma_private.h"

#ifdef __cplusplus
extern "C"
{
#endif

  enum forma_writer_console_type_e
  {
    FormaWriterConsoleType_Write, /**/
    FormaWriterConsoleType_Stdio, /**/
  } ;

  typedef enum forma_writer_console_type_e forma_writer_console_type_t ;

  typedef struct forma_writer_buffer_st forma_writer_buffer_t ;

  typedef struct forma_writer_print_st forma_writer_print_t ;

  typedef struct forma_writer_file_st forma_writer_file_t ;

  typedef struct forma_writer_file_descriptor_st forma_writer_file_descriptor_t ;

  typedef struct forma_writer_output_st forma_writer_output_t ;

  typedef struct forma_writer_count_st forma_writer_count_t ;

  struct forma_writer_buffer_st
  {
      forma_context_t context ;

      char* buffer ;

      unsigned bufferSize ;

      unsigned bytes ;
  } ;

  struct forma_writer_print_st
  {
      forma_context_t context ;

      forma_writer_console_type_t type ;

      char buffer[ FORMA_PRINT_BUFFER_SIZE ] ;

      char* bufferPtr ;

      unsigned bufferSize ;

      unsigned bytes ;
  } ;

  struct forma_writer_file_st
  {
      forma_context_t context ;

      char buffer[ FORMA_PRINT_BUFFER_SIZE ] ;

      char* bufferPtr ;

      unsigned bufferSize ;

      unsigned bytes ;

      FILE* file ;
  } ;

  struct forma_writer_file_descriptor_st
  {
      forma_context_t context ;

      char buffer[ FORMA_PRINT_BUFFER_SIZE ] ;

      char* bufferPtr ;

      unsigned bufferSize ;

      unsigned bytes ;

      int fileDescriptor ;
  } ;

  struct forma_writer_output_st
  {
      forma_context_t context ;

      unsigned bytes ;

      forma_OutputChar_t* outputChar ;

      void* outputCharContext ;
  } ;

  struct forma_writer_count_st
  {
      forma_context_t context ;

      unsigned bytes ;
  } ;

  int forma_writer_BufferChar ( forma_context_t* context, char c ) ;

  void forma_writer_PrintFlush ( forma_writer_print_t* context ) ;

  int forma_writer_PrintChar ( forma_context_t* context, char c ) ;

  void forma_writer_FileFlush ( forma_writer_file_t* context ) ;

  int forma_writer_FileChar ( forma_context_t* context, char c ) ;

  void forma_writer_FileDescriptorFlush ( forma_writer_file_descriptor_t* context ) ;

  int forma_writer_FileDescriptorChar ( forma_context_t* context, char c ) ;

  int forma_writer_OutputChar ( forma_context_t* context, char c ) ;

  int forma_writer_CountChar ( forma_context_t* context, char c ) ;

#ifdef __cplusplus
}
#endif

#endif /* __FORMA_WRITER_H__ */
