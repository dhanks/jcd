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

#if !defined(JCD_PRIVS)
	#define JCD_PRIVS 1

#define NEWBIE			(unsigned long)1<<0
#define RESIDENT		(unsigned long)1<<1
#define MAIL			(unsigned long)1<<2
#define NEWS			(unsigned long)1<<3
#define GAMES			(unsigned long)1<<4
#define SAGE			(unsigned long)1<<5
#define BUILDER			(unsigned long)1<<6
#define PSU			(unsigned long)1<<7
#define SU			(unsigned long)1<<8
#define ASU			(unsigned long)1<<9
#define SETHBOY			(unsigned long)1<<10
#define LOWER_ADMIN		(unsigned long)1<<11
#define ADMIN			(unsigned long)1<<12
#define HCADMIN			(unsigned long)1<<13
#define CODER			(unsigned long)1<<14
#define SYSADMIN		(unsigned long)1<<15

#endif
