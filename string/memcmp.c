#include <string.h>

#if 0

int memcmp(const void *s1,const void *s2,size_t n)
{ const unsigned char *p1=s1,*p2=s2;
  unsigned long r,c;

  if ((r=n))
    do;while(r=*p1++,c=*p2++,!(r-=c) && --n);
  return r;
}

#else
/*
 * $Id: string_memcmp.c,v 1.7 2006/01/08 12:04:26 obarthel Exp $
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

#include <errno.h>
#include "alignment.h"

static __inline__ int __memcmp(const unsigned char *m1,const unsigned char *m2,size_t len)
{
	int result = 0;

	/* The setup below is intended to speed up searching in larger
	 * memory blocks. This can be very elaborate and should not be
	 * done unless a payoff can be expected.
	 */
	if(len > 4 * sizeof(long))
	{
		/* Try to align both memory blocks to even addresses. */
		if(IS_UNALIGNED(m1) && IS_UNALIGNED(m2))
		{
			len--;

			if((*m1) != (*m2))
			{
				result = (*m1) - (*m2);
				goto out;
			}

			m1++;
			m2++;
		}

		/* Try to align both memory blocks to addresses which are a
		 * long word multiple.
		 */
		if(len >= sizeof(short) && IS_SHORT_ALIGNED(m1) && IS_SHORT_ALIGNED(m2))
		{
			const short * _m1 = (const short *)m1;
			const short * _m2 = (const short *)m2;

			/* Check if the two words are different, and if they are,
			 * figure out down below which of the bytes were different.
			 */
			if((*_m1++) != (*_m2++))
				goto out;

			m1 = (const unsigned char *)_m1;
			m2 = (const unsigned char *)_m2;

			len -= sizeof(short);
		}

		/* If both memory blocks are aligned to addresses which can be
		 * read directly one long word at a time, perform the comparison
		 * in that manner.
		 */
		if(len >= sizeof(long) && IS_LONG_ALIGNED(m1) && IS_LONG_ALIGNED(m2))
		{
			const long * _m1 = (const long *)m1;
			const long * _m2 = (const long *)m2;

			do
			{
				/* If the two long words are different, figure out down
				 * below which of the bytes were different.
				 */
				if((*_m1) != (*_m2))
				{
					m1 = (const char *)_m1;
					m2 = (const char *)_m2;

					goto out;
				}

				_m1++;
				_m2++;

				len -= sizeof(long);
			}
			while(len >= sizeof(long));

			m1 = (const unsigned char *)_m1;
			m2 = (const unsigned char *)_m2;
		}
	}

 out:

	/* If there are bytes left in need of comparison, take
	 * care of them here. This also includes 'aborted'
	 * comparison attempts from above.
	 */
	while(len-- > 0)
	{
		if((*m1) != (*m2))
		{
			result = (*m1) - (*m2);
			break;
		}

		m1++;
		m2++;
	}

	return(result);
}

#ifndef __INLINESTUB

int memcmp(const void *ptr1, const void *ptr2, size_t len)
{
	int result = 0;

	if(ptr1 == NULL || ptr2 == NULL || (signed)len < 1)
	{
		errno = EFAULT;
	}
	else if(ptr1 != ptr2)
	{
		result = __memcmp((const unsigned char *)ptr1,(const unsigned char *)ptr2,len);
	}
	return(result);
}
#endif
#endif
