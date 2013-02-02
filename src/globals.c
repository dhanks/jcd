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

char **ps;

char *reserved_names[] = {"config", "newbie", "room", "su", "jesus", "god", "system", "root", 0};
char *admin_list[] = {"admin", 0};

int pc, system_flags, timer = 0;
int inuse = 0;
unsigned int uptime = 0;
unsigned int mark = 5;
unsigned int repeated = 0;

size_t Chunks = 0;

player *player_head;
player *player_current;

interface *interface_head;

stream *stream_top;

list *list_head, *l_dfile, *l_player, *l_interface, *l_request, *l_stream;
