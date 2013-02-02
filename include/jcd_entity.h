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

#if !defined(JCD_ENTITY)
	#define JCD_ENTITY 1

enum entityType
{
	eUnknown,
	ePlayer,
	eInterface,
	eSTREAM,
	eTask,
	eFile,
	eObject
};

enum entityKey
{
	kUnknown,
	kFlags,
	kPrivs,
	kPassword,
	kName,
	kLowername,
	kGroundstring,
	kPrefix,
	kTitle,
	kPrompt,
	kCh
};

enum entityDataType
{
	dtUnknown,
	tInt,
	tChar,
	tString
};

union entityData
{
	struct s_player *p;
	struct s_interface *i;
	struct s_stream *s;
	struct s_task *t;
};

struct s_profile
{
	char			name[TWO_BYTES+1];
	enum entityKey 		key;
	enum entityDataType	type;
};

#endif
