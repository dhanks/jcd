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

#if !defined(JCD_PROTO)
	#define JCD_PROTO 1

#include <jcd_structs.h>

extern int test(func *);

/* buffer.c */
extern size_t buffer_add(player *, char const*);
extern void buffer_send(player *);

/* custom */
extern int vsnprintf(char *, size_t, const char *, va_list);

/* core.c */
extern void *idle_loop(void *);
extern void *jcd_core_loop(void);

/* elf.c */
extern void comm_hash_add(struct comm_list *);
extern struct comm_list *comm_hash[];
extern int check_modules(char *);
extern int mod_load(char *);
extern void mod_comm_register(struct mod *, char *, char *, unsigned long, int);
extern int mod_unload(char *);
extern int comm_hash_num(const char *);
extern void comm_hash_init(void);
extern void comm_hash_del(struct comm_list *);
extern struct mod *mod_list;
extern int safe_module_load(player *, char *);
extern int safe_module_unload(player *, char *);

/* entity.c */
extern size_t load_player(player *, char *);
extern size_t save_player(player *, char *);

/* dfile.c */
extern char *dfile_data(char const *);
extern struct s_dfile_hardload dfile_hardload[];
extern void init_dfiles(void);
extern int dfile_create(char const *, char const *, int const, int const);
extern int load_dfile(DFILE *, char const *, int const);

/* fatal.c */
extern void fatal(char *);

/* funcs.c */
extern size_t nc(char const *, char const);
extern char *time2date(time_t);
extern char *int2smalltime(int);
extern char *next_space(char *);
extern char *end_string (char *);
extern char *trim_end(char *);
extern char *bad_name(char const *);
extern char *word_time(int);
extern void lower_case(char *);
extern void top_banner(player *, char const);
extern void middle_banner(player *, char const);
extern void center_banner(player *, char const, char const, char const, char const *);
extern void bottom_banner(player *, char const);

/* globals.c */
extern char *stack;
extern char **ps;
extern char *reserved_names[];
extern char *admin_list[];
extern size_t Chunks;
extern int pc;
extern int system_flags;
extern int timer;
extern int inuse;
extern unsigned int uptime;
extern unsigned int mark;
extern unsigned int repeated;
extern interface *interface_head;
extern list *list_head, *l_dfile, *l_player, *l_interface, *l_stream, *l_request;
extern player *player_head;
extern player *player_current;
extern stream *stream_top;
extern pthread_t idle_loop_thread, process_manager_thread, maint_thread;
extern pthread_mutex_t delete_node;

/* handle.c */
extern void SOCK_Handle_Input(player *);
extern void SOCK_read_post_normal(player *, int *);

/* interface.c */
extern interface *interface_malloc(void);
extern char *interface_name(interface *);
extern char *interface_state(interface *);
extern int interface_listen(interface *, int);
extern void interface_init(interface *);

/* list.c */
extern list *list_init(char *, unsigned long);
extern size_t list_deinit(list *);
extern void list_add(list *, void *, unsigned long, char *, int);
extern void list_remove(list *, void *, char *, int);
extern void list_dump(player *, list *);

/* login.c */
extern void get_login(player *, char *);

/* malloc.c */
extern mtrace *mtop;
extern struct memstat mallocstat, *M;
extern struct mtrace *mt, *mts[];
extern void MALLOC_INIT (void);
extern void *sMALLOC (char *, unsigned long, size_t);
extern void *sCALLOC (char *, unsigned long, size_t, size_t);
extern void *sREALLOC (char *, unsigned long, void *, size_t);
extern void *sFREE (char *, unsigned long, void *);

/* output.c */
extern void tell_player(player *, char *, ...);
extern void tell_hack (player *, char *,...);
extern void prompt(player *p, char const *);

/* parse.c */
extern struct comm_list static_commands[];
extern int run_command(player *, char *);

/* player.c */
extern player *player_malloc(void);
extern player *find_player_name(char *);
extern int player_connect(interface *);
extern void player_disconnect(player *);
extern void player_logout(player *, char *, int, int);

/* pseudo-stream.c */
extern int format_output(char *, int, int, int);
extern stream *stream_malloc(void);
extern void stream_driver(player *, char *);
extern void stream_init(void);

/* request.c */
extern request *request_malloc(void);
extern void request_init(request *);

/* signals.c */
extern void init_signals(void);
extern void init_timer(void);

/* socket.c */
extern void shutdown_server(void);
extern void do_backspace(player *);
extern int sock_write(int, void *, size_t);
extern int sock_accept(sock *, sock *);
extern int sock_send(sock *, char *, ...);
extern int player_read_char(player *, char *);
extern int player_input(player *);

/* split.c */
extern struct s_split *split(char const *, size_t, char const, size_t);
extern size_t _nmemb(char const *, char const);
extern void delete_split(struct s_split *);

/* static.c */
extern void static_look(player *);

/* syslog.c */
extern ssize_t vmlog(int, char *, ...);
extern ssize_t malloc_log(char *, ...);

#endif
