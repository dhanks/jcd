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

void get_handle(player *p, char *str)
{
	char *fault;

	if(!*str || !str)
	{
		tell_player(p, "To create a new character login as \"new\"\n");
		prompt(p, "jcd login: ");
		p->func = get_login;
		return;
	}

	if((fault = bad_name (str)))
	{
		tell_player(p, fault);
		tell_player(p, "To create a new character login as \"new\"\n");
		prompt(p, "jcd login: ");
		return;
	}

	{
		struct stat s;
		char tmp[MAX_LINE+1];

		snprintf(tmp, MAX_LINE, "%s/lib/players/%s", SERVER_ROOT, str);

		if(stat(tmp, &s) != -1)
		{
			tell_player(p, "That handle has already been registered, try another.\n");
			prompt(p, "Please enter the handle you wish to register: ");
			p->func = get_handle;
			return;
		}
	}

	{
		node *scan;

		for(scan = l_request->head; scan; scan = scan->next)
		{
			request *r = (request *) scan->data;

			if(!(r->flags & REQ_IP_BAN) && !(r->flags * REQ_NAME_BAN) &&
			   !(r->flags & REQ_APPROVED) && !(r->flags & REQ_PREAPPROVED) &&
			   !strcasecmp(r->name, str))
			{
				tell_player(p, "That handle has already been submitted for approval.  Please try again.\n");
				prompt(p, "Please enter the handle you wish to register: ");
				p->func = get_handle;
				return;
			}
		}
	}

	{
		request *r = request_malloc();

		request_init(r);
		strncpy(r->name, str, MAX_STRING);
		strncpy(r->ip, p->handle.ip, TWO_BYTES);
		strncpy(r->dns, p->handle.dns, TEN_BYTES+THREE_BYTES);
		r->submitted = time((time_t *) NULL);
		r->timeout = REQ_TWO_DAYS;
		r->one = p->handle.one;
		r->two = p->handle.two;
		r->three = p->handle.three;
		r->four = p->handle.four;
		LIST_ADD(l_request, r, sizeof(request));

		vmlog(1, "submitted request for `%s'\n", str);
	}

	tell_player(p, dfile_data("request_msg"));
	p->mode = M_LOST;
	return;
}

void get_login(player *p, char *str)
{
	char *fault;
	node *scan;

	if(!*str || !str)
	{
		tell_player(p, "To create a new character login as \"new\"\n");
		prompt(p, "jcd login: ");
		p->func = get_login;
		return;
	}

	str = trim_end (str);

	lower_case (str);

	if((fault = bad_name (str)))
	{
		tell_player(p, fault);
		tell_player(p, "To create a new character login as \"new\"\n");
		prompt(p, "jcd login: ");
		p->func = get_login;
		return;
	}


	if(!strcasecmp(str, "quit"))
	{
		p->mode = M_LOST;
		return;
	}

	if(!strcasecmp(str, "who"))
	{
		tell_player(p, "Not on this system you don't.\n");
		tell_player(p, "To create a new character login as \"new\"\n");
		prompt(p, "jcd login: ");
		return;
	}

	if(!strcasecmp(str, "new"))
	{
		tell_player(p, dfile_data("newbie_msg"));
		prompt(p, "Please enter the handle you wish to register: ");
		p->func = get_handle;
		return;
	}

	/* hack hack FIXME */

	if(!strcasecmp(str, "admin"))
		goto admin_clear;

	/* request handler */
	for(scan = l_request->head; scan; scan = scan->next)
	{
		request *r = (request *) scan->data;

		if(r->flags & REQ_APPROVED || r->flags & REQ_PREAPPROVED)
		{
			tell_player(p, "Yes, you have been approved.  Get lost.\n");
			p->mode = M_LOST;
			return;
		}

		if(r->flags & REQ_NAME_BAN)
		{
			if(!strcasecmp(r->name, str))
			{
				tell_player(p, "That name has been banished.  Disconnecting.\n");
				vmlog(1, "login attemped to use banished name `%s'\n", str);
				p->mode = M_LOST;
				return;
			}
		}

		if(r->flags & REQ_NOTIFY)
			if(!strcasecmp(r->name, str))
				vmlog(7, "get_login: `%s' successfully logged in from %s\n", str, p->handle.ip);
	}

	for(scan = l_player->head; scan; scan = scan->next)
	{
		player *p2 = (player *) scan->data;

		if(!strcasecmp(p2->lower_name, str) && p2 != p)
		{
			tell_player(p, "There is already someone on the program with that name.\n");
			tell_player(p, "To create a new character login as \"new\"\n");
			prompt(p, "jcd login: ");
			return;
		}
	}

	{
		struct stat s;
		char tmp[MAX_LINE+1];

		snprintf(tmp, MAX_LINE, "%s/lib/players/%s", SERVER_ROOT, str);
		stat(tmp, &s);

		if(stat(tmp, &s) == -1)
		{
			tell_player(p, "That character doesn't exist, type 'new' to create a new character.\n");
			prompt(p, "jcd login: ");
			return;
		}

		if(load_player(p, tmp) == -1)
		{
			vmlog(8, "get_login:  load_player returned -1 on '%s', terminating connection.\n", tmp);
			p->mode = M_LOST;
			return;
		}

		strncpy(p->lower_name, p->name, 19);
		tell_player(p, "\n--\nRecord loaded\n--\n");
		goto login_continue;
	}

	admin_clear:

	strncpy(p->name, str, 19);
	strncpy(p->lower_name, str, 19);

	login_continue:

	tell_player(p, "\n");
	top_banner(p, p->ch);
	tell_player(p, dfile_data("motd_msg"));
	middle_banner(p, p->ch);
	static_look(p);
	bottom_banner(p, p->ch);
	p->func = 0;
	p->opts |= TEL_PROMPT;
	p->flags |= LOGGED_IN;
	p->flags |= WW;
	p->privs |= NEWBIE;

	{
		int i = 0;

		for (i = 0; admin_list[i]; i++)
			if(!strcasecmp(p->name, admin_list[i]))
					p->privs |= (RESIDENT | MAIL | NEWS | GAMES | SAGE | BUILDER | PSU | SU | ASU | SETHBOY | LOWER_ADMIN | ADMIN | HCADMIN | CODER | SYSADMIN);
	}

	tell_hack(p, "( %s enters the coil of the dragon. )\n", p->name);

	if(p->privs & LOWER_ADMIN || p->privs & ADMIN)
		tell_player(p, "%s[ Welcome back, Administrator %s.  JCD has awaited your return. ]%s\n", MOD_BOLD, p->name, NOR);

	prompt(p, p->prompt);
	return;
}

