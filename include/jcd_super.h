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

#if defined(__STRICT_ANSI__)
	#include <features.h>
	#define __USE_POSIX
	#define __USE_BSD
	#define __USE_MISC
#endif /* __STRICT_ANSI__ */

#if defined( LINUX ) && defined( GLIBC )
	#define __STRICT_ANSI__
	#include <sys/socket.h>
	#undef __STRICT_ANSI__
#else
	#include <sys/socket.h>
#endif

#include <arpa/inet.h>
#include <arpa/telnet.h>
#include <assert.h>
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include <jcd.h>
#include <jcd_ansi.h>
#include <jcd_buffer.h>
#include <jcd_bytes.h>
#include <jcd_config.h>
#include <jcd_dfile.h>
#include <jcd_entity.h>
#include <jcd_list.h>
#include <jcd_malloc.h>
#include <jcd_module.h>
#include <jcd_request.h>
#include <jcd_socket.h>
#include <jcd_thread.h>
#include <jcd_player.h>
#include <jcd_privs.h>
#include <jcd_prototypes.h>
#include <jcd_structs.h>
