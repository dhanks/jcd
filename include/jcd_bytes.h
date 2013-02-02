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

#if !defined(JCD_BYTES)
	#define JCD_BYTES 1

#define ONE_BYTE		8
#define TWO_BYTES		16
#define THREE_BYTES		24
#define FOUR_BYTES		32
#define FIVE_BYTES		40
#define SIX_BYTES		48
#define SEVEN_BYTES		56
#define EIGHT_BYTES		64
#define NINE_BYTES		72
#define TEN_BYTES		80

#define MAX_LINE	TEN_BYTES
#define MAX_BUFFER	((816*TEN_BYTES)+FOUR_BYTES)
#define MAX_STRING	FOUR_BYTES

#endif
