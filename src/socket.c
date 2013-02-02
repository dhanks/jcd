/* Jade Coiled Dragon - a multi-user, multi-tasking chat environment
 * Copyright (C) 2000 Douglas R. Hanks Jr.
 *                    Stephen A. Olesen
 *                    Matvei Nicoliavich Busigin
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

void shutdown_server(void)
{
	{
		node *scan, *tmp;

		for(scan = l_interface->head; scan; scan = tmp)
		{
			interface *i = (interface *) scan->data;

			tmp = scan->next;

			shutdown(i->handle.fd, 2);
			close(i->handle.fd);
			FREE(i);
		}
	}

	{
		node *scan, *tmp;

		for(scan = l_player->head; scan; scan = tmp)
		{
			player *p = (player *) scan->data;

			tmp = scan->next;

			shutdown(p->handle.fd, 2);
			close(p->handle.fd);
			FREE(p);
		}
	}
}

void do_backspace(player *p)
{
	if (p->ibuffer_ptr)
		p->ibuffer_ptr--;
	p->ibuffer[p->ibuffer_ptr] = '\0';

	if ((p->opts & TEL_ECHO))
		write(p->handle.fd, (char *) "\b \b", 3);
}

int sock_write(int fd, void *buf, size_t size)
{
	size_t sent = 0, blocks = 0, x;
	char *cptr;

	cptr = (char *) buf;
	lastSockState = lssOKAY;
	Chunks = 0;
	if (fd < 0)
	{
		return (-1);
	}

	if (size < swBLOCKSIZE)
	{
		return (write (fd, buf, size));
	}

	blocks = size / swBLOCKSIZE;

	if ((blocks * swBLOCKSIZE) != size)
		blocks++;

	for (x = 0; x < blocks; x++, Chunks++)
	{
		if (x != (blocks - 1))
		{
			if (((sent = write (fd, (void *) cptr, swBLOCKSIZE)) != swBLOCKSIZE) || errno == EINTR)
			{
				lastSockState = lssERROR;
				return (sent);
			}
			cptr += (int) swBLOCKSIZE;
		}
		else
		{
			size_t left = size - (swBLOCKSIZE * x);

			if (((sent = write (fd, (void *) cptr, left)) != left) || errno == EINTR)
			{
				lastSockState = lssERROR;
				return (sent);
			}
		}
	}
	return (sent);
}

void *sock_resolve(void *v)
{
	sock *s = (sock *) v;
	struct hostent *tmp;

	signal(SIGPIPE, SIG_IGN);
	tmp = gethostbyaddr ((char *) &s->handle.sin_addr, sizeof (s->handle.sin_addr), AF_INET);
	if (tmp)
		strncpy(s->dns, tmp->h_name, 99);
	else
		strncpy(s->dns, s->ip, 99);

	sscanf(s->ip, "%d.%d.%d.%d", (int *) &s->one, (int *) &s->two, (int *) &s->three, (int *) &s->four);
	return (NULL);
}

int sock_accept(sock *l, sock *s)
{
	int nfd;
	struct sockaddr *tmp = (struct sockaddr *) &s->handle;
	socklen_t i = sizeof(s->handle);

	s->fd = accept(l->fd, tmp, &i);
	nfd = dup(s->fd);
	close(s->fd);
	s->fd = nfd;

	i = 1;
	setsockopt (s->fd, SOL_SOCKET, SO_REUSEADDR, &i, 0);
	ioctl (s->fd, FIONBIO, &i);
	strncpy(s->ip, inet_ntoa(s->handle.sin_addr), sizeof(s->ip));
	s->port = ntohs (s->handle.sin_port);
	sock_resolve(s);
	return (0);
}

