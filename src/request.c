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

request *request_malloc(void)
{
	request *r = (request *) MALLOC(sizeof(request));

	if(r == NULL)
	{
		perror("request_malloc: out of memory: ");
		exit(255);
	}
	else
	{
		memset(r, '\0', sizeof(r));
		return r;
	}
}

void request_init(request *r)
{
	memset(r->name, '\0', sizeof(r->name));
	memset(r->alternate, '\0', sizeof(r->alternate));
	memset(r->password, '\0', sizeof(r->password));
	memset(r->ip, '\0', sizeof(r->ip));
	memset(r->dns, '\0', sizeof(r->dns));

	r->submitted = r->read = r->flags = r->timeout = 0;
	r->one = r->two = r->three = r->four = 0;
	return;
}
