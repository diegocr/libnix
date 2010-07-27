
#ifndef SMALL_DATA
#define A4(x) #x
#elif defined(mc68020)
#define A4(x) "a4@(" #x ":L)"
#else
#define A4(x) "a4@(" #x ":W)"
#endif

asm(
"		.globl	_div;"
"		.globl	_ldiv;"
"		.globl	___modsi3;"
"		.globl	___divsi3;"

/* D1.L = D0.L % D1.L signed */

"___modsi3:	moveml	sp@(4:W),d0/d1;"
"		jbsr	___divsi4;"
"		movel	d1,d0;"
"		rts;"

/* D0.L = D0.L / D1.L signed */

"_div:"
"_ldiv:"
"___divsi3:	moveml	sp@(4:W),d0/d1;"
"___divsi4:	movel	"A4(_UtilityBase)",a0;"
"		jmp	a0@(-150:W);"
);
