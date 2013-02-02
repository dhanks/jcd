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

void list_debug(list *l)
{
	vmlog(0, "--\nList '%s' has %i nodes.  It occupies a total of %lu bytes of ram.\n", l->name, l->nodes,
	(l->memory.structure+l->memory.data));

	vmlog(0, "%lu bytes of structure and %lu bytes of data.\n", l->memory.structure, l->memory.data);

	{
		node *scan;

		for(scan = l->head; scan; scan = scan->next)
		{
			vmlog(0, "  @ %p x %lu bytes.\n", scan->data, scan->length);
		}

	}

	vmlog(0, "--\n\n");
}

node *find_node_by_data(list *l, void *data)
{
	node *scan;

	for(scan = l->head; scan; scan = scan->next)
		if(scan->data == data)
			return scan;

	return NULL;
}

list *list_init(char *name, unsigned long size)
{
	list *l = (list *) MALLOC(size);

	if(!l)
		return NULL;

	strncpy(l->name, name, 79);
	l->nodes = 0;
	l->memory.data = 0;
	l->memory.structure = size;
	l->head = NULL;

	vmlog(0, "created list '%s' (%lu bytes)\n", name, size);

	{
		list *scan;

		if(list_head == 0)
		{
			list_head = l;
			l->next = 0;
			l->prev = 0;
			return l;
		}
		else
		{
			for(scan = list_head; scan; scan = scan->next)
			{
				if(scan->next == 0)
				{
					scan->next = l;
					l->next = 0;
					l->prev = scan;
					return l;
				}
			}
		}
	}
	return NULL;
}

size_t list_deinit(list *l)
{
	list *scan = NULL;

	if(scan == list_head)
	{
		if(l->next)
			list_head = l->next;
		else
			list_head = 0;
	}
	else
	{
		for(scan = list_head; scan; scan = scan->next)
		{
			if(scan == l)
			{
				if(l->next)
					l->next->prev = l->prev;
				if(l->next && l->prev)
					l->prev->next = l->next;
				else if(l->prev)
					l->prev->next = 0;
				break;
			}
		}
	}

	{
		size_t size = sizeof(l);
		FREE(l);
		return size;
	}
	return (-1);
}

void list_add(list *l, void *data, unsigned long size, char *file, int line)
{
	node *n;

	vmlog(1, "list_add: %s %iL (%lu bytes)\n", file, line, size);
	n = (node *) MALLOC(sizeof(node));
	l->memory.structure += sizeof(node);
	l->memory.data += size;
	l->nodes++;

	n->length = size;
	n->parent = l;
	n->data = data;
	strncpy(n->file, file, 19);
	n->line = line;

	{
		node *scan;

		if(l->head == 0)
		{
			l->head = n;
			n->next = 0;
			n->prev = 0;
		}
		else
		{
			for(scan = l->head; scan; scan = scan->next)
			{
				if(scan->next == 0)
				{
					scan->next = n;
					n->next = 0;
					n->prev = scan;
					break;
				}
			}
		}
	}
}

void list_remove(list *l, void *data, char *file, int line)
{
	node *out;

	vmlog(1, "list_remove: %s %iL\n", file, line);

	out = find_node_by_data(l, data);

	if(!out)
	{
		vmlog(7, "no out in list_remove\n");
		return;
	}

	if(out == l->head)
	{
		if(out->next)
			l->head = out->next;
		else
			l->head = 0;
	}
	else
	{
		node *scan;

		for(scan = l->head; scan; scan = scan->next)
		{
			if(scan == out)
			{
				if(out->next)
					out->next->prev = out->prev;
				if(out->next && out->prev)
					out->prev->next = out->next;
				else if(out->prev)
					out->prev->next = 0;
				break;
			}
		}
	}

	l->memory.structure -= sizeof(node);
	l->memory.data -= out->length;
	l->nodes--;
	FREE(out);
}

void list_dump(player *p, list *l)
{
	tell_player(p, "--\nList '%s' has %i nodes.  It occupies a total of %lu bytes of ram.\n", l->name, l->nodes,
	(l->memory.structure+l->memory.data));

	tell_player(p, "%lu bytes of structure and %lu bytes of data.\n", l->memory.structure, l->memory.data);

	{
		node *scan;

		for(scan = l->head; scan; scan = scan->next)
		{
			tell_player(p, "  @ %p x %lu bytes.\n", scan->data, scan->length);
		}

	}

	tell_player(p, "--\n\n");
}
