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

struct s_profile profile[] =
{
	/*    16 bytes    */
	{"flags           ", kFlags,		tInt		},
	{"privs           ", kPrivs,		tInt		},
	{"password        ", kPassword,		tString		},
	{"name            ", kName,		tString		},
	{"lower_name      ", kLowername,	tString		},
	{"groundstring    ", kGroundstring,	tString		},
	{"prefix          ", kPrefix,		tString		},
	{"title           ", kTitle,		tString		},
	{"prompt          ", kPrompt,		tString		},
	{"ch              ", kCh,		tChar		},
};

/* FIXME */
char *get_line(char *src, char *dst, size_t size)
{
	while(*src)
	{
		switch(*src)
		{
			case '\0':
				return (NULL);
				break;
			case '\n':
			case '\r':
				src++;
				return src;
				break;
			default:
				*dst++ = *src++;
				if(--size <= 0)
				{
					for(; *src && src && *src != '\n' && *src != '\r' && *src != '\0'; src++);
					return (++src);
				}
				break;
		}
	}
	return NULL;
}

/* toss any unknown datatypes out and verify exactlly known types */
int player_data_check(char const *key, enum entityDataType type)
{
	switch(type)
	{
		case (tInt):
			return strcmp(key, "int");
			break;
		case (tChar):
			return strcmp(key, "char");
			break;
		case (tString):
			return strcmp(key, "string");
			break;
		default:
			return 1;
			break;
	}
}

void player_assign(player *p, char const *key, char const *type, char const *data)
{
	int i = 0;
	int found = 0;

	for(i = 0; i < sizeof(profile)/sizeof(profile[0]); i++)
	{
		if(!strcmp(profile[i].name, key))
		{
			found = 1;
			if(player_data_check((char const *) type, profile[i].type) == 0)
			{
				/* This way we cannot mix and match datatypes
				 * it must match the prototype exactlly */
				switch(profile[i].key)
				{
					case (kFlags):
						p->flags = (unsigned int) strtol(data, (char **) NULL, 10);
						break;
					case (kPrivs):
						p->privs = (unsigned long) strtol(data, (char **) NULL, 10);
						break;
					case (kPassword):
						strncpy(p->password, data, MAX_STRING-1);
						break;
					case (kName):
						strncpy(p->name, data, MAX_STRING-1);
						break;
					case (kLowername):
						strncpy(p->lower_name, data, MAX_STRING-1);
						break;
					case (kGroundstring):
						strncpy(p->groundstring, data, MAX_LINE-1);
						break;
					case (kPrefix):
						strncpy(p->prefix, data, 9);
						break;
					case (kTitle):
						strncpy(p->title, data, MAX_LINE-1);
						break;
					case (kPrompt):
						strncpy(p->prompt, data, MAX_LINE-1);
						break;
					case (kCh):
						p->ch = *data;
						break;
					default:
						vmlog(5, "player_assign: unknown identifier on player %s `%i' on `%s'\n",
						p->name, profile[i].type, profile[i].name);
				}
			}
			else
			{
				vmlog(5, "player_assign: type mismatch on player `%s', key `%s', type `%s', and data `%s'\n",
				p->name, key, type, data);
			}
		}
	}
	if(!found)
	{
		vmlog(5, "player_assign: name mismatch on player `%s', key `%s', type `%s', and data `%s'\n",
		p->name, key, type, data);
	}
}


size_t load_player(player *p, char *path)
{
	char buffer[1024], *ptr;
	char line[MAX_LINE+1];
	int fd = 0, size = 0, len = 0;
	struct s_split *s;

	if((fd = open(path, O_RDONLY)) < 0)
		return (-1);

	size = lseek (fd, 0, SEEK_END);
	lseek (fd, 0, SEEK_SET);
	memset(buffer, '\0', sizeof(buffer));
	ptr = buffer;

	if(size >= sizeof(buffer))
	{
		vmlog(4, "load_player: size exceeds length of buffer (%i/%i) `%s'\n", size, sizeof(buffer), path);
		size = sizeof(buffer)-1;
	}

	len = read(fd, buffer, size);

	if(len < size)
	{
		vmlog(4, "load_player: read error (%s:%i) after %i bytes on file %s\n", strerror(errno), errno, len, path);
		return (-1);
	}

	while(ptr && *ptr)
	{
		memset(line, '\0', sizeof(line));
		ptr = get_line(ptr, line, MAX_LINE);
		s = split(line, _nmemb(line, '\007'), '\007', MAX_LINE);

		if(s->nmemb == 3)
			player_assign(p, (char const *) *(s->list+0), (char const *) *(s->list+1), (char const *) *(s->list+2));

		delete_split(s);
	}
	return (0);
}

void write_int(size_t *rc, FILE *fp, char const *key, int const i)
{
	*rc += fprintf(fp, "%-16s\007int\007%i\n", key, i);
	return;
}

void write_char(size_t *rc, FILE *fp, char const *key, char const c)
{
	*rc += (size_t) fprintf(fp, "%-16s\007char\007%c\n", key, c);
	return;
}

void write_string(size_t *rc, FILE *fp, char const *key, char const *str)
{
	*rc += (size_t) fprintf(fp, "%-16s\007string\007%s\n", key, str);
	return;
}

size_t save_player(player *p, char *path)
{
	FILE *fp = fopen(path, "w");
	size_t rc = 0;

	if(!fp)
	{
		vmlog(6, "save_player:  cannot write to '%s', reason being: %s (%i)\n", path, strerror(errno), errno);
		return (-1);
	}

	write_int(&rc, fp, "flags", p->flags);
	write_int(&rc, fp, "privs", p->privs);

	write_string(&rc, fp, "name", p->name);
	write_string(&rc, fp, "password", p->password);
	write_string(&rc, fp, "groundstring", p->groundstring);
	write_string(&rc, fp, "prefix", p->prefix);
	write_string(&rc, fp, "title", p->title);
	write_string(&rc, fp, "prompt", p->prompt);

	write_char(&rc, fp, "ch", p->ch);

	fclose(fp);
	return rc;
}

size_t load_entity(void *entity, char *path, enum entityType type)
{
	union entityData data;

	switch(type)
	{
		case (ePlayer):
			data.p = (player *) entity;
			return load_player(data.p, path);
			break;
		case (eUnknown):
		case (eInterface):
		case (eSTREAM):
		case (eTask):
		case (eFile):
		case (eObject):
		default:
			return (-1);
			break;
	}
}
