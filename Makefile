#
# Copyright (c) [2017] by Christophe Bacara <christophe@bacara.me>
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
# along with xaway. If not, see <http://www.gnu.org/licenses/>.
#

#
# Makefile for xaway
#

# Name of the final binary
PROGRAM_NAME := xaway

# Destination directory: root unless overriden
DESTDIR?=/

# Include path to xscrnsaver header
X11_EXTENSIONS=/usr/include/x11/extensions

# Compilation settings
CC	:= gcc

CFLAGS	:= -Wall -Werror
LFLAGS	:= $(shell pkg-config --cflags --libs x11 xscrnsaver)

#
# Rules
#

all: $(PROGRAM_NAME)

# Binary
$(PROGRAM_NAME): xaway.c
	$(CC) $(CFLAGS) $(LFLAGS) -I$(X11_EXTENSIONS) -o $@ $<

# Installation
install:
	install -d $(DESTDIR)/usr/bin/
	install -m755 xaway   $(DESTDIR)/usr/bin/

	install -d $(DESTDIR)/usr/share/man/man8/
	install	-m644 xaway.8 $(DESTDIR)/usr/share/man/man8/

	install -d $(DESTDIR)/usr/share/licenses/
	install -m644 LICENSE $(DESTDIR)/usr/share/licenses/xaway

# Uninstallation
uninstall:
	rm $(DESTDIR)/usr/bin/xaway
	rm $(DESTDIR)/usr/share/man/man8/xaway.8.gz
	rm $(DESTDIR)/usr/share/licenses/xaway

#
# Special rules
#

.PHONY: clean
clean:
	rm -f *~ $(PROGRAM_NAME)
