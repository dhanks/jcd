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

#if !defined(JCD_STRUCTS)
	#define JCD_STRUCTS 1

struct s_split
{
	size_t		nmemb;
	size_t		nspli;
	size_t		ndelm;
	size_t		maxlen;
	char		**list;
};

typedef struct
{
	char		*key;
	int		flag;
	int		bold;
	char		tag;
	unsigned int	level;
	char		*color;
} tag_list;

enum ceDATA_Type
{
	cePlayer,
	ceBot
};

enum commEngineType
{
	ceTELL,
	ceREMOTE,
	ceRTHINK,
	ceRECHO,
	ceSHOUT,
	ceWHISPER,
	ceSAY,
	ceEMOTE
};

union ceDATA
{
	player *p;
	player **plist;
};

#endif
