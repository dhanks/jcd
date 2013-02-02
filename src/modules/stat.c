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
	return ("stat.so");
}

void reg(struct mod *m)
{
	mod_comm_register(m, "commands",	"commands",		NEWBIE, 	0);
	mod_comm_register(m, "ifconfig",	"ifconfig",		SAGE,		0);
	mod_comm_register(m, "ldump",		"ldump",		NEWBIE, 	0);
	mod_comm_register(m, "privs",		"show_privs",		NEWBIE, 	0);
	mod_comm_register(m, "streams",		"show_streams",		SAGE,		0);
	mod_comm_register(m, "save",		"comm_save",		NEWBIE,		0);
	mod_comm_register(m, "term",		"term_set",		NEWBIE, 	0);
	mod_comm_register(m, "terminal",	"term_set",		NEWBIE, 	0);
	mod_comm_register(m, "time",		"jcd_time",		NEWBIE, 	0);
	mod_comm_register(m, "version",		"version",		NEWBIE, 	0);
	return;
}

char *ifconfig_dev(interface *in)
{
	static char buffer[20], *ptr;

	ptr = buffer;

	switch(in->device)
	{
		case INTERFACE_INTERCOM:
			snprintf(ptr, 19, "intercom%i", in->id);
			ptr = strchr(ptr, 0);
			*ptr++ = 0;
			return (buffer);
			break;
		case INTERFACE_WWW:
			snprintf(ptr, 19, "www%i", in->id);
			ptr = strchr(ptr, 0);
			*ptr++ = 0;
			return (buffer);
			break;
		case INTERFACE_JCD:
			snprintf(ptr, 19, "jcd%i", in->id);
			ptr = strchr(ptr, 0);
			*ptr++ = 0;
			return (buffer);
			break;
		default:
			return "error";
	}
}

static int dynamic_command_sort(const void *first, const void *second)
{
	const struct new_comm **one = (const struct new_comm **) first;
	const struct new_comm **two = (const struct new_comm **) second;

	return(strcmp((*one)->lst.key, (*two)->lst.key));
}

static int static_command_sort(const void *first, const void *second)
{
	const struct comm_list **one = (const struct comm_list **) first;
	const struct comm_list **two = (const struct comm_list **) second;

	return(strcmp((*one)->key, (*two)->key));
}

int comm_save(player *p, char *str)
{
	char buffer[80];
	size_t len = 0;

	snprintf(buffer, 79, "%s/lib/players/%s", SERVER_ROOT, p->lower_name);
	len = save_player(p, buffer);
	if(len)
	{
		tell_player(p, "Wrote %i bytes to disk.\n", len);
		return 0;
	}
	else
	{
		tell_player(p, "There was a problem writing your record to disk (%i bytes).\n", len);
		return -1;
	}
}

int show_privs(player *p, char *str)
{
	center_banner(p, '/', p->ch, '\\', "Privileges");
	if(p->privs & RESIDENT)
		tell_player(p, "You are a resident.\n");
	if(p->privs & NEWS)
		tell_player(p, "You may use news.\n");
	if(p->privs & MAIL)
		tell_player(p, "You may use mail.\n");
	if(p->privs & GAMES)
		tell_player(p, "You may use games.\n");
	if(p->privs & SAGE)
		tell_player(p, "You are a Sage.\n");
	if(p->privs & BUILDER)
		tell_player(p, "You are a builder.\n");
	if(p->privs & PSU)
		tell_player(p, "You are a Super User in training.\n");
	if(p->privs & SU)
		tell_player(p, "You are a Super User.\n");
	if(p->privs & ASU)
		tell_player(p, "You are a Senior Super User.\n");
	if(p->privs & SETHBOY)
		tell_player(p, "You are an Administrator.\n");
	if(p->privs & LOWER_ADMIN)
		tell_player(p, "You are a Pseudo-Administrator.\n");
	if(p->privs & ADMIN)
		tell_player(p, "You are in Administrator.\n");
	if(p->privs & HCADMIN)
		tell_player(p, "You are hard-coded into this program.\n");
	if(p->privs & CODER)
		tell_player(p, "You are a Coder.\n");
	if(p->privs & SYSADMIN)
		tell_player(p, "You are a Systems Administrator.\n");
	tell_player(p, "Your privilege code is %lu\n", p->privs);
	bottom_banner(p, p->ch);
	return (0);
}

int ldump(player *p, char *str)
{
	unsigned long total_data = 0;
	unsigned long total_structure = 0;
	unsigned int nodes = 0;
	unsigned int lists = 0;
	list *lscan;
	node *nscan;

	center_banner(p, '/', p->ch, '\\', "abstract list dump");
	for(lscan = list_head; lscan; lscan = lscan->next)
	{
		tell_player(p, " List Head `%s' contains %i nodes using %lu bytes of RAM, %lu bytes for structural and %lu bytes for data.\n", lscan->name, lscan->nodes, lscan->memory.structure+lscan->memory.data, lscan->memory.structure, lscan->memory.data);
		tell_player(p, "              FILE  LINE   HEX ADDR    SIZE\n");
		total_structure += lscan->memory.structure;
		total_data += lscan->memory.data;
		nodes += lscan->nodes;
		lists++;
		for(nscan = lscan->head; nscan; nscan = nscan->next)
			tell_player(p, "   %15s %4iL @ %p x %lu bytes\n", nscan->file, nscan->line, nscan, nscan->length);
		if(lscan->next)
			middle_banner(p, p->ch);
	}
	center_banner(p, '|', p->ch, '|', "summary of data");
	tell_player(p, " %lu bytes of RAM used to store %i list heads which contain %i nodes, %lu bytes reserved for structure and %lu bytes reserved for data.\n", total_data+total_structure, lists, nodes, total_structure, total_data);
	bottom_banner(p, p->ch);
	return 0;
}

int term_set(player *p, char *str)
{
	char *a, *b;

	if(!*str)
	{
		tell_player(p, " Usage : terminal < auto-set|info|toggle [echo|eor|ga|wrap_prompt]|set [ww<#columns>] >\n");
		return (-1);
	}

	a = next_space(str);
	*a++ = 0;

	str = trim_end(str);

	if(!strcasecmp(str, "auto-set"))
	{
		tell_player(p, "%c%c%c", IAC, DO, TELOPT_NAWS);
		tell_player(p, "%c%c%c", IAC, DO, TELOPT_ECHO);
		tell_player(p, "%c%c%c", IAC, DO, TELOPT_SGA);
		tell_player(p, "%c%c%c", IAC, DO, TELOPT_EOR);
		tell_player(p, "%c%c%c", IAC, DO, TELOPT_TTYPE);
		tell_player(p, "%c%c%c%c%c%c", IAC, SB, TELOPT_TTYPE, 1, IAC, SE);
		tell_player(p, "Attempting to auto-detect terminal attributes .. will time-out process after ten seconds.\n");
		p->flags |= AUTO_TERM;
		p->auto_term = 1;
		return (-1);
	}
	else if(!strcasecmp(str, "info"))
	{
		center_banner(p, '/', p->ch, '\\', "terminal properties");
		tell_player(p, " Terminal echo: %s\n", p->opts & TEL_ECHO ? "on" : "off");

		if(p->flags & CWW)
			tell_player(p, " Terminal size: %lux%lu (custom-set)\n", p->screen.width, p->screen.height);
		else
			tell_player(p, " Terminal size: %lux%lu (%s)\n", p->screen.width, p->screen.height,
			p->opts & TEL_NAWS ? "auto-detected" : "default");

		tell_player(p, " Terminal type: %s (%s)\n", p->opts & TEL_TTYPE ? p->screen.type : "unknown",
		p->opts & TEL_TTYPE ? "auto-detected" : "default");

		tell_player(p, " Terminap opts: %s%s%s%s%s%s\n", p->opts & TEL_GA ? "(TEL_GA) " : "",
		p->opts & TEL_EOR ? "(TEL_EOR) " : "", p->flags & WRAP_PROMPT ? "(WRAP_PROMPT) " : "(NO_WRAP_PROMPT) ",
		p->flags & WW ? "(WORD_WRAP) " : "(NO_WORD_WRAP) ", p->flags & CWW ? "(CUSTOM_WORD_WRAP) " : "",
		p->flags & AUTO_TERM ? "(AUTO_DETECTING)" : "");

		center_banner(p, '|', p->ch, '|', "network traffic");

		tell_player(p, " Bytes/packets\n");
		tell_player(p, "      received: %lu/%lu\n", p->handle.in.bytes, p->handle.in.packets);
		tell_player(p, "          sent: %lu/%lu\n", p->handle.out.bytes, p->handle.out.packets);

		center_banner(p, '|', p->ch, '|', "socket attributes");

		tell_player(p, "     Interface: Bound to interface %s on port %i\n",
		ifconfig_dev(p->handle.interface), p->handle.interface->handle.port);

		tell_player(p, " STREAM device: %lu bytes filtered\n", p->handle.stream);

		bottom_banner(p, p->ch);
		return (0);
	}
	else if(!strcasecmp(str, "toggle"))
	{
		if(!strcasecmp(a, "echo"))
		{
			p->opts ^= TEL_ECHO;
			tell_player(p, "Local echo %s\n", p->opts & TEL_ECHO ? "on" : "off");
			return (0);
		}
		else if(!strcasecmp(a, "ga"))
		{
			p->opts ^= TEL_GA;
			tell_player(p, "IAC GA prompts %s\n", p->opts & TEL_GA ? "on" : "off");
			return (0);
		}
		else if(!strcasecmp(a, "eor"))
		{
			p->opts ^= TEL_EOR;
			tell_player(p, "IAC EOR prompts %s\n", p->opts & TEL_EOR ? "on" : "off");
			return (0);
		}
		else if(!strcasecmp(a, "wrap_prompt"))
		{
			p->flags ^= WRAP_PROMPT;
			tell_player(p, "Word wrap will %scalculate the length of your prompt\n",
			p->flags & WRAP_PROMPT ? "" : "not ");
			return (0);
		}
		else
		{
			tell_player(p, "Toggle not found.\n");
			tell_player(p, " Usage : terminal < auto-set|info|toggle [echo|eor|ga|wrap_prompt]| set[ww<#columns>] >\n");
			return (0);
		}
	}
	else if(!strcasecmp(str, "set"))
	{
		b = next_space(a);
		*b++ = 0;

		if(!strcasecmp(a, "ww"))
		{
			size_t wrap, x, y;

			if(!strcasecmp(b, "off"))
			{
				p->flags &= ~WW;
				p->flags &= ~CWW;
				tell_player(p, "Turned off word wrap.\n");
				return (0);
			}

			wrap = (size_t) strtol((const char *) b, (char **) NULL, 10);
			x = p->screen.width;
			y = p->screen.height;

			if(wrap <= 9 || wrap >= 256)
			{
				tell_player(p, "You may only set your word wrap between 10 through 255\n");
				return (0);
			}

			p->flags |= WW;
			p->flags |= CWW;
			p->screen.width = wrap;

			if(p->opts & TEL_NAWS)
			{
				tell_player(p, "Your terminal size was already detected (%lux%lu), but will be over-written.\n",
				x, y);
				p->opts &= ~TEL_NAWS;
			}

			tell_player(p, "Word wrap is now turned on and set to %lu.\n", p->screen.width);
			return (0);
		}
		else
		{
			tell_player(p, "Set value not found.\n");
			tell_player(p, " Usage : terminal < auto-set|info|toggle [echo|eor|ga|wrap_prompt]|set [ww<#columns>] >\n");
			return (0);
		}
	}
	else
	{
		tell_player(p, "Invalid option.\n");
		tell_player(p, " Usage : terminal < auto-set|info|toggle [echo|eor|ga|wrap_prompt]|set [ww<#columns>] >\n");
		return (0);
	}
	return (0);
}

int jcd_time(player *p, char *str)
{
	time_t       t = time (0);
	char string[50], string2[50];

	strftime (string, 49, "%A %d %B %Y", localtime (&t));
	strftime (string2, 49, "%I:%M:%S", localtime (&t));

	center_banner(p, '/', p->ch, '\\', "jcd time");
	tell_player(p, " Today is %s\n", string);
	tell_player(p, " jcd time is %s\n", string2);
	tell_player(p, " We have been up for %s\n", word_time(uptime));
	bottom_banner(p, p->ch);
	return (0);
}

int version(player *p, char *str)
{
	center_banner(p, '/', p->ch, '\\', "jcd version");
	tell_player(p, " %sjcd version %s is running on a %s %s\n %s %s %s(%s%s%s)\n Copyright (C) 2000 %srohan%s\n",
	FG_WHITE, JCD_VERSION, UNAME_MACHINE, UNAME_PROCESSOR, UNAME_OS, UNAME_RELEASE, NOR, FG_WHITE, UNAME_NODE, NOR,
	MOD_BOLD, NOR);
	bottom_banner(p, p->ch);
	return (0);
}

int commands(player *p, char *str)
{
	struct mod *mc;
	struct comm_list *c;
	int total = 0;
	char buffer[2048], *ptr;

	ptr = buffer;

	center_banner(p, '/', p->ch, '\\', "Commands available to you");


	/* Static commands here */
	{
		/* FIXME */
		struct comm_list **list = (struct comm_list **) MALLOC(sizeof(struct comm_list **) * 100);
		int i = 0;

		for(i = 0, c = (struct comm_list *) static_commands; c->key && p->privs & c->level; list[i++] = c++);

		if (i == 0)
			sprintf(ptr, " %sThere are currently no static commands loaded.%s\n", MOD_BOLD, NOR);
		else if (i == 1)
			sprintf(ptr, " %sThere is only one static command available.%s\n", MOD_BOLD, NOR);
		else
			sprintf(ptr, " %sThere are %i static commands available to you.%s\n", MOD_BOLD, i, NOR);

		ptr = strchr(ptr, 0);
		total += i;

		qsort(list, i, sizeof(struct comm_list *), static_command_sort);

		{
			int j = 0;

			*ptr++ = ' ';

			for(j = 0; j < i; j++)
			{
				sprintf(ptr, "%s ", list[j]->key);
				ptr = strchr(ptr, 0);
			}
		}

		FREE(list);
	}

	ptr--;
	*ptr++ = '\n';
	*ptr++ = '\n';

	/* Dynamic commands here */
	{
		/* FIXME */
		struct new_comm **list = (struct new_comm **) MALLOC(sizeof(struct new_comm **) * 300);
		int i = 0;

		if (mod_list)
		{
			struct new_comm *nc;

			mc = mod_list;

			while (mc)
			{
				nc = mc->comm;

				while (nc)
				{
					if(p->privs & nc->lst.level)
					{
						list[i] = nc;
						i++;
					}
					nc = nc->next;
				}
				mc = mc->next;
			}

			if (i == 0)
				sprintf(ptr, " %sThere are currently no dynamic commands loaded.%s\n", MOD_BOLD, NOR);
			else if(i == 1)
				sprintf(ptr, " %sThere is only one dynamic command available.%s\n", MOD_BOLD, NOR);
			else
				sprintf(ptr, " %sThere are %i dynamic commands available to you.%s\n", MOD_BOLD, i, NOR);

			ptr = strchr(ptr, 0);
			total += i;
		}

		qsort(list, i, sizeof(struct new_comm *), dynamic_command_sort);

		{
			int j = 0;

			*ptr++ = ' ';

			for(j = 0; j < i; j++)
			{
				sprintf(ptr, "%s ", list[j]->lst.key);
				ptr = strchr(ptr, 0);
			}
		}

		ptr--;
		*ptr++ = '\n';

		FREE(list);
	}

	tell_player(p, buffer);
	{
		char temp[80];

		sprintf(temp, " %i commands total.", total);
		center_banner(p, '\\', p->ch, '/', temp);
	}

	return (0);
}

int show_streams(player *p, char *str)
{
	node *scan;
	int i = 0;

	center_banner(p, '/', p->ch, '\\', "socket stream filters");
	tell_player(p, " DEVICE STREAM_FUNCTION                            BYTES_PROCESSED\n");
	for(i = 0, scan = l_stream->head; scan; i++, scan = scan->next)
	{
		stream *s = (stream *) scan->data;

		tell_player(p, " %6i %-42s %lu\n", i, s->name, s->bytes);
	}
	bottom_banner(p, p->ch);
	return (0);
}

char *ifconfig_state(interface *in)
{
	if(in->flags & INTERFACE_UP)
		return "UP";
	else if (in->flags & INTERFACE_DOWN)
		return "DOWN";
	else if (in->flags & INTERFACE_STALE)
		return "STALE";
	else
		return "ERROR";
}


int ifconfig(player *p, char *str)
{
	node *scan;

	if(!*str)
	{
		center_banner(p, '/', p->ch, '\\', "dynamic interface configuration");
		tell_player(p, " Device      Bound    State     RX        TX        Connections\n");
		for(scan = l_interface->head; scan; scan = scan->next)
		{
			interface *i = (interface *) scan->data;

			tell_player(p, " %-11s %-8i %-9s %-9i %-9i %-8i\n", ifconfig_dev(i),
			i->handle.port, ifconfig_state(i), i->in.bytes, i->out.bytes, i->connections);
		}
		bottom_banner(p, p->ch);
		return (0);
	}

	{
		char *com, *dev;
		int N, P;

		sscanf(str, "%s %s %d %d", com, dev, &N, &P);
		tell_player(p, "%s %s %d %d\n", com, dev, N, P);
	}
	return 0;
}
