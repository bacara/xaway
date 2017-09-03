#
# Copyright (c) [2017] by Christophe Bacara <christophe.bacara@live.fr>
#
# This file is part of xaway.
#
# Some open source application is free software: you can redistribute
# it and/or modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
#
# Some open source application is distributed in the hope that it will
# be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with xaway. If not, see
# <http://www.gnu.org/licenses/>.
#

#
# Makefile for xaway
#

# Name of the final binary
PROGRAM_NAME := xaway

# Destination
DESTDIR?=/

# Compilation settings

CC	:= gcc
CFLAGS	:= -Wall -Werror

LFLAGS	:= -lXss
LFLAGS	+= -lX11

# Generic rule
all: $(PROGRAM_NAME)

$(PROGRAM_NAME): xaway.c
	$(CC) $(CFLAGS) $(LFLAGS) -I/usr/include/X11/extensions -o $@ $<

#
# Special rules
#

.PHONY: clean cleanall install
clean:

cleanall: clean
	rm -f *~ $(PROGRAM_NAME)

install:
	install -D -m755 xaway   $(DESTDIR)/usr/bin/
	install	-D -m644 xaway.8 $(DESTDIR)/usr/share/man/man8/
	install -D -m644 LICENCE $(DESTDIR)/usr/share/licenses/xaway
