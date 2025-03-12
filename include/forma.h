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

#ifndef __FORMA_H__
#define __FORMA_H__

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#ifdef __cplusplus
#include "forma_Buffer.hpp"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct forma_object_st forma_object_t ;

  typedef struct forma_context_st forma_context_t ;

  typedef int forma_WriteChar_t ( forma_context_t* context, char c ) ;

  typedef void forma_Function_t ( forma_object_t* object, void* userData ) ;

  typedef int forma_OutputChar_t ( void* context, char c ) ;

  typedef char forma_MapChar_t ( void* context, bool filler, char c ) ;

  struct forma_object_st
  {
      forma_context_t* context ;

      forma_WriteChar_t* writeChar ;
  } ;

  void forma_Object ( forma_object_t* object, const char* fmt, ... ) ;

  void forma_ObjectArgList ( forma_object_t* object, const char* fmt, va_list ap ) ;

  unsigned forma_Print ( const char* fmt, ... ) ;

  unsigned forma_PrintArgList ( const char* fmt, va_list ap ) ;

  unsigned forma_File ( FILE* file, const char* fmt, ... ) ;

  unsigned forma_FileArgList ( FILE* file, const char* fmt, va_list ap ) ;

  unsigned forma_FileDescriptor ( int fileDescriptor, const char* fmt, ... ) ;

  unsigned forma_FileDescriptorArgList ( int fileDescriptor, const char* fmt, va_list ap ) ;

  unsigned forma_Write ( const char* fmt, ... ) ;

  unsigned forma_WriteArgList ( const char* fmt, va_list ap ) ;

  unsigned forma_Buffer ( char* buffer, size_t bufferSize, const char* fmt, ... ) ;

  unsigned forma_BufferArgList ( char* buffer, size_t bufferSize, const char* fmt, va_list ap ) ;

  unsigned forma_Output ( forma_OutputChar_t* outputChar, void* outputCharContext, const char* fmt, ... ) ;

  unsigned forma_OutputArgList ( forma_OutputChar_t* outputChar, void* outputCharContext, const char* fmt, va_list ap ) ;

  unsigned forma_Count ( const char* fmt, ... ) ;

  unsigned forma_CountArgList ( const char* fmt, va_list ap ) ;

#ifdef __cplusplus
}
#endif

#endif /* __FORMA_H__ */
