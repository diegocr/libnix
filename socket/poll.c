/* ***** BEGIN LICENSE BLOCK *****
 * Version: APL License
 * 
 * Copyright (c) 2009 Diego Casorran <dcasorran@gmail.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * ***** END LICENSE BLOCK ***** */


#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <poll.h>
#include <errno.h>

#include <proto/exec.h>
#include <proto/socket.h>

extern void __chkabort(void);

int poll( struct pollfd *pfds, unsigned int nfds, int timeout )
{
	unsigned int i;
	int maxfd = -1, ret;
	fd_set rset,wset,xset;
	struct timeval timeout_tv, *tvp = NULL;

	if (timeout >= 0)
	{
		timeout_tv.tv_sec = (timeout / 1000);
		timeout_tv.tv_usec = (timeout % 1000) * 1000;
		tvp = &timeout_tv;
	}

	if(pfds==NULL||nfds<1)
	{
	  if(pfds==NULL&&nfds<1&&timeout>=0)
	  {
	    ret = WaitSelect(0,NULL,NULL,NULL,tvp,NULL);
	    __chkabort();
	    return(ret);
	  }

	  errno=EINVAL;
	  return -1;
	}

	FD_ZERO (&rset);
	FD_ZERO (&wset);
	FD_ZERO (&xset);

	for (i = 0; i < nfds; i++)
	{
		pfds[i].revents = 0;

		if (pfds[i].events == 0)
			continue;

		if (pfds[i].fd > maxfd)
			maxfd = pfds[i].fd;

		if (pfds[i].events & POLLIN)
			FD_SET (pfds[i].fd, &rset);

		if (pfds[i].events & POLLOUT)
			FD_SET (pfds[i].fd, &wset);

		if (pfds[i].events & POLLERR)
			FD_SET (pfds[i].fd, &xset);
	}

	ret = WaitSelect (maxfd + 1, &rset, &wset, &xset, tvp, NULL);

	__chkabort ( ) ;

	if(ret == -1)
		return ret;

	for (i = 0; i < nfds; i++)
	{
		if (pfds[i].events == 0)
			continue;

		if (FD_ISSET (pfds[i].fd, &rset))
			pfds[i].revents |= POLLIN;

		if (FD_ISSET (pfds[i].fd, &wset))
			pfds[i].revents |= POLLOUT;

		if (FD_ISSET (pfds[i].fd, &xset))
			pfds[i].revents |= POLLERR;
	}

	return ret;
}
