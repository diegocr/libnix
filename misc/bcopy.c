#include <stdlib.h>

/* This is a _fast_ block move routine! */

#if 0 // libnix < 3.x
void bcopy(const void *s1,void *s2,size_t n)
{ size_t m;
  if(!n)
    return;
  if(s2<s1)
  { if(n>15)
    { if((long)s1&1)
      { *((char *)s2)++=*((char *)s1)++;
        n--; }
      if(!((long)s2&1))
      { if((long)s1&2)
        { *((short *)s2)++=*((short *)s1)++;
          n-=2; }
        for(m=n/(8*sizeof(long));m;--m)
        { *((long *)s2)++=*((long *)s1)++; *((long *)s2)++=*((long *)s1)++;
          *((long *)s2)++=*((long *)s1)++; *((long *)s2)++=*((long *)s1)++;
          *((long *)s2)++=*((long *)s1)++; *((long *)s2)++=*((long *)s1)++;
          *((long *)s2)++=*((long *)s1)++; *((long *)s2)++=*((long *)s1)++; }
        n&=8*sizeof(long)-1;
        for(m=n/sizeof(long);m;--m)
          *((long *)s2)++=*((long *)s1)++;
        n&=sizeof(long)-1; }
      if(!n) return;
    } do;while(*((char *)s2)++=*((char *)s1)++,--n);
  }else
  { (char *)s1+=n;
    (char *)s2+=n;
    if(n>15)
    { if((long)s1&1)
      { *--((char *)s2)=*--((char *)s1);
        n--; }
      if(!((long)s2&1))
      { if((long)s1&2)
        { *--((short *)s2)=*--((short *)s1);
          n-=2; }
        for(m=n/(8*sizeof(long));m;--m)
        { *--((long *)s2)=*--((long *)s1); *--((long *)s2)=*--((long *)s1);
          *--((long *)s2)=*--((long *)s1); *--((long *)s2)=*--((long *)s1);
          *--((long *)s2)=*--((long *)s1); *--((long *)s2)=*--((long *)s1);
          *--((long *)s2)=*--((long *)s1); *--((long *)s2)=*--((long *)s1); }
        n&=8*sizeof(long)-1;
        for(m=n/sizeof(long);m;--m)
          *--((long *)s2)=*--((long *)s1);
        n&=sizeof(long)-1; }
      if(!n) return;
    } do;while(*--((char *)s2)=*--((char *)s1),--n);
  }
}
#elif 0
void bcopy(const void *s1,void *s2,size_t n)
{ size_t m,z=n;
  if(!n)
    return;
  if(s2<s1)
  { if(n>15)
    { if((long)s1&1)
      { *((char *)s2)++=*((char *)s1)++;
        n--; }
      if(!((long)s2&1))
      { if((long)s1&2)
        { *((short *)s2)++=*((short *)s1)++;
          n-=2; }
        for(m=n/(8*sizeof(long));m;--m)
        { *((long *)s2)++=*((long *)s1)++; *((long *)s2)++=*((long *)s1)++;
          *((long *)s2)++=*((long *)s1)++; *((long *)s2)++=*((long *)s1)++;
          *((long *)s2)++=*((long *)s1)++; *((long *)s2)++=*((long *)s1)++;
          *((long *)s2)++=*((long *)s1)++; *((long *)s2)++=*((long *)s1)++; }
        n&=8*sizeof(long)-1;
        for(m=n/sizeof(long);m;--m)
          *((long *)s2)++=*((long *)s1)++;
        n&=sizeof(long)-1; }
      if(!n) return;
    } //do;while(*((char *)s2)++=*((char *)s1)++,--n);
    switch(n)
    {
      default:
	printf("-%d//%d//",n,z);abort();
      case 15:
	*((long *)s2)++=*((long *)s1)++;
	*((long *)s2)++=*((long *)s1)++;
	*((long *)s2)++=*((long *)s1)++;
	*((short *)s2)++=*((short *)s1)++;
	*((char *)s2)=*((char *)s1);
	break;
      case 14:
	*((long *)s2)++=*((long *)s1)++;
	*((long *)s2)++=*((long *)s1)++;
	*((long *)s2)++=*((long *)s1)++;
	*((short *)s2)=*((short *)s1);
	break;
      case 13:
	*((long *)s2)++=*((long *)s1)++;
	*((long *)s2)++=*((long *)s1)++;
	*((long *)s2)++=*((long *)s1)++;
	*((char *)s2)=*((char *)s1);
	break;
      case 12:
	*((long *)s2)++=*((long *)s1)++;
	*((long *)s2)++=*((long *)s1)++;
	*((long *)s2)=*((long *)s1);
	break;
      case 11:
	*((long *)s2)++=*((long *)s1)++;
	*((long *)s2)++=*((long *)s1)++;
	*((short *)s2)++=*((short *)s1)++;
	*((char *)s2)=*((char *)s1);
	break;
      case 10:
	*((long *)s2)++=*((long *)s1)++;
	*((long *)s2)++=*((long *)s1)++;
	*((short *)s2)=*((short *)s1);
	break;
      case 9:
	*((long *)s2)++=*((long *)s1)++;
	*((long *)s2)++=*((long *)s1)++;
	*((char *)s2)=*((char *)s1);
	break;
      case 8:
	*((long *)s2)++=*((long *)s1)++;
	*((long *)s2)=*((long *)s1);
	break;
      case 7:
	*((long *)s2)++=*((long *)s1)++;
	*((short *)s2)=*((short *)s1);
	break;
      case 6:
	*((long *)s2)++=*((long *)s1)++;
	*((short *)s2)=*((short *)s1);
	break;
      case 5:
	*((long *)s2)++=*((long *)s1)++;
	*((char *)s2)=*((char *)s1);
	break;
      case 4:
	*((long *)s2)=*((long *)s1);
	break;
      case 3:
	*((short *)s2)=*((short *)s1);
	*((char *)s2)=*((char *)s1);
	break;
      case 2:
	*((short *)s2)=*((short *)s1);
	break;
      case 1:
	*((char *)s2)=*((char *)s1);
	break;
    }
  }else
  { (char *)s1+=n;
    (char *)s2+=n;
    if(n>15)
    { if((long)s1&1)
      { *--((char *)s2)=*--((char *)s1);
        n--; }
      if(!((long)s2&1))
      { if((long)s1&2)
        { *--((short *)s2)=*--((short *)s1);
          n-=2; }
        for(m=n/(8*sizeof(long));m;--m)
        { *--((long *)s2)=*--((long *)s1); *--((long *)s2)=*--((long *)s1);
          *--((long *)s2)=*--((long *)s1); *--((long *)s2)=*--((long *)s1);
          *--((long *)s2)=*--((long *)s1); *--((long *)s2)=*--((long *)s1);
          *--((long *)s2)=*--((long *)s1); *--((long *)s2)=*--((long *)s1); }
        n&=8*sizeof(long)-1;
        for(m=n/sizeof(long);m;--m)
          *--((long *)s2)=*--((long *)s1);
        n&=sizeof(long)-1; }
      if(!n) return;
    } //do;while(*--((char *)s2)=*--((char *)s1),--n);
    switch(n)
    {
      default:
	printf("+%d//%d//",n,z);abort();
      case 15:
	*--((long *)s2)=*--((long *)s1);
	*--((long *)s2)=*--((long *)s1);
	*--((long *)s2)=*--((long *)s1);
	*--((short *)s2)=*--((short *)s1);
	*--((char *)s2)=*--((char *)s1);
	break;
      case 14:
	*--((long *)s2)=*--((long *)s1);
	*--((long *)s2)=*--((long *)s1);
	*--((long *)s2)=*--((long *)s1);
	*--((short *)s2)=*--((short *)s1);
	break;
      case 13:
	*--((long *)s2)=*--((long *)s1);
	*--((long *)s2)=*--((long *)s1);
	*--((long *)s2)=*--((long *)s1);
	*--((char *)s2)=*--((char *)s1);
	break;
      case 12:
	*--((long *)s2)=*--((long *)s1);
	*--((long *)s2)=*--((long *)s1);
	*--((long *)s2)=*--((long *)s1);
	break;
      case 11:
	*--((long *)s2)=*--((long *)s1);
	*--((long *)s2)=*--((long *)s1);
	*--((short *)s2)=*--((short *)s1);
	*--((char *)s2)=*--((char *)s1);
	break;
      case 10:
	*--((long *)s2)=*--((long *)s1);
	*--((long *)s2)=*--((long *)s1);
	*--((short *)s2)=*--((short *)s1);
	break;
      case 9:
	*--((long *)s2)=*--((long *)s1);
	*--((long *)s2)=*--((long *)s1);
	*--((char *)s2)=*--((char *)s1);
	break;
      case 8:
	*--((long *)s2)=*--((long *)s1);
	*--((long *)s2)=*--((long *)s1);
	break;
      case 7:
	*--((long *)s2)=*--((long *)s1);
	*--((short *)s2)=*--((short *)s1);
	*--((char *)s2)=*--((char *)s1);
	break;
      case 6:
	*--((long *)s2)=*--((long *)s1);
	*--((short *)s2)=*--((short *)s1);
	break;
      case 5:
	*--((long *)s2)=*--((long *)s1);
	*--((char *)s2)=*--((char *)s1);
	break;
      case 4:
	*--((long *)s2)=*--((long *)s1);
	break;
      case 3:
	*--((short *)s2)=*--((short *)s1);
	*--((char *)s2)=*--((char *)s1);
	break;
      case 2:
	*--((short *)s2)=*--((short *)s1);
	break;
      case 1:
	*--((char *)s2)=*--((char *)s1);
	break;
    }
  }
}
#else
/*
 * $Id: string_memmove.c,v 1.7 2006/09/22 09:02:51 obarthel Exp $
 *
 * :ts=4
 *
 * Portable ISO 'C' (1994) runtime library for the Amiga computer
 * Copyright (c) 2002-2006 by Olaf Barthel <olsen (at) sourcery.han.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Neither the name of Olaf Barthel nor the names of contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

void bcopy(const unsigned char * from,unsigned char *to,size_t len)
{
	#include "alignment.h"
	
	if(from < to && to < from + len)
	{
		to	+= len;
		from	+= len;

		/* The setup below is intended to speed up copying larger
		 * memory blocks. This can be very elaborate and should not be
		 * done unless a payoff can be expected.
		 */
		if(len > 4 * sizeof(long))
		{
			size_t distance;

			/* Try to align both source and destination to an even address. */
			if(IS_UNALIGNED(to) && IS_UNALIGNED(from))
			{
				(*--to) = (*--from);
				len--;
			}

			/* Try to align both source and destination to addresses which are
			 * multiples of four.
			 */
			if(len >= sizeof(short) && IS_SHORT_ALIGNED(to) && IS_SHORT_ALIGNED(from))
			{
				(*--to) = (*--from);
				(*--to) = (*--from);

				len -= sizeof(short);
			}

			/* Check the distance between source and destination. If it's shorter
			 * than a long word, don't dive into the copying routine below since
			 * the overlapping copying may clobber data.
			 */
			distance = (size_t)(to - from);

			/* If both source and destination are aligned to addresses which are
			 * multiples of four and there is still enough data left to be copied,
			 * try to move it in larger chunks.
			 */
			if(distance >= sizeof(long) && len >= sizeof(long) && IS_LONG_ALIGNED(to) && IS_LONG_ALIGNED(from))
			{
				unsigned long * _to		= (unsigned long *)to;
				unsigned long * _from	= (unsigned long *)from;

				/* An unrolled transfer loop, which shifts 32 bytes per iteration. */
				while(len >= 8 * sizeof(long))
				{
					/* The following should translate into load/store
					   opcodes which encode the access offsets (-1..-8)
					   into the respective displacement values. This
					   should help the PowerPC by avoiding pipeline
					   stalls (removing the predecrement on the address
					   will do that) but has no noticeable impact on the
					   68k platform (I checked). */

					_to[-1] = _from[-1];
					_to[-2] = _from[-2];
					_to[-3] = _from[-3];
					_to[-4] = _from[-4];
					_to[-5] = _from[-5];
					_to[-6] = _from[-6];
					_to[-7] = _from[-7];
					_to[-8] = _from[-8];

					_to		-= 8;
					_from	-= 8;

					/*
					(*--_to) = (*--_from);
					(*--_to) = (*--_from);
					(*--_to) = (*--_from);
					(*--_to) = (*--_from);
					(*--_to) = (*--_from);
					(*--_to) = (*--_from);
					(*--_to) = (*--_from);
					(*--_to) = (*--_from);
					*/

					len -= 8 * sizeof(long);
				}

				/* Try to mop up any small amounts of data still in need of
				 * copying...
				 */
				while(len >= sizeof(long))
				{
					(*--_to) = (*--_from);

					len -= sizeof(long);
				}		

				to	= (unsigned char *)_to;
				from	= (unsigned char *)_from;
			}
		}

		/* If there's anything left, copy the rest. */
		while(len-- > 0)
			(*--to) = (*--from);
	}
	else
	{
		/* The setup below is intended to speed up copying larger
		 * memory blocks. This can be very elaborate and should not be
		 * done unless a payoff can be expected.
		 */
		if(len > 4 * sizeof(long))
		{
			size_t distance;

			/* Try to align both source and destination to an even address. */
			if(IS_UNALIGNED(to) && IS_UNALIGNED(from))
			{
				(*to++) = (*from++);
				len--;
			}

			/* Try to align both source and destination to addresses which are
			 * multiples of four.
			 */
			if(len >= sizeof(short) && IS_SHORT_ALIGNED(to) && IS_SHORT_ALIGNED(from))
			{
				(*to++) = (*from++);
				(*to++) = (*from++);

				len -= sizeof(short);
			}

			/* Check the distance between source and destination. If it's shorter
			 * than a long word, don't dive into the copying routine below since
			 * the overlapping copying may clobber data.
			 */
			if(to >= from)
				distance = (size_t)(to - from);
			else
				distance = (size_t)(from - to);

			/* If both source and destination are aligned to addresses which are
			 * multiples of four and there is still enough data left to be copied,
			 * try to move it in larger chunks.
			 */
			if(distance >= sizeof(long) && len >= sizeof(long) && IS_LONG_ALIGNED(to) && IS_LONG_ALIGNED(from))
			{
				unsigned long * _to	= (unsigned long *)to;
				unsigned long * _from	= (unsigned long *)from;

				/* An unrolled transfer loop, which shifts 32 bytes per iteration. */
				while(len >= 8 * sizeof(long))
				{
					/* The following should translate into load/store
					   opcodes which encode the access offsets (0..7)
					   into the respective displacement values. This
					   should help the PowerPC by avoiding pipeline
					   stalls (removing the postincrement on the address
					   will do that) but has no noticeable impact on the
					   68k platform (I checked). */

					_to[0] = _from[0];
					_to[1] = _from[1];
					_to[2] = _from[2];
					_to[3] = _from[3];
					_to[4] = _from[4];
					_to[5] = _from[5];
					_to[6] = _from[6];
					_to[7] = _from[7];

					_to	+= 8;
					_from	+= 8;

					/*
					(*_to++) = (*_from++);
					(*_to++) = (*_from++);
					(*_to++) = (*_from++);
					(*_to++) = (*_from++);
					(*_to++) = (*_from++);
					(*_to++) = (*_from++);
					(*_to++) = (*_from++);
					(*_to++) = (*_from++);
					*/

					len -= 8 * sizeof(long);
				}

				/* Try to mop up any small amounts of data still in need of
				 * copying...
				 */
				while(len >= sizeof(long))
				{
					(*_to++) = (*_from++);

					len -= sizeof(long);
				}		

				to	= (unsigned char *)_to;
				from	= (unsigned char *)_from;
			}
		}

		/* If there's anything left, copy the rest. */
		while(len-- > 0)
			(*to++) = (*from++);
	}
}

#endif
