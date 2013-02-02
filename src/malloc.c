/* Jade Coiled Dragon - a multi-user, multi-tasking chat environment
 * Copyright (C) 2000 Douglas R. Hanks Jr.
 *                    Stephen A. Olesen
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

void MALLOC_INIT(void)
{
#ifdef MALLOC_DEBUG
	vmlog(1, "starting MALLOC wrapper\n");
#endif
}

void *sMALLOC(char *name,unsigned long line,size_t size)
{
	void *malloc_ptr;

	malloc_ptr=malloc(size);
#ifdef MALLOC_DEBUG
	malloc_log("limalloc:%p:%d:%s:%d\n", (unsigned) malloc_ptr, size, name, line);
#endif
	return(malloc_ptr);
}

void *sCALLOC(char *name,unsigned long line,size_t size1,size_t size2)
{
	void *malloc_ptr;
	unsigned long size=size1*size2;

	malloc_ptr=malloc(size);
#ifdef MALLOC_DEBUG
	malloc_log("licalloc:%p:%d:%s:%d\n", (unsigned) malloc_ptr, size1 * size2, name, line);
#endif
	return(malloc_ptr);
}

void *sREALLOC(char *name,unsigned long line,void *ptr,size_t size)
{
	void *malloc_ptr;

	malloc_ptr=realloc(ptr,size);
#ifdef MALLOC_DEBUG
	malloc_log("lirealloc:%p:%d:%s:%d\n", (unsigned) ptr, 0, name, line);
	malloc_log("lirealloc:%p:%d:%s:%d\n", (unsigned) malloc_ptr, size, name, line);
#endif
	return(malloc_ptr);
}

void *sFREE(char *name,unsigned long line,void *ptr)
{
#ifdef MALLOC_DEBUG
	malloc_log("lifree:%p:0:%s:%d\n", (unsigned) ptr, name, line);
#endif
	free(ptr);
	return (NULL);
}
