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


#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define READSOCK(fdname,charname) \
	if ( *size > 0 ) \
	{ \
		if ( read(fdname, &charname, 1) < 1 ) \
		{ \
			vmlog(2, "READSOCK: %s (%d)\n", strerror(errno), errno); \
			return; \
		} \
		p->handle.interface->in.bytes++; \
		p->handle.out.bytes++; \
	}


#define S_FD_SET(a) FD_SET(a,&fds); if(a>largest) largest=a
#define S_FD_ISSET(a) FD_ISSET(a,&fds)
#define S_FD_ZERO(a) FD_ZERO(a); largest=-1

#define swBLOCKSIZE  1024

#define INTERFACE_UP		(1<<0)
#define INTERFACE_DOWN		(1<<1)
#define INTERFACE_STALE		(1<<2)

#define TEL_EOR			(1<<0)
#define TEL_ECHO		(1<<1)
#define TEL_GA			(1<<2)
#define PREV_N			(1<<3)
#define PREV_R			(1<<4)
#define TEL_TTYPE		(1<<5)
#define TEL_NAWS		(1<<6)
#define TEL_PROMPT		(1<<7)

#define LOGGED_IN		(1<<0)
#define WRAP_PROMPT		(1<<1)
#define WW			(1<<2)
#define CWW			(1<<3)
#define AUTO_TERM		(1<<4)
#define TAGGED			(1<<5)
#define MULTI			(1<<6)

#define INTERFACE_JCD		1
#define INTERFACE_WWW		2
#define INTERFACE_INTERCOM	3

enum lastSockState_e
{
	lssOKAY,
	lssERROR
} lastSockState;


struct s_packets
{
	unsigned long		bytes, errors, dropped, packets;
};

struct s_socket
{
	unsigned int		flags;
	unsigned int		port;
	unsigned long		stream;
	int			fd;
	char			ip[TWO_BYTES+1];
	char			dns[TEN_BYTES+THREE_BYTES+1];
	unsigned char		one, two, three, four;
	struct sockaddr_in	handle;
	struct s_packets	in, out;
	struct s_interface	*interface;
	struct s_socket		*next, *prev;
	enum e_state
	{
		sNORMAL,
		sIAC,
		sDO,
		sDONT,
		sWILL,
		sWONT,
		sSB,
		sABORT,
		sWAIT
	} state;
};

struct s_interface
{
	int			connections;
	int			(*func) (void);
	unsigned int		flags;
	unsigned char		device, id;
	char			name[TWO_BYTES+1];
	struct s_socket		handle;
	struct s_interface	*next, *prev;
	struct s_packets	in, out;
};

struct s_stream
{
	char			name[MAX_LINE+1];
	unsigned long		bytes;
	int			(*func)(char *, int, int, int);
	struct s_stream		*next, *prev;
};

typedef struct s_packets packet;
typedef struct s_socket sock;
typedef struct s_interface interface;
typedef struct s_stream stream;
