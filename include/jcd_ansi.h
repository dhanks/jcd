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

#if !defined (JCD_ANSI)
	#define JCD_ANSI 1

#define CLEAR_CURSOR_TO_END    "\033[0K" /* Clear from cursor to end of line */
#define CLEAR_BEGIN_TO_CURSOR  "\033[1K" /* Clear from begin of line to cursor */
#define CLEAR_LINE             "\033[2K" /* Clear line containing cursor */
#define CLEAR_FROM_CURSOR      "\033[0J" /* Clear screen from cursor */
#define CLEAR_TO_CURSOR        "\033[1J" /* Clear screen to cursor */
#define CLEAR_SCREEN           "\033[2J" /* Clear entire screen */

/*
 #define CURSOR_UP(number)      printf("\033[%dA", number)
 #define CURSOR_DOWN(number)    printf("\033[%dB", number)
 #define CURSOR_FORWARD(number) printf("\033[%dC", number)
 #define CURSOR_BACKWARD(number) printf("\033[%dD", number)
 #define CURSOR_POSITION(row, column) printf("\033[%d;%dH", row, column)
*/

#define NOR           "\033[0m"
#define MOD_BOLD      "\033[1m"
#define MOD_FAINT     "\033[2m"
#define MOD_UNDERLINE "\033[4m"
#define MOD_BLINK     "\033[5m"
#define MOD_REVERSE   "\033[7m"

/* 00-09 */
#define FG_BLACK      "\033[0;30m"
#define FG_RED        "\033[0;31m"
#define FG_GREEN      "\033[0;32m"
#define FG_BROWN      "\033[0;33m"
#define FG_BLUE       "\033[0;34m"
#define FG_MAGENTA    "\033[0;35m"
#define FG_CYAN       "\033[0;36m"
#define FG_LT_GRAY    "\033[0;37m"
#define FG_DK_GRAY    "\033[1;30m"
#define FG_LT_RED     "\033[1;31m"

/* 10-15 */
#define FG_LT_GREEN   "\033[1;32m"
#define FG_YELLOW     "\033[1;33m"
#define FG_LT_BLUE    "\033[1;34m"
#define FG_LT_MAGENTA "\033[1;35m"
#define FG_LT_CYAN    "\033[1;36m"
#define FG_WHITE      "\033[1;37m"

#define BK_BLACK      "\033[0;40m"
#define BK_RED        "\033[0;41m"
#define BK_GREEN      "\033[0;42m"
#define BK_BROWN      "\033[0;43m"
#define BK_BLUE       "\033[0;44m"
#define BK_MAGENTA    "\033[0;45m"
#define BK_CYAN       "\033[0;46m"
#define BK_LT_GRAY    "\033[0;47m"

#endif
