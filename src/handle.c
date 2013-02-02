/* Jade Coiled Dragon - a multi-user, multi-tasking chat environment
 * Copyright (C) 2000 Douglas R. Hanks Jr.
 *                    Matvei Nicoliavich Busigin
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

char accepted_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,<.>/?\";:'\\|]}[{=+-_0987654321`~!@#$%^&*() ";

void SOCK_read_post_wont (player *p, int *size)
{
	unsigned char c;

	READSOCK (p->handle.fd, c);
	size--;
	p->handle.state = sNORMAL;

	switch (c)
	{
		case TELOPT_ECHO:
			p->opts &= ~TEL_ECHO;
			if(p->flags & AUTO_TERM)
				tell_player(p, "auto terminal: client doesn't want to echo\n");
			break;
		case TELOPT_SGA:
			p->opts &= ~TEL_GA;
			if(p->flags & AUTO_TERM)
				tell_player(p, "auto terminal: client doesn't want iac sga prompts\n");
			break;
		case TELOPT_EOR:
			p->opts &= ~TEL_EOR;
			if(p->flags & AUTO_TERM)
				tell_player(p, "auto terminal: client doesn't want iac eor prompts\n");
			break;
		case TELOPT_NAWS:
			break;
		case TELOPT_NEW_ENVIRON:
			break;
		default:
			vmlog (1, "WARNING : Unknown post-WONT character '%c' (%d)\n", c, c);
			break;
	}
	return;
}

void SOCK_read_post_will (player *p, int *size)
{
	unsigned char c;

	READSOCK (p->handle.fd, c);
	size--;
	p->handle.state = sNORMAL;

	switch (c)
	{
/*
		case TELOPT_NAWS:
		case TELOPT_TTYPE:
			break;
*/
		case TELOPT_SGA:
			p->opts &= ~TEL_EOR;
			p->opts |= TEL_GA;
		case TELOPT_NEW_ENVIRON:
			tell_player (p, "%c%c%c%c%c%s%c%s%c%c%c%c", IAC, SB,
			TELOPT_NEW_ENVIRON, 1, 0, "USER", 0, "ACCT", 0, 3, IAC, SE);
			break;
		default:
			vmlog (1, "WARNING : Unknown post-WILL character '%c' (%d)\n", c, c);
			break;
	}
	return;
}

void SOCK_read_post_sb (player *p, int *size)
{
	unsigned char c;
	short x, y;
	char term[256], *ptr;
	int limit = 0;

	READSOCK (p->handle.fd, c);
	size--;
	p->handle.state = sNORMAL;

	switch (c)
	{
		case (char) TELOPT_TTYPE:
			ptr = term;
			read(p->handle.fd, &c, 1);
			if(c != TELQUAL_IS)
			{
				vmlog(4, "bad TELOPT_TTYPE qualifer `%c' (%i)\n", c, c);
				break;
			}
			while(limit < 255)
			{
				limit++;
				read(p->handle.fd, &c, 1);
				*ptr++ = c;
				if(c == IAC)
					break;
			}
			ptr--;
			*ptr = '\0';
			ptr = trim_end(ptr);
			p->opts |= TEL_TTYPE;
			strcpy(p->screen.type, term);
			if(p->flags & AUTO_TERM)
				tell_player(p, "auto terminal: detected terminal type: %s\n", term);
			break;
		case (char) TELOPT_NAWS:
			/* Big Endian */
			read(p->handle.fd, &x, 2);
			x = ntohs(x);
			read(p->handle.fd, &y, 2);
			y = ntohs(y);

			if(x < 10 || x > 255)
				x = 80;
			if(y < 10 || y > 255)
				y = 24;

			p->opts |= TEL_NAWS;
			p->screen.width = x;
			p->screen.height = y;

			if(p->flags & LOGGED_IN)
			{
				p->flags &= ~CWW;
				p->flags |= WW;

				if(p->flags & AUTO_TERM)
					tell_player(p, "auto terminal: detected terminal size: %ix%i\n", x, y);
				else if(p->flags & CWW || !(p->flags & WW))
					tell_player(p, "You had a custom word wrap set or it was turned off, but your client just send new dimensions for the screen size (%ix%i), these will be used instead.\n", x, y);

			}

			break;
		default:
			vmlog (1, "WARNING : Unknown post-SB character '%c' (%d)\n", c, c);
			break;
	}
	return;
}

void SOCK_read_post_iac (player *p, int *size)
{
	unsigned char c;

	READSOCK (p->handle.fd, c);
	size--;
	p->handle.state = sNORMAL;

	switch (c)
	{
		case AYT:
			tell_player (p, "[ jcd : yes ]\007\n\r");
			break;
		case BREAK:
		case IP:
			DISCONNECT(p);
			return;
			break;
		case EC:
			p->ibuffer[strlen (p->ibuffer) - 1] = '\0';
			break;
		case EL:
			memset (p->ibuffer, '\0', sizeof (p->ibuffer));
			break;
		case DO:
			p->handle.state = sDO;
			break;
		case DONT:
			p->handle.state = sDONT;
			break;
		case WONT:
			p->handle.state = sWONT;
			break;
		case WILL:
			p->handle.state = sWILL;
			break;
		case SB:
			p->handle.state = sSB;
			break;
		case SE:
			break;
		default:
			vmlog (1, "WARNING : Unknown post-IAC character '%c' (%d)\n", c, c);
	}
	return;
}

void SOCK_read_post_normal (player *p, int *size)
{
	unsigned char c;

	READSOCK (p->handle.fd, c);
	size--;

	switch (c)
	{
		case IAC:
			p->handle.state = sIAC;
			break;
		case AYT:
			tell_player (p, "[ jcd : yes ]\007\n\r");
			break;
		case BREAK:
		case IP:
			DISCONNECT(p);
			return;
			break;
		case 8:
		case EC:
			p->ibuffer[strlen (p->ibuffer) - 1] = '\0';
			break;
		case EL:
			memset (p->ibuffer, '\0', sizeof (p->ibuffer));
			break;
		case DO:
			p->handle.state = sDO;
			break;
		case DONT:
			p->handle.state = sDONT;
			break;
		case WONT:
			p->handle.state = sWONT;
			break;
		case WILL:
			p->handle.state = sWILL;
			break;
		case SB:
			p->handle.state = sSB;
			break;
		case SE:
			break;
		case '\r':
		{
			char *pargs;
			int x;

			pargs = p->ibuffer;

			while (pargs)
			{
				if (*pargs == ' ')
					pargs++;
				else
					break;
			}

			for (x = strlen (pargs); x; x--)
			{
				if (pargs[x] == ' ')
					pargs[x] = '\0';
				if (pargs[x] != 0)
					break;
			}

			if (!(p->opts & PREV_N))
			{
				p->opts |= PREV_R;

				if ((p->opts & TEL_ECHO))
					tell_player (p, "\n\r");

				if(p->func)
					(p->func)(p, pargs);
				else if(strlen(pargs) > 0)
					run_command(p, pargs);

				if(p->opts & TEL_PROMPT && strlen(pargs) < 1)
					prompt(p, p->prompt);

				memset (p->ibuffer, '\0', sizeof (p->ibuffer));
			}

			break;
		}
		case '\n':
		{
			char *pargs;
			int x;

			pargs = p->ibuffer;

			while (pargs)
			{
				if (*pargs == ' ')
					pargs++;
				else
					break;
			}

			for (x = strlen (pargs); x; x--)
			{
				if (pargs[x] == ' ')
					pargs[x] = '\0';

				if (pargs[x] != 0)
					break;
			}

			if (!(p->opts & PREV_R))
			{
				p->opts |= PREV_N;

				if ((p->opts & TEL_ECHO))
					tell_player (p, "\n\r");

				if(p->func)
					(p->func)(p, pargs);
				else if(strlen(pargs) > 0)
					run_command(p, pargs);

				if(p->opts & TEL_PROMPT && strlen(pargs) < 1)
					prompt(p, p->prompt);

				memset (p->ibuffer, '\0', sizeof (p->ibuffer));
			}

			break;
		}
		default:
			p->opts &= ~(PREV_N | PREV_R);

			if(strlen(p->ibuffer) > 511)
			{
				tell_player(p, "[packet too large, flushing input]\n");
				memset (p->ibuffer, '\0', sizeof (p->ibuffer));
			}

			if (strchr (accepted_chars, c))
			{
				if(p->opts & TEL_ECHO)
					write(p->handle.fd, (char *) &c, 1);
				p->ibuffer[strlen (p->ibuffer)] = c;
			}
			else
				vmlog (1, "WARNING : Found unhandled, unreadable character '%c' (%d)\n", c, c);
			break;
	}
	return;
}

void SOCK_read_post_do (player *p, int *size)
{
	unsigned char c;

	READSOCK (p->handle.fd, c);
	size--;
	p->handle.state = sNORMAL;

	switch (c)
	{
		case TELOPT_ECHO:
			p->opts |= TEL_ECHO;
			tell_player (p, "%c%c%c", IAC, WILL, TELOPT_ECHO);
			if(p->flags & AUTO_TERM)
				tell_player(p, "auto terminal: client wants to echo\n");
			break;
		case TELOPT_SGA:
			p->opts &= ~TEL_GA;
			tell_player (p, "%c%c%c", IAC, WILL, TELOPT_SGA);
			if(p->flags & AUTO_TERM)
				tell_player(p, "auto terminal: client wants iac ga prompts\n");
			break;
		case TELOPT_EOR:
			p->opts |= TEL_EOR;
			tell_player (p, "%c%c%c", IAC, WILL, TELOPT_ECHO);
			if(p->flags & AUTO_TERM)
				tell_player(p, "auto terminal: client wants iac eor prompts\n");
		default:
			vmlog (1, "WARNING : Unknown post-DO character - '%c' (%d)\n", c, c);
			tell_player (p, "%c%c%c", (char) IAC, (char) WONT, c);
			break;
	}
	return;
}

void SOCK_read_post_dont (player *p, int *size)
{
	unsigned char c;

	READSOCK (p->handle.fd, c);
	size--;
	p->handle.state = sNORMAL;

	switch (c)
	{
		case TELOPT_ECHO:
			p->opts &= ~TEL_ECHO;
			if(p->flags & AUTO_TERM)
				tell_player(p, "auto terminal: client doesn't want to echo\n");
			break;
		case TELOPT_SGA:
			p->opts &= ~TEL_GA;
			if(p->flags & AUTO_TERM)
				tell_player(p, "auto terminal: client doesn't want iac sga prompts\n");
			break;
		case TELOPT_EOR:
			p->opts &= ~TEL_EOR;
			if(p->flags & AUTO_TERM)
				tell_player(p, "auto terminal: client doesn't want iac eor prompts\n");
			break;
		default:
			vmlog (1, "WARNING : Unknown post-DONT character - '%c' (%d)\n", c, c);
			break;
	}
	return;
}

void SOCK_Handle_Input (player *p)
{
	int size;
	int returnv;

	returnv = ioctl (p->handle.fd, FIONREAD, &size);

	if (returnv < 0 || !size)
	{
		DISCONNECT(p);
		return;
	}

	p->handle.out.packets++;
	p->handle.out.bytes += size;
	p->handle.interface->in.packets++;
	p->handle.interface->in.bytes += size;
	while (size)
	{
		switch (p->handle.state)
		{
			case sIAC:
				SOCK_read_post_iac (p, &size);
				break;
			case sSB:
				SOCK_read_post_sb (p, &size);
				break;
			case sWILL:
				SOCK_read_post_will (p, &size);
				break;
			case sWONT:
				SOCK_read_post_wont (p, &size);
				break;
			case sDO:
				SOCK_read_post_do (p, &size);
				break;
			case sDONT:
				SOCK_read_post_dont (p, &size);
				break;
			case sNORMAL:
			default:
				SOCK_read_post_normal (p, &size);
				break;
		}
		size--;
	}
	return;
}
