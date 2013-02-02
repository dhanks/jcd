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

void prompt(player *p, char const *pr)
{
	if(!pr || !pr[0])
		return;

	if(p->opts & TEL_EOR)
		tell_player(p, "%s%c%c", pr, IAC, EOR);
	else
	{
		if(p->opts & TEL_GA)
			tell_player (p, "%s%c%c", pr, IAC, GA);
		else
			tell_player (p, "%s", pr);
	}

	return;
}

void tell_player (player * p, char *format,...)
{
	va_list      args;
	char ref[1024];
	int rc = 0;

	va_start (args, format);
	vsnprintf (ref, 1023, format, args);
	va_end (args);
	stream_driver(p, ref);
	rc = buffer_add(p, ref);
	p->handle.in.bytes += rc;
	p->handle.in.packets++;
	p->handle.interface->out.bytes += rc;
	p->handle.interface->out.packets++;
	return;
}

void TELL_PLAYER(player *p, char *str)
{
	int rc;
	char buffer[1024];

	strncpy(buffer, str, 1023);
	stream_driver(p, buffer);
	rc = buffer_add (p, buffer);
	p->handle.out.packets += rc;
	p->handle.out.bytes++;
	p->handle.interface->out.packets += rc;
	p->handle.interface->out.bytes++;
	return;
}

void tell_hack (player * p, char *format,...)
{
	va_list      args;
	char ref[1024];
	node *scan;

	va_start (args, format);
	vsnprintf (ref, 1023, format, args);
	va_end (args);

	for(scan = l_player->head; scan; scan = scan->next)
	{
		player *p2 = (player *) scan->data;

		if(p2->flags & LOGGED_IN && p2 != p)
			TELL_PLAYER(p2, ref);
	}
	return;
}

