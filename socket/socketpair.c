
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

// #define TEST

#ifndef AF_UNIX
# define AF_UNIX AF_INET
#endif
#ifndef AF_LOCAL
# define AF_LOCAL AF_UNIX
#endif
#ifndef PF_LOCAL
# define PF_LOCAL AF_LOCAL
#endif

static int invalid_socket (int socket)
{
#ifdef WIN32
	return (socket == INVALID_SOCKET);
#else
	return (socket < 0);
#endif /* WIN32 */
}

#if !defined (WIN32) && !defined (closesocket)
# define closesocket close
#endif /* !WIN32 */

#if defined(AMIGA) && !defined(__ixemul__)
# define close CloseSocket
# include <proto/socket.h>
#endif

#ifdef DEBUG
# include <stdio.h>
# define GIFT_TRACE( msg )	perror msg
#else
# define GIFT_TRACE( msg )	((void)0)
#endif

int socketpair (int family, int type, int protocol, int sv[2])
{
	long insock, outsock, newsock;
	struct sockaddr_in sock_in, sock_out;
	long len;

	/* windowz only has AF_INET (we use that for AF_LOCAL too) */
	if (family != AF_LOCAL && family != AF_INET)
		return -1;

	/* STRAM and DGRAM sockets only */
	if (type != SOCK_STREAM && type != SOCK_DGRAM)
		return -1;

	/* yes, we all love windoze */
	if ((family == AF_LOCAL && protocol != PF_UNSPEC && protocol != PF_LOCAL)
	    || (family == AF_INET && protocol != PF_UNSPEC && protocol != PF_INET))
		return -1;

	/* create the first socket */
	newsock = socket (AF_INET, type, 0);
	if (invalid_socket (newsock))
	{
		GIFT_TRACE (("first socket call failed"));
		return -1;
	}

	/* bind the socket to any unused port */
	sock_in.sin_family      = AF_INET;
	sock_in.sin_port        = 0;
	sock_in.sin_addr.s_addr = INADDR_ANY;
	if (bind (newsock, (struct sockaddr *) &sock_in, sizeof (sock_in)) < 0)
	{
		GIFT_TRACE (("bind failed"));
		closesocket (newsock);
		return -1;
	}
	len = sizeof (sock_in);
	if (getsockname (newsock, (struct sockaddr *) &sock_in, &len) < 0)
	{
		GIFT_TRACE (("getsockname error"));
		closesocket (newsock);
		return -1;
	}

	/* For stream sockets, create a listener */
	if (type == SOCK_STREAM)
		listen (newsock, 2);

	/* create a connecting socket */
	outsock = socket (AF_INET, type, 0);
	if (invalid_socket (outsock))
	{
		GIFT_TRACE (("second socket call failed"));
		closesocket (newsock);
		return -1;
	}

	/* For datagram sockets, bind the 2nd socket to an unused address, too */
	if (type == SOCK_DGRAM)
	{
		sock_out.sin_family       = AF_INET;
		sock_out.sin_port        = 0;
		sock_out.sin_addr.s_addr = INADDR_ANY;
		if (bind (outsock, (struct sockaddr *) &sock_out, sizeof (sock_out)) < 0)
		{
			GIFT_TRACE (("bind failed"));
			closesocket (newsock);
			closesocket (outsock);
			return -1;
		}
		len = sizeof (sock_out);
		if (getsockname (outsock, (struct sockaddr *) &sock_out, &len) < 0)
		{
			GIFT_TRACE (("getsockname error"));
			closesocket (newsock);
			closesocket (outsock);
			return -1;
		}
	}

	/* Force IP address to loopback */
	sock_in.sin_addr.s_addr = htonl (INADDR_LOOPBACK);
	if (type == SOCK_DGRAM)
		sock_out.sin_addr.s_addr = htonl (INADDR_LOOPBACK);

	/* Do a connect */
	if (connect (outsock, (struct sockaddr *) &sock_in, sizeof (sock_in)) < 0)
	{
		GIFT_TRACE (("connect error"));
		closesocket (newsock);
		closesocket (outsock);
		return -1;
	}

	if (type == SOCK_STREAM)
	{
		/* For stream sockets, accept the connection and close the listener */
		len = sizeof (sock_in);
		insock = accept (newsock, (struct sockaddr *) &sock_in, &len);
		if (invalid_socket (insock))
		{
			GIFT_TRACE (("accept error"));
			closesocket (newsock);
			closesocket (outsock);
			return -1;
		}
		closesocket (newsock);
	}
	else
	{
		/* For datagram sockets, connect the 2nd socket */
		if (connect (newsock, (struct sockaddr *) &sock_out, sizeof (sock_out)) < 0)
		{
			GIFT_TRACE (("connect error"));
			closesocket (newsock);
			closesocket (outsock);
			return -1;
		}
		insock = newsock;
	}

	/* set the descriptors */
	sv[0] = insock;
	sv[1] = outsock;

	/* we've done it */
	return 0;
}


#ifdef TEST

#include <stdlib.h>
#include <proto/exec.h>

struct Library *SocketBase = NULL;


int main( void )
{
	int pfd[2];
	
	if((SocketBase = OpenLibrary("bsdsocket.library",4)))
	{
		if(socketpair(AF_LOCAL, SOCK_STREAM, 0, pfd) != -1)
		{
			printf("socketpair() succedeed\n");
			printf("1: %ld, 2: %ld\n", pfd[0], pfd[1]);
			
			Wait( SIGBREAKF_CTRL_C );
			
			closesocket(pfd[0]);
			closesocket(pfd[1]);
		}
		
		CloseLibrary(SocketBase);
		
		return EXIT_SUCCESS;
	}
	
	return EXIT_FAILURE;
}

#endif /* TEST */

