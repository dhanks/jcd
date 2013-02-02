# Jade Coiled Dragon - a multi-user, multi-tasking chat environment
# Copyright (C) 2000 Douglas R. Hanks Jr.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

SERVER_ROOT	= /home/gimli/jcd
SERVER_BIN	= jcd
VERSION		= 0.1.0
BINDIR		= ../bin/
CC		= gcc
PERMS		= 0700
LFLAGS		= -lpthread
CFLAGS		= -O2 -ansi -pedantic -Wall
LFLAGS		= -export-dynamic -ldl
INCLUDE		= -I/home/gimli/jcd/include/
SRCS		= $(shell ls *.c)
OBJECTS		= $(SRCS:.c=.o)


all:	deps jcd
	make -C modules

install:
	mv out/jcd ../bin
	chmod $(PERMS) ../bin/jcd

clean:	
	find . -type f -name "*.o" | xargs rm -rf
	rm -rf ../bin/$(SERVER_BIN) $(SERVER_BIN) core .depend *~
	(cd out && rm -rf $(SERVER_BIN) core .depend *~)
	cat /dev/null > .depend
	make -C modules clean

jcd:	$(OBJECTS)
	(cd out && $(CC) $(CFLAGS) $(INCLUDE) -o $(SERVER_BIN) $(OBJECTS) $(LFLAGS))

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c -o out/$@ $<

deps:	
	@./.create_deps

include .depend
