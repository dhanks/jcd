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

player *player_malloc(void)
{
	player *i = (player *) MALLOC(sizeof(player));

	if(i == NULL)
	{
		fatal("malloc_player:  out of memory.\n");
		return (NULL);
	}
	else
	{
		memset(i, '\0', sizeof(i));
		return (i);
	}
}

player *find_player_name(char *name)
{
	player *p;
	node *scan;

	for (scan = l_player->head; scan; scan = scan->next)
	{
		p = (player *) scan->data;

		if (!strncasecmp(name, p->name, strlen(name)))
			return p;
	}

	return NULL;
}

void player_init(player *tmp)
{
	tmp->screen.height = 24;
	tmp->screen.width = 80;
	tmp->screen.linewrap = 74;
	tmp->screen.wordwrap = 10;
	tmp->idle = 0;
	tmp->auto_term = 0;
	tmp->opts = 0;
	tmp->mode = 0;
	tmp->flags = 0;
	tmp->privs = 0;
	strcpy(tmp->prompt, "-> ");
	strcpy(tmp->groundstring, "stands here.");
	strcpy(tmp->title, "the newbie, so please treat me nicely.");
	tmp->prefix[0] = 0;
	tmp->name[0] = 0;
	tmp->lower_name[0] = 0;
	tmp->ch = '-';
	tmp->handle.stream = 0;
	tmp->handle.out.packets = 0;
	tmp->handle.out.bytes = 0;
	tmp->handle.in.packets = 0;
	tmp->handle.in.bytes = 0;

	tmp->opts = TEL_GA | TEL_EOR;
	tmp->opts &= ~TEL_PROMPT;

	memset (&tmp->handle, '\0', sizeof (tmp->handle));
	tmp->handle.fd = -1;
	return;
}

void player_logout(player *p, char *file, int line, int e)
{
	p->mode = M_DISCONNECT;
	vmlog(1, "[%s, line %i (errno %s)]: disconnecting player, socket: %i\n", file, line, strerror(e), p->handle.fd);
	return;
}

void player_disconnect(player *p)
{
	if(p->handle.fd != -1)
	{
		shutdown(p->handle.fd, 2);
		close(p->handle.fd);
	}

	if(strlen(p->name) > 1)
		tell_hack(p, "( %s leaves the perimeter of the dragon. )\n", p->name);
	else
		tell_hack(p, "( a presence leaves the perimeter of the dragon. )\n");


	LIST_REMOVE(l_player, p);
	FREE(p);
	return;
}

int player_connect(interface *i)
{
	player *p = player_malloc();
	node *scan;

	player_init(p);
	LIST_ADD(l_player, p, sizeof(player));
	p->handle.interface = i;
	p->handle.interface->out.packets++;
	p->mode = 0;
	sock_accept(&i->handle, &p->handle);

	if(p->handle.interface->connections > 99)
	{
		p->mode = M_DISCONNECT;
		return (-1);
	}

	for(scan = l_request->head; scan; scan = scan->next)
	{
		request *r = (request *) scan->data;

		if(r->flags & REQ_IP_BAN)
		{
			if(r->one == '*' || r->one == p->handle.one)
				if(r->two == '*' || r->two == p->handle.two)
					if(r->three == '*' || r->three == p->handle.three)
						if(r->four == '*' || r->four == p->handle.four)
						{

							tell_player(p, dfile_data("banished_msg"));
							vmlog(5, "login attemped from banished octet %s\n", p->handle.ip);
							p->mode = M_LOST;
							return 0;
						}
		}
	}

	tell_player(p, "%c%c%c", IAC, DO, TELOPT_NAWS);
	tell_player(p, "%c%c%c%c%c%c", IAC, SB, TELOPT_TTYPE, 1, IAC, SE);
	tell_player(p, dfile_data("connect_msg"));
	tell_player(p, "To create a new character login as \"new\"\n");
	prompt(p, "jcd login: ");
	p->func = get_login;
	return (0);
}
