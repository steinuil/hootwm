NAME= hootwm
CFLAGS= -Wall -Wextra -pedantic
CFLAGS+= -std=c99 -O2
LIBS= -lxcb
CC= cc
INSTALLPATH= ~/bin

# Only for OpenBSD:
LIBPATH= -L/usr/X11R6/lib
INCPATH= -I/usr/X11R6/include

FLAGS= $(INCPATH) $(LIBPATH) $(CFLAGS) $(LIBS)

all: main

install: main
	mv $(NAME) $(INSTALLPATH)

uninstall: clean
	rm -f $(INSTALLPATH)/$(NAME)

main:
	$(CC) $(FLAGS) $(NAME).c -o $(NAME)

clean:
	rm -f $(NAME) *.core
