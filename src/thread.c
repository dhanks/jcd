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

/* This code is completly deprecated, thus commented out; you can tinker with
 * it if you'd like */

/* -- starting block --

#define STD_DEF -1

ssize_t fd_write (int fd, void *buf, size_t size)
{
	size_t sent = 0, blocks = 0, x;
	char *cptr = buf;

	lastFdState = lfsOKAY;

	if (fd < 0)
		return (0);

	if (size < swBLOCKSIZE)
		return (write (fd, buf, size));

	blocks = size / swBLOCKSIZE;

	if ((blocks * swBLOCKSIZE) != size)
		blocks++;

	for (x = 0; x < blocks; x++)
	{
		if (x != (blocks - 1))
		{
			if (((sent = write (fd, (void *) cptr, swBLOCKSIZE)) != swBLOCKSIZE) || errno == EINTR)
			{
				lastFdState = lfsERROR;
				return (sent);
			}
		cptr += swBLOCKSIZE;
		}
		else
		{
			size_t left = size - (swBLOCKSIZE * x);

			if (((sent = write (fd, (void *) cptr, left)) != left) || errno == EINTR)
			{
				lastFdState = lfsERROR;
				return (sent);
			}
		}
	}
	return (sent);
}

ssize_t print (int fd, char *format, ...)
{
	va_list      args;
	char        *string;
	size_t       size = 1024;
	size_t       count = 0;

	string = MALLOC (size);

	if (!string)
		return (-1);

	va_start (args, format);

	while (vsnprintf (string, size, format, args) == -1)
	{
		size *= 2;
		string = realloc (string, size);
		if (!string)
			return (-1);
	}

	va_end (args);

	if (fd < 0)
		return (-1);

	count = (fd_write (fd, string, strlen (string) + 1));
	FREE (string);

	return (count);
}

void change_fd (int *fd1, int fd2)
{
	*fd1 = (*fd1 == STD_DEF ? fd2 : *fd1);
	return;
}

pid_t call_func (func * f)
{
	int          status;

	if ((f->pid = fork ()) == 0)
	{
		dup2 (f->stdin, STDIN_FILENO);
		dup2 (f->stdout, STDOUT_FILENO);
		dup2 (f->stderr, STDERR_FILENO);

		status = f->func (f);

		close (STDIN_FILENO);
		close (STDOUT_FILENO);
		close (STDERR_FILENO);
		exit(status);
	}
	return (f->pid);
}

void wait_for_exit (func * f)
{
	int status;

	waitpid (f->pid, &status, 0);
	f->return_val = (WIFEXITED(status) ? WEXITSTATUS(status) : 0);
	return;
}

int open_pipe (int stdi, int stdo, int stde, int pipes, ...)
{
	int fds[2];
	int pos, tot_pos;
	func **funcs;
	void *func_ptr;
	int stdin, stdout, stderr;
	va_list ap;

	stdin = dup (stdi);
	stdout = dup (stdo);
	stderr = dup (stde);

	tot_pos = pipes - 1;
	va_start (ap, pipes);
	funcs = alloca (sizeof (void *) * tot_pos + 1);
	pos = 0;

	while (func_ptr = va_arg (ap, func *))
	{
		funcs[pos] = (func *) func_ptr;
		funcs[pos]->seq = pos;
		pos++;
	}

	va_end (ap);
	pos = tot_pos;

	if (pos == 0)
	{
		change_fd (&funcs[pos]->stdin, stdin);
		change_fd (&funcs[pos]->stdout, stdout);
		change_fd (&funcs[pos]->stderr, stderr);

		call_func (funcs[pos]);

		wait_for_exit (funcs[pos]);
	}
	else
	{
		change_fd (&funcs[pos]->stdout, stdout);
		change_fd (&funcs[pos]->stderr, stderr);
		pos--;

		while (pos >= 0)
		{
			socketpair (AF_UNIX, SOCK_STREAM, 0, fds);
			change_fd (&funcs[pos]->stdout, fds[0]);
			change_fd (&funcs[pos + 1]->stdin, fds[1]);
			change_fd (&funcs[pos]->stderr, stderr);
			pos--;
		}

		pos = 0;
		change_fd (&funcs[pos]->stdin, stdin);

		for (pos = tot_pos; pos >= 0; pos--)
			call_func (funcs[pos]);

		for (pos = 0; pos <= tot_pos; pos++)
		{
			int so = funcs[pos]->stdout;
			int si = funcs[pos]->stdin;
			int se = funcs[pos]->stderr;

			wait_for_exit (funcs[pos]);

			shutdown (so, 2);
			close (so);
			shutdown (si, 2);
			close (si);
			shutdown (se, 2);
			close (se);
		}
	}

	close (stdin);
	close (stdout);
	close (stderr);

	return (funcs[tot_pos]->return_val);
}

int test (func *f)
{
	close (f->stdin);
	close (f->stderr);

	if (f->assoc_player)
		print (f->stdout, "Hello, %s.\n", f->assoc_player->name);
	else
		print (f->stdout, "Hello, unknown person! There is no associated player.\n");

	close (f->stdout);
	return (0);
}

int count (func * f)
{
	char buf[1024];
	unsigned long int count = 0, r = 0;

	while (r = read (f->stdin, buf, 1024))
		count += r;

	print (f->stdout, "Total count of stdin: %lu\n", count);
	return (count);
}

void set_func (func * f, char *name, int (*function) (func *), player *p)
{
	f->name = name;
	f->func = function;
	f->stdin = STD_DEF;
	f->stdout = STD_DEF;
	f->stderr = STD_DEF;
	f->params = NULL;

	if (p)
		f->environment = p->environment;

	f->assoc_player = p;
	return;
}

-- ending block -- */
