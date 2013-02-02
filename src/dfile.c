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

struct s_dfile_hardload dfile_hardload[] =
{
	/* files to be loaded upon runtime */
	{"banished_msg",	"lib/files/banished",	O_RDONLY},
	{"connect_msg",		"lib/files/connect",	O_RDONLY},
	{"motd_msg",		"lib/files/motd",	O_RDONLY},
	{"request_msg",		"lib/files/request",	O_RDONLY},
	{"room_msg",		"lib/files/room",	O_RDONLY},
	{"newbie_msg",		"lib/files/newbie",	O_RDONLY},
};

DFILE *dfile_malloc(void)
{
	DFILE *df = (DFILE *) MALLOC(sizeof(DFILE));

	if(df == NULL)
	{
		fatal("malloc_interface:  out of memory.\n");
		return NULL;
	}
	else
	{
		memset(df, '\0', sizeof(df));
		return df;
	}
}

void dfile_init(DFILE *df)
{
	memset(df->name, '\0', sizeof(df->name));
	memset(df->path, '\0', sizeof(df->path));
	df->data = NULL;
	df->flags = df->size = df->accessed = df->timeout = df->reloaded = df->fd = df->loaded = df->last_accessed = 0;
	df->state = dfsVirgin;
	return;
}

int load_dfile(DFILE *df, char const *path, int const flags)
{
	int len = 0;

	if(!df)
		return (-1);

	if((df->fd = open(path, flags)) < 0)
	{
		vmlog(3, "load_dfile: couldn't load `%s' (flags = %i), reason: %s (%i)\n", path, flags, strerror(errno), errno);
		return (errno);
	}

	df->loaded = time(0);
	df->size = lseek(df->fd, 0, SEEK_END);
	lseek(df->fd, 0, SEEK_SET);

	df->data = (char *) MALLOC(df->size+1);
	memset(df->data, 0, df->size+1);
	len = read(df->fd, df->data, df->size);

	if(len < df->size)
	{
		vmlog(8, "load_dfile: error reading after %i bytes on `%s', reason: %s (%i)\n", len, path, strerror(errno),
		errno);
		close(df->fd);
		df->state = dfsError;
		return (-1);
	}

	close(df->fd);
	return (0);
}

int dfile_create(char const *name, char const *path, int const flags, int const system)
{
	DFILE *df = dfile_malloc();

	if(!df)
		return (-1);

	dfile_init(df);
	LIST_ADD(l_dfile, df, sizeof(DFILE));
	df->state = dfsVirgin;
	strncpy(df->name, name, MAX_STRING-1);
	strncpy(df->path, path, 79);

	if(load_dfile(df, path, flags) != 0)
	{
		vmlog(8, "dfile_create: bad load on `%s', aborting.\n", path);
		LIST_REMOVE(l_dfile, df);
		FREE(df);
		return (-1);
	}

	if(system)
		df->flags |= DFS_SYSTEM;

	return (0);
}

void init_dfiles(void)
{
	int i = 0;

	while(i < sizeof(dfile_hardload) / sizeof(dfile_hardload[0]))
	{
		if(dfile_create(dfile_hardload[i].name, dfile_hardload[i].path, dfile_hardload[i].flags, 1) < 0)
			vmlog(7, "init_dfiles:  error loading `%s'\n", dfile_hardload[i].path);
		else
			vmlog(0, "init_dfiles:  successfully loaded `%s'\n", dfile_hardload[i].path);
		i++;
	}

	return;
}

/* call this function to retrieve file data, so we may do proper increments */
char *dfile_data(char const *key)
{
	node *scan;

	for(scan = l_dfile->head; scan; scan = scan->next)
	{
		DFILE *df = (DFILE *) scan->data;

		if(!strcasecmp(df->name, key))
		{
			df->last_accessed = time(0);
			df->accessed++;
			df->state = dfsNormal;
			return df->data;
		}
	}

	return NULL;
}
