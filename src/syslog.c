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

ssize_t write_malloc_log(char *str)
{
	FILE *fp;
	char open_string[256];
	ssize_t ret = 0;

	snprintf (open_string, 255, "%s/var/log/malloc", SERVER_ROOT);
	fp = fopen (open_string, "a");

	if (!fp)
	{
		fprintf(stderr, "Can't open file to log: %s\n", open_string);
		fflush(stderr);
		return (-1);
	}

	ret += fprintf (fp, str);
	fclose (fp);
	return (ret);
}

ssize_t write_log(int level, char *str)
{
	FILE *fp;
	struct tm *t;
	char the_time[MAX_LINE+1], open_string[MAX_LINE+1];
	static char buffer[256];
	time_t current_time;
	ssize_t ret = 0;

	if(!strcasecmp(buffer, str))
	{
		repeated++;
		return 0;
	}

	strncpy(buffer, str, 255);
	current_time = time (0);
	t = localtime (&current_time);
	strftime ((char *) &the_time, MAX_LINE, "%H:%M:%S - %m/%d/%Y - ", t);

	snprintf (open_string, MAX_LINE, "%s/var/log/messages", SERVER_ROOT);
	fp = fopen (open_string, "a");

	if (!fp)
	{
		fprintf(stderr, "Can't open file to log: %s\n", open_string);
		fflush(stderr);
		return (-1);
	}

	if(repeated)
	{
		ret += fprintf(fp, "<%d> %s", level, the_time);
		ret += fprintf(fp, "Last message repeated %i times.\n", repeated);
		repeated = 0;
	}

	ret += fprintf (fp, "<%d> %s", level, the_time);
	ret += fprintf (fp, str);
	fclose (fp);
	return (ret);
}

ssize_t vmlog(int level, char *format, ...)
{
	va_list args;
	char ref[256];

	va_start (args, format);
	vsnprintf (ref, 255, format, args);
	va_end (args);
	write_log(level, ref);
	return (strlen(ref));
}

ssize_t malloc_log(char *format, ...)
{
	va_list args;
	char ref[256];

	va_start (args, format);
	vsnprintf (ref, 255, format, args);
	va_end (args);
	write_malloc_log(ref);
	return (strlen(ref));
}
