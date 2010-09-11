#
# Copyright (C) 2010 Jean Champemont (jean.champemont@gmail.com)
#
# This file is part of iBrainfuck.
# 
# iBrainfuck is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# iBrainfuck is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with iBrainfuck.  If not, see <http://www.gnu.org/licenses/>.
#

CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -g
LDFLAGS=
EXEC=ibrainfuck
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

ibrainfuck: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
