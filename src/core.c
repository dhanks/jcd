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

void *idle_loop(void *arg)
{
	signal (SIGPIPE, SIG_IGN);

	while(!(system_flags & SHUTDOWN))
	{
		errno = 0;
		jcd_core_loop();
	}

	vmlog(5, "jcd going down, system_flags & SHUTDOWN\n");
	shutdown_server();
	return (NULL);
}

void *jcd_core_loop(void)
{
	node *scan, *tmp;
	fd_set fds;
	struct timeval tv;
	int r = 0, largest;

	signal (SIGPIPE, SIG_IGN);

	/* Zero out the fd set */
	S_FD_ZERO(&fds);

	/* Walk through all the interfaces and set them */
	for(scan = l_interface->head; scan; scan = scan->next)
	{
		interface *i = (interface *) scan->data;

		if(i->flags & INTERFACE_UP)
		{
			S_FD_SET(i->handle.fd);
		}
	}

	/* Same thing for players, and we'll handle disconnects at the same time */
	for(scan = l_player->head; scan; scan = tmp)
	{
		player *p = (player *) scan->data;

		tmp = scan->next;

		if(p->handle.fd == -1 || p->mode == M_LOST || p->mode == M_DISCONNECT)
		{
			/* be nice and try? */
			if(p->buffer.pos > 0)
				buffer_send(p);
			vmlog(0, "disconnected socket %i\n", p->handle.fd);
			p->handle.interface->connections--;
			player_disconnect(p);
		}
		else
		{
			S_FD_SET(p->handle.fd);
			if(p->buffer.pos > 0)
				buffer_send(p);
		}
	}

	tv.tv_sec = 1;
	tv.tv_usec = 0;
	r = select (largest + 1, &fds, NULL, NULL, &tv);

	if(r > 0)
	{
		/* See which player is set */
		for(scan = l_player->head; scan; scan = tmp)
		{
			player *p = (player *) scan->data;

			tmp = scan->next;

			if(S_FD_ISSET(p->handle.fd))
			{
				SOCK_Handle_Input(p);
			}
		}

		/* See which interface has a new connection */
		for(scan = l_interface->head; scan; scan = scan->next)
		{
			interface *i = (interface *) scan->data;

			if(S_FD_ISSET(i->handle.fd) && i->flags & INTERFACE_UP)
			{
				player_connect(i);
				i->connections++;
			}
		}
	}
	return (NULL);
}
