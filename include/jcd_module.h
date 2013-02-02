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

#if !defined(JCD_MODULE)
	#define JCD_MODULE 1

#define C_BUCKETS 197

struct comm_list
{
	char *key;
	int (*comm_func) (struct s_player *, char *);
	char *func_name;
	unsigned long level;
	int flags;
	struct comm_list *next;
};

struct mod
{
	char *name, *fn;
	void *handle;
	struct new_comm
	{
		struct comm_list lst;
		struct new_comm *next;
	} *comm;
	struct mod *next;
};

#endif
