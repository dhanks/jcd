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

size_t buffer_add(player *p, char const *str)
{
	size_t size = strlen(str); /* save CPU cycles */

	if(size+1 >= BUFFER_SIZE)
	{
		/* this is all we can really do since we're dealing with a
		 * static buffer */
		vmlog(7, "buffer_add:  str exceeds buffer size, premature return.\n");
		return (-1);
	}

	if(p->buffer.pos+size+1 >= BUFFER_SIZE)
	{
		/* Let's go ahead and flush the buffer if the new data
		 * fit comfortably so we may add it below */
		vmlog(1, "buffer_add:  str and current position exceeds buffer size, premature flush.\n");
		buffer_send(p);
	}

	memcpy(p->buffer.out+p->buffer.pos, str, size+1);
	p->buffer.pos += size;

	return size;
}

void buffer_send(player *p)
{
	if(!p->buffer.pos)
		return;

	sock_write(p->handle.fd, (void *) p->buffer.out, p->buffer.pos);

	p->buffer.pos = 0;
	return;
}
