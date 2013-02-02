/* Jade Coiled Dragon - a multi-user, multi-tasking chat environment
 * Copyright (C) 2000 Douglas R. Hanks Jr.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <jcd_super.h>

interface *interface_malloc(void)
{
	interface *i = (interface *) MALLOC(sizeof(interface));

	assert(i != NULL);
	memset(i, '\0', sizeof(i));
	return (i);
}

void interface_init(interface *in)
{
	static char id = 0; /* This will emulate the device number */

	memset(&in->handle, '\0', sizeof(in->handle));

	in->flags = 0;
	in->device = 0;
	in->id = id++; /* Beware of the 255 device limit */

	in->in.packets = 0;
	in->in.bytes = 0;
	in->in.errors = 0;
	in->in.dropped = 0;

	in->out.packets = 0;
	in->out.bytes = 0;
	in->out.errors = 0;
	in->out.dropped = 0;
	return;
}

int interface_listen(interface *in, int port)
{
	int			d = 1;
	struct protoent		*proto;


	in->handle.handle.sin_port        = htons(port);
	in->handle.handle.sin_family      = AF_INET;
	in->handle.handle.sin_addr.s_addr = INADDR_ANY;

	proto = (struct protoent *) getprotobyname("tcp");

	if(!proto)
		in->handle.fd = (int) socket(AF_INET, SOCK_STREAM, proto->p_proto);
	else
		in->handle.fd = (int) socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

	assert(setsockopt(in->handle.fd, SOL_SOCKET, SO_REUSEADDR, &d, sizeof(d)) == 0);

	if(ioctl(in->handle.fd, FIONBIO, &d) < 0)
	{
		fprintf(stderr, "ioctl failed on port %i because: %s (%i)\n", port, strerror(errno), errno);
		fflush(stderr);
		exit(errno);
	}

	if(bind(in->handle.fd, (struct sockaddr *) &in->handle.handle, sizeof(in->handle.handle)) < 0)
	{
		fprintf(stderr, "bind failed on port %i because: %s (%i)\n", port, strerror(errno), errno);
		fflush(stderr);
		exit(errno);
	}

	if(listen(in->handle.fd, 10) < 0)
	{
		fprintf(stderr, "listen failed on port %i because: %s (%i)\n", port, strerror(errno), errno);
		fflush(stderr);
		exit(errno);
	}

	in->handle.port = port;
	return (in->handle.fd);
}
