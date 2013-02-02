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

/* These functions are bit redundant, but I couldn't figure out how to
 * call a dynamic function from a shared object explicitly */

void static_look(player *p)
{
	node *scan;

	tell_player(p, dfile_data("room_msg"));
	tell_player(p, "\n");

	for(scan = l_player->head; scan; scan = scan->next)
	{
		player *p2 = (player *) scan->data;

		if(p2 != p && p2->flags & LOGGED_IN)
			tell_player(p, "%s %s\n", p2->name, p2->groundstring);
	}

	return;
}
