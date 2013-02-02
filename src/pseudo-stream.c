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

int format_output(char *t, int ww, int flags, int prompt)
{
	int count = 0;

	ww-=2;
	while(*t)
	{
		switch(*t)
		{
			case '\033':
				t+=3;
				count+=3;
				switch(*t)
				{
					case ';':
						count-=8;
						break;
					default:
						count-=4;
						break;
				}
				break;
			case '\n':
				count = -1;
				*t = '\r';
				memmove(t+1, t, strlen(t)+1);
				*t++ = '\n';
				break;
			default:
				if((flags & WW) && ((count == ww && prompt < 1) || (count == (ww-(prompt-1)) && prompt > 0)))
				{
					if(prompt > 0)
						prompt = 0;

					count = -1;
					memmove(t+3, t, strlen(t)+3);
					*t++ = '\n';
					*t++ = '\r';
					*t = ' ';
				}
				break;
		}
		count++;
		t++;
	}
	*t++ = '\0';
	return (strlen(t));
}

int color_filter(char *buffer, int line, int flags, int prompt)
{
	return (0);
}

int pager_filter(char *buffer, int line, int flags, int prompt)
{
	return (0);
}

stream *stream_malloc(void)
{
	stream *i = (stream *) MALLOC(sizeof(stream));

	if(i == NULL)
	{
		fatal("malloc_stream:  out of memory.\n");
		return (NULL);
	}
	else
	{
		memset(i, '\0', sizeof(i));
		return (i);
	}
}

void stream_driver(player *p, char *in)
{
	node *scan;
	int ww = p->flags & WRAP_PROMPT ? strlen(p->prompt) : 0;
	int bytes = 0;

	for(scan = l_stream->head; scan; scan = scan->next)
	{
		stream *s = (stream *) scan->data;

		bytes = (s->func)(in, p->screen.width, p->flags, ww);
		s->bytes += bytes;
		p->handle.stream += bytes;
	}

	return;
}

void stream_create(char *name, int (*function)(char *, int, int, int))
{
	stream *new = stream_malloc();

	if(!new)
		return;

	new->func = function;
	new->bytes = 0;
	strcpy(new->name, name);
	LIST_ADD(l_stream, new, sizeof(stream));
}

void stream_init(void)
{
	stream_create("int format_output(char *, int, int, int)", format_output);
	stream_create("int color_filter(char *, int, int, int)", color_filter);
	stream_create("int pager_filter(char *, int, int, int)", pager_filter);
	return;
}
