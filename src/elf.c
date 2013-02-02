/* Jade Coiled Dragon - a multi-user, multi-tasking chat environment
 * Copyright (C) 2000 Douglas R. Hanks Jr.
 *                    Stephen A. Olesen
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

tag_list accepted_modules[] =
{
	{"comm_engine.so", 0, 0, 0, 0, 0},
	{"commands.so", 0, 0, 0, 0, 0},
	{"stat.so", 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0},
};

struct mod *mod_list = NULL, *mod_curr = NULL;
struct comm_list *comm_hash[C_BUCKETS];
extern int mod_load(char *);

int check_modules(char *mod)
{
	int i;

	for (i = 0; accepted_modules[i].key; i++)
	{
		if (!strcasecmp(accepted_modules[i].key, mod))
		return 0;
	}
	return -1;
}

int safe_module_load(player *p, char *str)
{
	if(!*str)
	{
		tell_player(p, " Usage : insmod <module.so>\n");
		return (-1);
	}

	if((check_modules(str)) < 0)
	{
		tell_player(p, " Couldn't find module `%s'\n", str);
		return (-1);
	}

	switch(mod_load(str))
	{
		case (0):
			tell_player(p, "Successfully loaded module `%s'\n", str);
			return (0);
			break;
		case (-1):
			tell_player(p, "dlopen(location, RTLD_LAZY) failed on `%s'\n", str);
			return (-1);
			break;
		case (-2):
			tell_player(p, "dlsym(mod_curr->handle, \"name\") failed on `%s'\n", str);
			return (-1);
			break;
		case (-3):
			tell_player(p, "dlsym(mod_curr->handle, \"reg\") failed on `%s`\n", str);
			return (-1);
			break;
		default:
			tell_player(p, "You just blew up the system, good job!\n");
			return (-2);
			break;
	}
}

int safe_module_unload(player *p, char *str)
{
	if(!*str)
	{
		tell_player(p, " Usage : rmmod <module.so>\n");
		return (-1);
	}

	if ((check_modules(str)) < 0)
	{
		tell_player(p, "Couldn't find module `%s'\n", str);
		return (-1);
	}

	switch (mod_unload(str))
	{
		case (0):
			tell_player(p, "Successfully unloaded module `%s'\n", str);
			return (0);
			break;
		case (-1):
			tell_player(p, "if(!mod_curr) on `%s'\n", str);
			return (-1);
			break;
		default:
			tell_player(p, "You just blew up the system, good job!\n");
			return (-1);
			break;
	}
}

int mod_load(char *name)
{
	char *(*tmp_func) (void);
	char *(*tmp_func2) (struct mod *);
	char location[MAX_LINE+1];
	int error = 0, ret = 0;

	snprintf(location, MAX_LINE, "%s/src/modules/modules/%s", SERVER_ROOT, name);
	lower_case(location);

	if (mod_list)
	{
		mod_curr = mod_list;

		while (mod_curr->next)
			mod_curr = mod_curr->next;

		mod_curr->next = (struct mod *) MALLOC(sizeof(struct mod));
		mod_curr = mod_curr->next;
		mod_curr->next = NULL;
	}
	else
	{
		mod_curr = mod_list;
		mod_curr = mod_list = (struct mod *) MALLOC(sizeof(struct mod));
		mod_curr->next = NULL;
	}

	mod_curr->fn = (char *) strdup(location);

	if ((mod_curr->handle = (void *) dlopen(location, RTLD_LAZY)) == NULL)
	{
		error = 1;
		ret = -1;
		vmlog(5, " dlopen (%s) : %s\n", name, dlerror());
	}
	if ((tmp_func = (char *(*)()) dlsym(mod_curr->handle, "name")) == NULL)
	{
		error = 1;
		ret = -2;
		vmlog(5, " dlsym(handle, name) (%s) : %s\n", name, dlerror());
	}

	if (tmp_func && error == 0)
		mod_curr->name = (char *) tmp_func();

	if ((tmp_func2 = (char *(*)(struct mod *)) dlsym(mod_curr->handle, "reg")) == NULL)
	{
		error = 1;
		ret = -3;
		vmlog(5, " dlsym(handle, mod_curr->handle) (%s) : %s\n", name, dlerror());
	}

	mod_curr->comm = NULL;

	if (tmp_func2 && error == 0)
		tmp_func2(mod_curr);

	if (error)
	{
		strcpy(mod_curr->name, "bad");
		mod_unload("bad");
		return (ret);
	}
	return (0);
}

void mod_comm_register(struct mod *m, char *key, char *fnc, unsigned long level, int flags)
{
	struct new_comm *nc;

	if (!m)
		return;
	if (!m->comm)
		nc = m->comm = (struct new_comm *) MALLOC(sizeof(struct new_comm));
	else
	{
		nc = m->comm;

		while (nc->next)
			nc = nc->next;

		nc->next = (struct new_comm *) MALLOC(sizeof(struct new_comm));
		nc = nc->next;
	}

	nc->lst.key = key;
	nc->lst.comm_func = (int (*)(struct s_player *, char *)) dlsym(m->handle, fnc);
	nc->lst.level = level;
	nc->lst.flags = flags;
	nc->next = NULL;
	comm_hash_add(&nc->lst);
}

int mod_unload(char *name)
{
	struct new_comm *nc;
	struct mod *mc;

	mod_curr = mod_list;

	while (mod_curr && strcasecmp(mod_curr->name, name))
		mod_curr = mod_curr->next;

	if (!mod_curr)
		return -1;

	nc = mod_curr->comm;

	while (nc)
	{
		struct new_comm *nc2;

		nc2 = nc->next;
		comm_hash_del(&nc->lst);

		if (nc)
			FREE(nc);

		nc = nc2;
	}

	if (mod_curr->handle)
		dlclose(mod_curr->handle);

	if (mod_curr == mod_list)
	{
		mod_list = mod_curr->next;
		if (mod_curr)
			FREE(mod_curr);
	}
	else
	{
		mc = mod_list;

		while (mc->next && mc->next != mod_curr)
			mc = mc->next;

		if (mc->next == mod_curr)
		{
			mc->next = mod_curr->next;
			if (mod_curr)
				FREE(mod_curr);
		}
	}
	return 0;
}

int comm_hash_num(const char *c)
{
	int h = *c;

	while (*++c)
	{
		int n = *c;

		h = ((h << 5) + n) % C_BUCKETS;
	}

	h %= C_BUCKETS;
	return (h);
}

void comm_hash_init(void)
{
	int i = 0;

	for (; i < C_BUCKETS; i++)
		comm_hash[i] = NULL;
}

void comm_hash_add(struct comm_list *c)
{
	int h = comm_hash_num(c->key);

	c->next = comm_hash[h];
	comm_hash[h] = c;
}

void comm_hash_del(struct comm_list *c)
{
	int h = comm_hash_num(c->key);
	struct comm_list *c2 = comm_hash[h];

	if (c2 == c)
		comm_hash[h] = c->next;
	else
	{
		while (c2)
		{
			if (c2->next == c)
			{
				c2->next = c->next;
				break;
			}
			else
				c2 = c2->next;
		}
	}
}
