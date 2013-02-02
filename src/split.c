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

size_t _nmemb(char const *str, char const delim)
{
	size_t nmemb = 0;
	char set = 1;

	if(!*str)
		return 0;

	/* Don't even ask */
	for(nmemb=0,set=1;str&&*str;*str++==delim?(set=1):(nmemb+=(set==1?!(set=0):(set=0))));
	return nmemb==0?1:nmemb;
}

struct s_split *split(char const *str, size_t nmemb, char const delim, size_t maxlen)
{
	struct s_split *s;
	int i = 0, element = 0;

	s = (struct s_split *) MALLOC(sizeof(struct s_split));
	s->list = (char **) MALLOC(sizeof(char *) * nmemb);
	s->nmemb = nmemb;
	s->nspli = -1;
	s->ndelm = nc(str, delim);
	s->maxlen = maxlen;

	{
		int i = 0;
		for(i=0;i<nmemb;i++[s->list]=(char *) MALLOC(maxlen));
	}

	if(nmemb == 0)
	{
		int i = 0;
		i[s->list] = (char *) MALLOC(maxlen);

		while(str && *str && i != maxlen)
			*(*(s->list)+i++) = *str++;

		s->nspli = 0;
		return s;
	}

	/* I told you */
	while(str && *str && element != nmemb)
	{
		if(*str==delim)
			for(;str&&*str&&*str==delim;str++);
		while(str&&*str&&*str!=delim&&i!=maxlen)
			*(*(s->list+element)+i++)=*str++;
		for(s->nspli++,i=(*(*(s->list+element++)+i)='\0');str&&*str&&*str!=delim;str++);
	}

	return s;
}

void delete_split(struct s_split *s)
{
	int i = 0;

	while(i != s->nmemb) FREE(i++[s->list]);
	FREE(s->list);
	FREE(s);
}
