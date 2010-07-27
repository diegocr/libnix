#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/socket.h>
#include <amitcp/socketbasetags.h>
#include <stdlib.h>
#include "stdio.h"
#include "stabs.h"

struct Library *SocketBase = NULL;
extern int errno, h_errno;
extern char *__procname;

DESTRUCTOR(closeSockets)
{
  if( SocketBase )
  {
    CloseLibrary( SocketBase );
    SocketBase = NULL;
  }
}

CONSTRUCTOR(openSockets)
{
  if(!(SocketBase = OpenLibrary("bsdsocket.library", 4)))
  {
    fprintf(stderr,"No TCP/IP Stack running!\a\n");
    exit(20);
  }

  SocketBaseTags(
	SBTM_SETVAL(SBTC_ERRNOPTR(sizeof(errno))),   (ULONG) &errno,
	SBTM_SETVAL(SBTC_HERRNOLONGPTR),             (ULONG) &h_errno,
	SBTM_SETVAL(SBTC_LOGTAGPTR),                 (ULONG)__procname,
  TAG_DONE );
}
