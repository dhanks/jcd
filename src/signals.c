#include <jcd_super.h>
#include <signal.h>

void         timer_click (void)
{
	node *scan; /* Polymorphism in C?  No way =) */

	/* players */
	for(scan = l_player->head; scan; scan = scan->next)
	{
		player *p = (player *) scan->data;

		if(p->auto_term > 0)
			p->auto_term++;

		if(p->auto_term > 10)
		{
			p->flags &= ~AUTO_TERM;
			p->auto_term = 0;
			tell_player(p, "auto terminal: complete\n");
		}

		p->idle++;
	}

	/* login requsts */
	for(scan = l_request->head; scan; scan = scan->next)
	{
		request *r = (request *) scan->data;

		switch(r->timeout)
		{
			case (0):
				vmlog(4, "Dropping login request `%s', submitted %s due to timeout.\n",
				r->name, time2date(r->submitted));
				LIST_REMOVE(l_request, r);
				FREE(r);
				continue;
				break;
			case (-1):
				continue;
				break;
			default:
				r->timeout--;
				break;
		}
	}

	/* global */
	switch(mark)
	{
		case (0):
			vmlog(2, "-- MARK --\n");
			mark = 60*60;
			break;
		default:
			mark--;
	}
	uptime++;
	return;
}

void         signal_DUMM (int dummy)
{
	return;
}

void         signal_EXIT (int dummy)
{
	node *scan;

	fprintf(stderr, "Going down on signal %i..\n", dummy);
	fflush(stderr);
	for(scan = l_interface->head; scan; scan = scan->next)
	{
		interface *i = (interface *) scan->data;

		shutdown(i->handle.fd, 2);
		close(i->handle.fd);
	}
	shutdown_server();
	exit(255);
	return;
}

void         signal_ALRM (int dummy)
{
	if (timer)
		timer--;
	else
	{
		timer_click ();
		timer = CLICK_RATE;
	}
	return;
}

void         init_timer (void)
{
	struct itimerval t;

	t.it_interval.tv_usec = 1000000 / CLICK_RATE;
	t.it_interval.tv_sec = 0;
	t.it_value.tv_usec = 1000000 / CLICK_RATE;
	t.it_value.tv_sec = 0;

	if (setitimer (ITIMER_REAL, &t, 0) < 0)
	{
		fatal ("init_timer: Failed to set timer.\n");
		return;
	}

	timer = CLICK_RATE;
	return;
}

void         stop_timer (void)
{
	struct itimerval t;

	t.it_interval.tv_usec = 0;
	t.it_interval.tv_sec = 0;
	t.it_value.tv_usec = 0;
	t.it_value.tv_sec = 0;

	if (setitimer (ITIMER_REAL, &t, 0) < 0)
		fatal ("init_timer: Failed to stop timer.\n");
	return;
}

void         init_signals (void)
{
	struct sigaction sa;

	sa.sa_flags = 0;
	sa.sa_handler = signal_EXIT;
	sigaction (SIGHUP, &sa, 0);
	sa.sa_handler = signal_EXIT;
	sigaction (SIGINT, &sa, 0);
	sa.sa_handler = signal_EXIT;
	sigaction (SIGQUIT, &sa, 0);
	sa.sa_handler = signal_EXIT;
	sigaction (SIGILL, &sa, 0);
	sa.sa_handler = signal_EXIT;
	sigaction (SIGABRT, &sa, 0);
	sa.sa_handler = signal_EXIT;
	sigaction (SIGFPE, &sa, 0);
	sa.sa_handler = signal_EXIT;
	sigaction (SIGUSR1, &sa, 0);
	sa.sa_handler = signal_EXIT;
	sigaction (SIGSEGV, &sa, 0);
	sa.sa_handler = signal_EXIT;
	sigaction (SIGUSR2, &sa, 0);
	sa.sa_handler = signal_ALRM;
	sigaction (SIGALRM, &sa, 0);
	sa.sa_handler = signal_EXIT;
	sigaction (SIGTERM, &sa, 0);
	sa.sa_handler = signal_EXIT;
	sigaction (SIGTRAP, &sa, 0);
	signal (SIGPIPE, SIG_IGN);

	return;
}
