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

#if !defined(JCD_MALLOC)
	#define JCD_MALLOC 1

/*
#define MALLOC(a) malloc(a)
#define CALLOC(a,b) calloc(a,b)
#define REALLOC(a,b) realloc(a,b)
#define FREE(a) free(a)
*/

#define MALLOC(a) sMALLOC(__FILE__,__LINE__,(size_t)a)
#define CALLOC(a,b) sCALLOC(__FILE__,__LINE__,(size_t)a,(size_t)b)
#define REALLOC(a,b) sREALLOC(__FILE__,__LINE__,(void *)a,(size_t)b)
#define FREE(a) sFREE(__FILE__,__LINE__,(void *)a)

#define MT sizeof(struct mtrace)
#define SET_MT mt->size=size; \
               mt->line=line; \
               strcpy(mt->name,name);
#define M_ADD(a) { int i=0; while(mts[i]) i++; mts[i]=a; }
#define M_DEL(a) { int i=0; while(i<5000) if(mts[i++]==a) mts[i-1]=NULL; }
#define M_INCREASE(a) M->inuse+=a; if(M->inuse>M->usedmax) M->usedmax=M->inuse;
#define M_DECREASE(a) M->inuse-=a; M->freed+=a;

struct memstat
{
	unsigned long mallocs, callocs, reallocs, frees, inuse, usedmax, freed;
};

struct mtrace
{
	unsigned long size, line;
	void *what;
	char *name;
	struct mtrace *next, *prev;
};

typedef struct mtrace mtrace;
#endif
