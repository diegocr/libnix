/**
 * readpassphrase() implementation for AmigaOS
 * Copyright (c) 2006, Diego Casorran <diegocr at users dot sf dot net>
 * All rights reserved.
 *
 * Redistribution  and  use  in  source  and  binary  forms,  with  or without
 * modification, are permitted provided that the following conditions are met:
 *
 * -  Redistributions  of  source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this  list  of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * -  Neither  the name of the author(s) nor the names of its contributors may
 * be  used  to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND  ANY  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE  DISCLAIMED.   IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE  FOR  ANY  DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR
 * CONSEQUENTIAL  DAMAGES  (INCLUDING,  BUT  NOT  LIMITED  TO,  PROCUREMENT OF
 * SUBSTITUTE  GOODS  OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION)  HOWEVER  CAUSED  AND  ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT,  STRICT  LIABILITY,  OR  TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: readpassphrase.c,v 0.1 2006/07/26 20:17:34 diegocr Exp $
 */

#include <proto/dos.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>

#define RPP_ECHO_OFF    0x00		/* Turn off echo (default). */
#define RPP_ECHO_ON     0x01		/* Leave echo on. */
#define RPP_REQUIRE_TTY 0x02		/* Fail if there is no tty. */
#define RPP_FORCELOWER  0x04		/* Force input to lower case. */
#define RPP_FORCEUPPER  0x08		/* Force input to upper case. */
#define RPP_SEVENBIT    0x10		/* Strip the high bit from input. */
#define RPP_STDIN       0x20		/* Read from stdin, not /dev/tty */

/*****************************************************************************/

char * readpassphrase(const char *prompt, char *buf, size_t bufsiz, int flags)
{
	BPTR input, output;
	char ch, *p, *end;
	LONG nr, save_errno;

	if (bufsiz == 0) {
		errno = EINVAL;
		return(NULL);
	}

	__fflush(stderr);
	__fflush(stdout);

	if ((flags & RPP_STDIN) ||
	    !(input = output = Open("CONSOLE:", MODE_OLDFILE))) {
		if((flags & RPP_REQUIRE_TTY) && !IsInteractive(Input())) {
			errno = ENOTTY;
			return(NULL);
		}
		input = Input();
		output = Output();
	}

	if(!(flags & RPP_STDIN) && (prompt != NULL))
	{
		Write(output, (STRPTR)prompt, strlen(prompt));
		Flush(output);
	}

	// disable echo
	if(!(flags & RPP_ECHO_ON))
	{
		// plus disable cursor (not really needed but funny ;)
		ULONG crs = 0x9B302070;
		Write(input, (STRPTR) &crs, sizeof(ULONG));

		SetMode( input, 1 );
	}

	end = buf + bufsiz - 1;
	for (p = buf; (nr = Read(input, &ch, 1)) == 1 && ch != '\n' && ch != '\r';) {
		if (p < end) {
			if ((flags & RPP_SEVENBIT))
				ch &= 0x7f;
			if (isalpha(ch)) {
				if ((flags & RPP_FORCELOWER))
					ch = tolower(ch);
				if ((flags & RPP_FORCEUPPER))
					ch = toupper(ch);
			}
			*p++ = ch;
		}
	}
	*p = '\0';

	save_errno = IoErr();
	if(!(flags & RPP_ECHO_ON))
	{
		// re-enable cursor
		ULONG crs = 0x9B207000;
		Write(input, (STRPTR) &crs, sizeof(ULONG)-1);

		// re-enable echo (unset RAW)
		SetMode( input, 0 );

		FPutC( output, '\n');
		Flush( output );
	}

	if(input != Input())
		Close(input);

	SetIoErr( save_errno );
	return(nr == -1 ? NULL : buf);
}

