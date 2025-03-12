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

#ifndef __FORMA_CONV_H__
#define __FORMA_CONV_H__

#include "forma_config.h"
#include <stdint.h>
#include "forma_private.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct forma_conv_data_st forma_conv_data_t ;

  typedef struct forma_conv_state_definition_st forma_conv_state_definition_t ;

  typedef const forma_conv_state_definition_t* forma_conv_StateRun_t ( forma_conv_data_t* data ) ;

  struct forma_conv_state_definition_st
  {
      forma_conv_StateRun_t* run ;
  } ;

  struct forma_conv_data_st
  {
      forma_context_t* context ;

      forma_WriteChar_t* writeChar ;

      const char* fmt ;

#if FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED == 1
      unsigned length ;
#endif

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1

      ssize_t width ;

      ssize_t precision ;

#endif

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1
      int leftOriented ;
#endif

#if FORMA_SUPPORT_FLAG_TILDE_ENABLED == 1
      int repeatOrReverseOrOffsetOrMultiParts ;
#endif

#if FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED == 1

      forma_MapChar_t* mapChar ;

      void* mapCharContext ;

#endif

#if FORMA_SUPPORT_FLAG_PATTERN_ENABLED == 1

      const char* pattern ;

      size_t patternLength ;

      size_t patternPlaceholders ;

#endif

#if FORMA_SUPPORT_CONV_INT_ENABLED == 1
      int divider ;
#endif

#if FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED == 1 || FORMA_SUPPORT_CONV_TIME_ENABLED == 1
      char filler ;
#endif

#if FORMA_SUPPORT_CONV_STR_ENABLED == 1 && FORMA_SUPPORT_CONV_STR_SQL_ENABLED == 1

      int expressionValue ;

      char quote ;

#endif

#if FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED == 1
      char artificialQuote ;
#endif

#if FORMA_SUPPORT_CONV_HEX_ENABLED == 1

      unsigned featureColon ;

      char hex10 ;

      const char* forcedPrefix ;

#endif
  } ;

  extern const forma_conv_state_definition_t forma_conv_STATE_FINISHED ;

  const char* forma_conv_Process ( forma_context_t* context, forma_WriteChar_t* writeChar, const char* fmt ) ;

#ifdef __cplusplus
}
#endif

#endif /* __FORMA_CONV_H__ */
