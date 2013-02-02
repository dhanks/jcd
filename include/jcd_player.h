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

#if !defined(JCD_PLAYER)
	#define JCD_PLAYER 1

#define M_LOST			1
#define M_DISCONNECT		2

#define DISCONNECT(a) player_logout(a, __FILE__, __LINE__, errno)

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

typedef void c_func(struct s_player *, char *);

struct s_screen
{
	size_t width, height, linewrap, wordwrap;
	char type[256];
};

struct s_player
{
	int		stdin, stdout, stderr;
	int		mode;
	int		opts;
	int		ibuffer_ptr;
	int		idle;
	int		auto_term;
	unsigned int	flags;
	unsigned long	privs;
	char		ibuffer[512];
	char		*bob;
	char		**environment;
	char		password[MAX_STRING+1];
	char		name[MAX_STRING+1];
	char		lower_name[MAX_STRING+1];
	char		groundstring[MAX_LINE+1];
	char		prefix[TWO_BYTES+1];
	char		tmp[MAX_LINE+1];
	char		title[MAX_LINE+1];
	char		prompt[MAX_LINE+1];
	char		ch;
	struct s_player	*next, *prev;
	struct s_socket	handle;
	struct s_screen	screen;
	struct
	{
		char		out[BUFFER_SIZE];
		unsigned int	pos;
	} buffer;
	c_func		*func;
};

typedef struct s_player player;

#endif
