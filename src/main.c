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

int main(int argc, char **argv)
{
	ps = argv;
	pc = argc;

	inuse = 0;

	if(chdir(SERVER_ROOT) < 0)
	{
		fprintf(stderr, "%s: couldn't chdir to `%s'.  Please fix in include/jcd.h\n", *argv, SERVER_ROOT);
		fflush(stderr);
		exit(255);
	}

	vmlog(9, "jcd coming up\n");
	MALLOC_INIT();

	l_dfile = list_init("dynamic_file_list", sizeof(list));
	l_player = list_init("player_list", sizeof(list));
	l_interface = list_init("interface_list", sizeof(list));
	l_request = list_init("request_list", sizeof(list));
	l_stream = list_init("stream_list", sizeof(list));

	comm_hash_init();
	{
		struct comm_list *curr = (struct comm_list *) &static_commands;

		while (curr->key)
			comm_hash_add(curr++);
	}

	{
		request *r = request_malloc();
		request_init(r);
		r->flags |= REQ_NAME_BAN|REQ_IP_BAN;
		strcpy(r->name, "scott");
		r->timeout = REQ_NEVER;
		r->one = 50;
		r->two = 66;
		r->three = '*';
		r->four = 4;
		LIST_ADD(l_request, r, sizeof(request));
	}

	mod_load("stat.so");
	mod_load("commands.so");
	system_flags &= ~SHUTDOWN;

	init_dfiles();
	init_timer(); 
	init_signals();
	stream_init();

	/* Change the appearance to something more elegant */
	{
		int i;
		for(i = (argc - 1); i >= 0; i--)
			memset(argv[i], 0, strlen(argv[i]));
		sprintf(argv[0], "[Jade Coiled Dragon]");
	}

	{
		/* Create the interfaces */
		interface *listener = interface_malloc();
		interface *www = interface_malloc();

		interface_init(listener);
		LIST_ADD(l_interface, listener, sizeof(interface));
		interface_listen(listener, 2010);

		listener->device = INTERFACE_JCD;
		listener->flags |= INTERFACE_UP;

		interface_init(www);
		LIST_ADD(l_interface, www, sizeof(interface));
		interface_listen(www, 8000);

		www->device = INTERFACE_JCD;
		www->flags |= INTERFACE_UP;

		signal (SIGPIPE, SIG_IGN);

		idle_loop(NULL);
	}

	list_deinit(l_player);
	list_deinit(l_interface);
	list_deinit(l_stream);
	list_deinit(l_dfile);

	vmlog(9, "jcd down\n");
	exit(0);
}
