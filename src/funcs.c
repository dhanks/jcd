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

char *tens_words[] = {"", "ten", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
char *units_words[] = {"none", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
char *teens[] = {"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};

size_t nc(char const *str, char const c)
{
	char *walk = (char *) str;
	size_t i = 0;

	while(walk && *walk) *walk++==c?(i++):0;

	return i;
}

char *time2date(time_t submit)
{
	struct tm *t;
	static char buffer[MAX_LINE+1];

	t = localtime(&submit);
	strftime ((char *) &buffer, MAX_LINE, "%H:%M:%S - %m/%d/%Y", t);

	return buffer;
}

char *next_space(char *str)
{
	for (; str && *str && *str != ' ' && *str != '\t'; str++);

	if (*str == ' ' || *str == '\t')
	{
		for (; str && *str && (*str == ' ' || *str == '\t'); str++);
		str--;
	}

	return str;
}

char *end_string (char *str)
{
	str = strchr (str, 0);
	return ++str;
}

char *trim_end(char *str)
{
	char *trail = strchr(str, 0);

	if (*(--trail))
		for (; *trail == ' ' || *trail == '\t'; *trail-- = '\0');
			return str;
}

void lower_case(char *str)
{
	while (*str)
		*str++ = tolower(*str);
}

void center_banner(player *p, char const head, char const ch, char const tail, char const *str)
{
	int width = p->screen.width - 3;
	int first = (width - (strlen(str) + 2)) / 2;
	int second = (width - (strlen(str) + 2)) / 2;
	char right[MAX_LINE+1];
	char left[MAX_LINE+1];
	char *r = right;
	char *l = left;

	if(strlen(str) > (p->screen.width - 5))
	{
		tell_player(p, "%s%s%s\n", MOD_BOLD, str, NOR);
		return;
	}

	if((first * 2) != (width - (strlen(str) + 2)))
		second++;

	while(first-- != 0)
		*l++ = ch;
	*l = '\0';

	while(second-- != 0)
		*r++ = ch;
	*r = '\0';

	tell_player(p, "%s%c%s %s%s%s%s %s%c%s\n", FG_LT_BLUE, head, left, NOR, MOD_BOLD, str, FG_LT_BLUE, right, tail, NOR);
	return;
}

void middle_banner(player *p, char const ch)
{
	char middle[256];
	char *mid = middle;
	int spaces = p->screen.width - 3;

	while(spaces-- != 0)
		*mid++ = ch;

	*mid = '\0';
	tell_player(p, "%s|%s|%s\n", FG_LT_BLUE, middle, NOR);
	return;
}

void bottom_banner(player *p, char const ch)
{
	char middle[256];
	char *mid = middle;
	int spaces = p->screen.width - 3;

	while(spaces-- != 0)
		*mid++ = ch;

	*mid = '\0';
	tell_player(p, "%s\\%s/%s\n", FG_LT_BLUE, middle, NOR);
	return;
}

void top_banner(player *p, char const ch)
{
	char middle[256];
	char *mid = middle;
	int spaces = p->screen.width - 3;

	while(spaces-- != 0)
		*mid++ = ch;

	*mid = '\0';
	tell_player(p, "%s/%s\\%s\n", FG_LT_BLUE, middle, NOR);
	return;
}

char *bad_name(char const *name)
{
	int i;
	char *c;

	for (i = 0; reserved_names[i]; i++)
		if (!strcasecmp (name, reserved_names[i]))
			return "That is a reserved name. Please choose another.\n";

	if (strlen (name) > 20)
		return "That's too long. Try something shorter.\n";

	if (strlen (name) < 2)
		return "Names must be at least two letters long.\n";

	for (c = (char *) name; *c; c++)
		if (!isalpha (*c))
			return "Names may only contain alphabetic characters (ie. letters)\n";

	return 0;
}

char *word_time(int t)
{
	static char time_string[MAX_LINE+1], *fill;
	int days, hrs, mins, secs;

	if (!t)
		return "no time at all";

	days = t / 86400;
	hrs = (t / 3600) % 24;
	mins = (t / 60) % 60;
	secs = t % 60;
	fill = time_string;

	if (days)
	{
		snprintf(fill, MAX_LINE, "%d day", days);

		while (*fill)
			fill++;

		if (days != 1)
			*fill++ = 's';

		if (hrs || mins || secs)
		{
			*fill++ = ',';
			*fill++ = ' ';
		}
	}
	if (hrs)
	{
		snprintf(fill, MAX_LINE, "%d hour", hrs);
		while (*fill)
			fill++;
		if (hrs != 1)
			*fill++ = 's';
		if (mins && secs)
		{
			*fill++ = ',';
			*fill++ = ' ';
		}
		if ((mins && !secs) || (!mins && secs))
		{
			strcpy(fill, " and ");
			while (*fill)
				fill++;
		}
	}
	if (mins)
	{
		snprintf(fill, MAX_LINE, "%d minute", mins);
		while (*fill)
			fill++;
		if (mins != 1)
			*fill++ = 's';
		if (secs)
		{
			strcpy(fill, " and ");
			while (*fill)
				fill++;
		}
	}
	if (secs)
	{
		snprintf(fill, MAX_LINE, "%d second", secs);
		while (*fill)
			fill++;
		if (secs != 1)
			*fill++ = 's';
	}

	*fill++ = 0;
	return time_string;
}

char *int2smalltime(int t)
{
	static char time_string[MAX_LINE+1], *fill;
	int days, hrs, mins, secs;

	if (t == -1)
		return "Never";

	days = t / 86400;
	hrs = (t / 3600) % 24;
	mins = (t / 60) % 60;
	secs = t % 60;
	fill = time_string;

	if (days)
	{
		snprintf(fill, MAX_LINE, "%dd", days);

		while (*fill)
			fill++;
	}
	if (hrs)
	{
		snprintf(fill, MAX_LINE, "%dh", hrs);
		while (*fill)
			fill++;

		*fill++ = 0;
		return time_string;
	}
	if (mins)
	{
		snprintf(fill, MAX_LINE, "%dm", mins);
		while (*fill)
			fill++;
	}
	if (secs)
	{
		snprintf(fill, MAX_LINE, "%ds", secs);
		while (*fill)
			fill++;
	}

	*fill++ = 0;
	return time_string;
}
