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

#if !defined(JCD_REQUEST)
	#define JCD_REQUEST 1

#define REQ_APPROVED		(1<<0)
#define REQ_PREAPPROVED		(1<<1)
#define REQ_NAME_BAN		(1<<2)
#define REQ_IP_BAN		(1<<3)
#define REQ_NOTIFY		(1<<4)
#define REQ_ALTERNATE		(1<<5)

#define REQ_TWO_DAYS		60*60*24*2
#define REQ_WEEK		60*60*24*7
#define REQ_NEVER		-1

struct s_request
{
	char			name[MAX_STRING+1];
	char			by[MAX_STRING+1];
	char			alternate[MAX_STRING+1];
	char			password[MAX_STRING+1];
	char			ip[TWO_BYTES+1];
	char			dns[TEN_BYTES+THREE_BYTES+1];
	time_t			submitted;
	size_t			read;
	unsigned int		flags;
	unsigned int		timeout;
	unsigned char		one, two, three, four; /* IP Addr Octets */
};

typedef struct s_request request;

#endif
