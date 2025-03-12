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

#ifndef __DROPAREA__forma__Buffer_hpp__
#define __DROPAREA__forma__Buffer_hpp__

#include <stddef.h>

#ifdef __cplusplus

namespace droparea
{

  namespace forma
  {

    class Buffer
    {

      public:

        template< unsigned SIZE > class Allocated ;

        Buffer ( char* buffer, size_t bufferSize ) throw () ;

        void reset ( void ) ;

        void format ( const char* fmt, ... ) ;

        inline char* getBufferPosition ( size_t& available )
        {
          available = mBufferAvailable ;
          return mBufferPtr ;
        }

        inline char* toString ( void )
        {
          return mBuffer ;
        }

        inline size_t writtenBytes ( void )
        {
          return ( size_t ) ( mBufferSize - mBufferAvailable ) ;
        }

      protected:

        char* mBuffer ;

        char* mBufferPtr ;

        size_t mBufferSize ;

        size_t mBufferAvailable ;

      private:

        Buffer ( const Buffer& instance ) ;

    } ;

    template< unsigned SIZE > class Buffer::Allocated : public Buffer
    {

      public:

        Allocated ( void ) : //
            Buffer( mBuffer, SIZE ) /**/

        {
        }

      protected:

        char mBuffer[ SIZE ] ;

    } ;

  }

}

#endif

#endif /* __DROPAREA__forma__Buffer_hpp__ */
