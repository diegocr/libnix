#if 0
/*  $NetBSD: bswap16.c,v 1.3 2003/12/04 13:57:31 keihan Exp $    */

/*
 * Written by Manuel Bouyer <bouyer@NetBSD.org>.
 * Public domain.
 */

#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
__RCSID("$NetBSD: bswap16.c,v 1.3 2003/12/04 13:57:31 keihan Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>

#if 0
#include <machine/bswap.h>

#undef bswap16

u_int16_t
bswap16(x)
	u_int16_t x;
{
	return ((x << 8) & 0xff00) | ((x >> 8) & 0x00ff);
}
#else
#undef bswap16
u_int16_t bswap16(u_int16_t var)
{
	__asm__ __volatile ("rorw #8, %0" : "=d" (var) : "0" (var));
	return (var);
}
#endif
#else

asm("\n\
.text; .even; .globl  _bswap16    ; .type  _bswap16    ,@function;  _bswap16    :\n\
	movl	%sp@(4),%d0\n\
	rolw	#8,%d0\n\
	rts\n\
");
#endif
