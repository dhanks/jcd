/* Jade Coiled Dragon - a multi-user, multi-tasking chat environment
 * Copyright (C) 2000 Douglas R. Hanks Jr.
 *                    Stephen A. Olesen
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

struct comm_list static_commands[] =
{
	{"insmod",	safe_module_load,	0, LOWER_ADMIN,		0, NULL},
	{"rmmod",	safe_module_unload,	0, LOWER_ADMIN,		0, NULL},
	{0, 0, 0, 0, 0, 0},
};

int run_command(player * p, char *str)
{
	struct comm_list *comm;
	int found = 0, do_alias = 0;
	char *command, *params_ptr, *b;
	char params[512], temp[2];

	p->idle = 0;

	if (!*str)
		return (0);

	while (*str == ' ')
		str++;

	str = trim_end(str);
	b = strpbrk(str, ",./?\\\"'[]{}:;<>|-_=+()*&^%$#@!~`");

	if (str == b)
		do_alias = 1;

	switch (do_alias)
	{
		case 1:
			temp[0] = *str++;
			temp[1] = '\0';
			command = strdup(temp);

			while (*str == ' ')
				str++;

			strcpy(params, str);
			break;
		default:
		case 0:
			command = strtok(str, " \t\n\r");
			params_ptr = strtok(NULL, "\n\r");
			if (params_ptr == NULL)
				memset(params, '\0', sizeof(params));
			else
				strncpy(params, params_ptr, 511);
			break;
	}

	comm = (struct comm_list *) comm_hash[comm_hash_num(command)];

	while (!found && comm)
	{
		if (p->privs & comm->level)
		{
			if (!strcasecmp(command, comm->key))
			{
				char post[MAX_LINE+1];
				char pr[MAX_LINE+1];
				int wp = p->flags & WRAP_PROMPT ? 1: 0;

				found = 1;
				snprintf(post, MAX_LINE, "|int %s(struct s_player *, char *)|", comm->key);
				if(wp)
				{
					strncpy(pr, p->prompt, MAX_LINE);
					p->prompt[0] = 0;
					(comm->comm_func)(p, params);
					strncpy(p->prompt, pr, MAX_LINE);
				}
				else
				{
					(comm->comm_func)(p, params);
				}
				/* task_create(p, idle_loop_thread, post, comm->comm_func, p, tUser); */
				prompt(p, p->prompt);
			}
		}
		comm = comm->next;
	}
	if (!found)
	{
		tell_player(p, "The command '%s' was not found.\n\r", command);
		prompt(p, p->prompt);
	}
	return (0);
}
