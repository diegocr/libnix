#if 0
/*  $NetBSD: bswap32.c,v 1.3 2003/12/04 13:57:31 keihan Exp $    */

/*
 * Written by Manuel Bouyer <bouyer@NetBSD.org>.
 * Public domain.
 */

#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
__RCSID("$NetBSD: bswap32.c,v 1.3 2003/12/04 13:57:31 keihan Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>

#if 0
#include <machine/bswap.h>

#undef bswap32

u_int32_t
bswap32(x)
	u_int32_t x;
{
	return	((x << 24) & 0xff000000 ) |
		((x <<  8) & 0x00ff0000 ) |
		((x >>  8) & 0x0000ff00 ) |
		((x >> 24) & 0x000000ff );
}
#else
#undef bswap32
u_int32_t bswap32(u_int32_t var)
{
	__asm__ __volatile (
		"rorw #8, %0; swap %0; rorw #8, %0" : "=d" (var) : "0" (var));
	return (var);
}
#endif
#else
asm("\n\
.text; .even; .globl  _bswap32    ; .type  _bswap32    ,@function;  _bswap32    :\n\
	movl	%sp@(4),%d0\n\
	rolw	#8,%d0\n\
	swap	%d0\n\
	rolw	#8,%d0\n\
	rts\n\
");
#endif
