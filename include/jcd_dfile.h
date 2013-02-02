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

#if !defined(JCD_FILE)
	#define JCD_FILE 1

#define DFS_SYSTEM		(1<<0)

enum dfileState
{
	dfsUnknown,
	dfsOpen,
	dfsClosed,
	dfsVirgin,
	dfsNormal,
	dfsError
};

struct s_dfile
{
	char		name[MAX_STRING+1];
	char		path[80];
	char		*data;
	unsigned long	size;
	unsigned int	accessed;
	unsigned int	reloaded;
	unsigned int	timeout;
	unsigned int	flags;
	int		fd;
	enum dfileState	state;
	time_t		loaded;
	time_t		last_accessed;
};

struct s_dfile_hardload
{
	char		name[MAX_LINE+1];
	char		path[80];
	int		flags;
};

typedef struct s_dfile DFILE;

#endif
