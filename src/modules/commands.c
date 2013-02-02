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

char *name(void)
{
	return ("commands.so");
}

void reg(struct mod *m)
{
	mod_comm_register(m, ".",		"comm_tell",		NEWBIE,		0);
	mod_comm_register(m, ",",		"comm_remote",		NEWBIE,		0);
	mod_comm_register(m, ">",		"comm_tell",		NEWBIE,		0);
	mod_comm_register(m, "<",		"comm_remote",		NEWBIE,		0);
	mod_comm_register(m, ";",		"comm_emote",		NEWBIE,		0);
	mod_comm_register(m, ":",		"comm_emote",		NEWBIE,		0);
	mod_comm_register(m, "'",		"comm_say",		NEWBIE,		0);
	mod_comm_register(m, "`",		"comm_say",		NEWBIE,		0);
	mod_comm_register(m, "\"",		"comm_say",		NEWBIE,		0);
	mod_comm_register(m, "l",		"comm_look",		NEWBIE,		0);
	mod_comm_register(m, "dfile",		"comm_dfile",		NEWBIE,		0);
	mod_comm_register(m, "emote",		"comm_emote",		NEWBIE,		0);
	mod_comm_register(m, "look",		"comm_look",		NEWBIE,		0);
	mod_comm_register(m, "remote",		"comm_remote",		NEWBIE,		0);
	mod_comm_register(m, "req",		"comm_req",		NEWBIE,		0);
	mod_comm_register(m, "requests",	"comm_req",		NEWBIE,		0);
	mod_comm_register(m, "say",		"comm_say",		NEWBIE,		0);
	mod_comm_register(m, "tell",		"comm_tell",		NEWBIE,		0);
	mod_comm_register(m, "quit",		"comm_quit",		NEWBIE,		0);
	mod_comm_register(m, "w",		"comm_who",		NEWBIE,		0);
	mod_comm_register(m, "who",		"comm_who",		NEWBIE,		0);
	return;
}

char asks(char *str)
{
	{
	while (*str)
		str++;
	if (*(str - 1))
	{
		return *(--str);
	}
	return 0;
	}
}

char *dfile_state(DFILE *df)
{
	switch(df->state)
	{
		case (dfsNormal):
			return "NORMAL";
			break;
		case (dfsVirgin):
			return "VIRGIN";
			break;
		case (dfsOpen):
			return "OPEN";
			break;
		case (dfsClosed):
			return "CLOSED";
			break;
		case (dfsError):
			return "ERROR";
			break;
		default:
			return "UNKNOWN";
			break;
	}
}

char *dfile_lastr(DFILE *df)
{
	static char buffer[80];
	struct tm *t = localtime(&df->last_accessed);

	strftime((char *) &buffer, 79, "%H:%M:%S - %b %d", t);
	return buffer;
}

char *req_ban(request *r)
{
	static char buffer[80];

	memset(buffer, '\0', sizeof(buffer));

	if(r->flags & REQ_NAME_BAN)
	{
		snprintf(buffer, 79, "(HB) %s", r->name);
		return buffer;
	}

	if(r->flags & REQ_IP_BAN)
	{
		snprintf(buffer, 79, "(IPB) %i.%i.%i.%i", r->one, r->two, r->three, r->four);
		return buffer;
	}

	if(r->flags & REQ_NOTIFY)
		return "NOTIFY_ONLY";

	if(r->flags & REQ_APPROVED)
		return "APPROVED";

	if(r->flags & REQ_PREAPPROVED)
		return "PREAPPROVED";

	if(r->flags & REQ_ALTERNATE)
		return "BLACKBOOK";

	return "*NEED APRVL*";
}

char *req_apr(request *r)
{
	if(r->flags & REQ_APPROVED || r->flags & REQ_PREAPPROVED)
		return "yes";
	else
		return "no";
}

int req_show(player *p)
{
	center_banner(p, '/', p->ch, '\\', "Login Request Synopsis (use \"request <name>\" for detail)");
	tell_player(p, "                NAME READ TIMEOUT STATE                APPROVAL\n");

	{
		node *scan;

		for(scan = l_request->head; scan; scan = scan->next)
		{
			request *r = (request *) scan->data;

			tell_player(p, "%20s %-4i %-7s %-20s %s\n", r->name, r->read,
			int2smalltime(r->timeout), req_ban(r), req_apr(r));
		}
	}

	middle_banner(p, p->ch);
	tell_player(p, " Legend:    HB  = Handle Banish\n");
	tell_player(p, "            IPB = IP Banish\n");
	bottom_banner(p, p->ch);
	return 0;
}

char *req_flag(unsigned int flags)
{
	static char buffer[80];
	char *ptr = buffer;
	char z = 0;

	memset(buffer, '\0', sizeof(buffer));

	if(flags & REQ_PREAPPROVED)
	{
		strcpy(ptr, "PREAPPRD");
		ptr = strchr(ptr, 0);
		z = 1;
	}
	if(flags & REQ_APPROVED)
	{
		z==1?*ptr++='|':0;
		strcpy(ptr, "APPRD");
		ptr = strchr(ptr, 0);
		z = 1;
	}
	if(flags & REQ_NOTIFY)
	{
		z==1?*ptr++='|':0;
		strcpy(ptr, "NOTIFY");
		ptr = strchr(ptr, 0);
		z = 1;
	}
	if(flags & REQ_ALTERNATE)
	{
		z==1?*ptr++='|':0;
		strcpy(ptr, "ALT");
		ptr = strchr(ptr, 0);
		z = 1;
	}
	if(flags & REQ_IP_BAN)
	{
		z==1?*ptr++='|':0;
		strcpy(ptr, "IPB");
		ptr = strchr(ptr, 0);
		z = 1;
	}
	if(flags & REQ_NAME_BAN)
	{
		z==1?*ptr++='|':0;
		strcpy(ptr, "HB");
		ptr = strchr(ptr, 0);
	}

	*ptr = 0;
	return buffer;
}

int req_show_detail(player *p, request *r)
{
	center_banner(p, '/', p->ch, '\\', "Request Detail");
	tell_player(p, " NAME:     %-25s ALTERNATE: %s\n", r->name, r->alternate);
	tell_player(p, " DNS:      %-25s IP:        %s\n", r->dns, r->ip);
	tell_player(p, " READ:     %-25i FLAGS:     %i (%s)\n", r->read, r->flags, req_flag(r->flags));
	tell_player(p, " TIMEOUT:  %-25s SUBMITED:  %s\n", int2smalltime(r->timeout), time2date(r->submitted));
	tell_player(p, " AUTHOR:   %-25s PASSWORD:  ", r->by);
	if(p->privs & ADMIN)
		tell_player(p, "%s\n", r->password);
	else
		tell_player(p, "xxxxxx\n");
	tell_player(p, " IP OCTET: A = %i, B = %i, C = %i, E = %i\n", r->one, r->two, r->three, r->four);
	bottom_banner(p, p->ch);
	r->read++;
	return 0;
}

int comm_req(player *p, char *str)
{
	node *scan;

	if(!*str)
		return req_show(p);

	for(scan = l_request->head; scan; scan = scan->next)
	{
		request *r = (request *) scan->data;

		if(!strncasecmp(r->name, str, strlen(str)))
			return req_show_detail(p, r);
	}

	tell_player(p, "Couldn't find that request.  See `requests' for a list.\n");
	return -1;
}


int comm_dfile(player *p, char *str)
{
	char *a;
	node *scan;

	if(!*str)
	{
		tell_player(p, " Usage : dfile [list|load <file>|reload|unload] <file>\n");
		return (-1);
	}

	a = next_space(str);
	*a++ = 0;
	str = trim_end(str);

	if(!strcasecmp(str, "list"))
	{
		center_banner(p, '/', p->ch, '\\', "Dynamic Files");
		tell_player(p, "                 FILE ACCSD RELOD TIMEOUT STATE      SIZE   LASTR\n");

		for(scan = l_dfile->head; scan; scan = scan->next)
		{
			DFILE *df = (DFILE *) scan->data;

			tell_player(p, " %20s %5i %-5i %7i %-10s %-6i %s\n", df->name, df->accessed, df->reloaded, df->timeout,
			dfile_state(df), df->size, dfile_lastr(df));
		}

		bottom_banner(p, p->ch);
		return (0);
	}
	else if(!strcasecmp(str, "load"))
	{
		struct stat s;
		char buf[80];

		if(!*a)
		{
			tell_player(p, " Usage : dfile [list|load <file>|reload|unload <file>]\n");
			return (-1);
		}

		if(strpbrk(a, "."))
		{
			tell_player(p, "I can't allow dots (.) in the path.\n");
			return (-1);
		}

		snprintf(buf, 79, "%s/lib/files/%s", SERVER_ROOT, a);

		if(stat(buf, &s) != 0)
		{
			tell_player(p, "%s (%i)\n", strerror(errno), errno);
			vmlog(3, "comm_dfile: couldn't load `%s', reason: %s (%i)\n", buf, strerror(errno), errno);
			return (-1);
		}
		else
		{
			if(dfile_create(a, buf, O_RDONLY, 0) == 0)
			{
				tell_player(p, "%s successfully loaded.\n", buf);
				return (0);
			}
			else
			{
				tell_player(p, "dfile_create returned a non-zero value, `%s' not loaded properly.\n", a);
				return (-1);
			}
		}
	}
	else if(!strcasecmp(str, "reload"))
	{
		node *scan;
		unsigned int count = 0;

		for(count = 0, scan = l_dfile->head; scan; scan = scan->next)
		{
			DFILE *df = (DFILE *) scan->data;

			FREE(df->data);
			if(load_dfile(df, df->path, O_RDONLY) == 0)
			{
				df->reloaded++;
				count++;
			}
			else
			{
				tell_player(p, "Error reloading `%s' so I'm removing it.\n", df->name);
				LIST_REMOVE(l_dfile, df);
				FREE(df);
			}
		}

		tell_player(p, "Reloaded %i files.\n", count);
		return (0);
	}
	else if(!strcasecmp(str, "unload"))
	{
		node *scan;

		for(scan = l_dfile->head; scan; scan = scan->next)
		{
			DFILE *df = (DFILE *) scan->data;

			if(!strcasecmp(a, df->name))
			{
				if(df->flags & DFS_SYSTEM)
				{
					tell_player(p, "You can't unload protected system files.\n");
					return (-1);
				}

				LIST_REMOVE(l_dfile, df);
				if(df->data)
					FREE(df->data);
				FREE(df);
				tell_player(p, "Unloaded `%s'\n", a);
				return (0);
			}
		}

		tell_player(p, "Couldn't find that file.\n");
		return (-1);
	}
	else
	{
		tell_player(p, " Usage : dfile [list|load <file>|reload|unload <file>]\n");
		return (-1);
	}
}

int comm_remote(player *p, char *str)
{
	player *p2;
	char *msg;

	if(!*str)
	{
		tell_player(p, " Usage : remote <person> <something>\n");
		return (-1);
	}

	msg = strchr(str, ' ');

	if(!msg)
	{
		tell_player(p, " Usage : remote <person> <something>\n");
		return (-1);
	}

	*msg++ = 0;

	str = trim_end(str);
	p2 = find_player_name (str);

	if(!p2)
	{
		tell_player(p, " Couldn't find anyone with the name of `%s'\n", str);
		return (-1);
	}

	if(p2 == p)
	{
		tell_player(p, " Bu bu bu bu bu bu, what?\n", str);
		return (-1);
	}

	tell_player(p2, "%s> %s %s%s\n", MOD_BOLD, p->name, msg, NOR);
	tell_player(p, "You emote '%s %s' to %s\n", p->name, msg, p2->name);

	return (0);
}

int comm_tell(player *p, char *str)
{
	player *p2;
	char *msg, *mid;

	if(!*str)
	{
		tell_player(p, " Usage : tell <person> <something>\n");
		return (-1);
	}

	msg = strchr(str, ' ');

	if(!msg)
	{
		tell_player(p, " Usage : tell <person> <something>\n");
		return (-1);
	}

	*msg++ = 0;

	str = trim_end(str);
	p2 = find_player_name (str);

	if(!p2)
	{
		tell_player(p, " Couldn't find anyone with the name of `%s'\n", str);
		return (-1);
	}

	if(p2 == p)
	{
		tell_player(p, " Bu bu bu bu bu bu, what?\n", str);
		return (-1);
	}

	switch (asks (msg))
	{
		case '?':
			mid = "ask";
			break;
		case '!':
			mid = "exclaim";
			break;
		default:
			mid = "tell";
			break;
	}

	tell_player(p2, "%s> %s %ss you, '%s'%s\n", MOD_BOLD, p->name, mid, msg, NOR);
	tell_player(p, "You %s %s, '%s'\n", mid, p2->name, msg);

	return (0);
}


int comm_quit(player *p, char *str)
{
	tell_player(p, "Come back again!\n");
	p->mode = M_LOST;
	p->opts &= ~TEL_PROMPT;
	return (0);
}

int comm_who(player *p, char *str)
{
	node *scan;
	int count = 0;
	char tmp[80];

	for(scan = l_player->head; scan; scan = scan->next, count++);
	sprintf(tmp, "%i users online", count);
	center_banner(p, '/', p->ch, '\\', tmp);
	for(scan = l_player->head; scan; scan = scan->next)
	{
		player *p2 = (player *) scan->data;

		if(p2->flags & LOGGED_IN)
			tell_player(p, " %s from %s (%s:%i)\n", p2->name, p2->handle.dns, p2->handle.ip, p2->handle.port);
		else
			tell_player(p, " <LOGGING IN> from %s (%s:%i)\n", p2->handle.dns, p2->handle.ip, p2->handle.port);
	}
	bottom_banner(p, p->ch);
	return (0);
}

int comm_look(player *p, char *str)
{
	node *scan;

	top_banner(p, p->ch);
	tell_player(p, dfile_data("room_msg"));
	tell_player(p, "\n");
	for(scan = l_player->head; scan; scan = scan->next)
	{
		player *p2 = (player *) scan->data;

		if(p2 != p && p2->flags & LOGGED_IN)
			tell_player(p, "%s %s\n", p2->name, p2->groundstring);
	}
	bottom_banner(p, p->ch);
	return (0);
}

int comm_emote(player *p, char *str)
{
	if(!*str)
	{
		tell_player(p, " Usage : emote <emotion>\n");
		return (-1);
	}

	str = trim_end(str);

	tell_hack(p, "- %s %s\n", p->name, str);
	tell_player(p, "You emote: %s %s\n", p->name, str);
	return (0);
}

int comm_say(player *p, char *str)
{
	char *mid;

	if(!*str)
	{
		tell_player(p, " Usage : say <message>\n");
		return (-1);
	}

	str = trim_end(str);

	switch(asks(str))
	{
		case '?':
			mid = "ask";
			break;
		case '!':
			mid = "exclaim";
			break;
		default:
			mid = "say";
			break;
	}

	tell_hack(p, "- %s %ss, '%s'\n", p->name, mid, str);
	tell_player(p, "You %s, '%s'\n", mid, str);
	return (0);
}
