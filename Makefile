NAME= hootwm
CFLAGS= -Wall -Wextra -pedantic
CFLAGS+= -std=c99 -O2
LIBS= -lxcb
CC= cc

# Only for OpenBSD:
LIBPATH= -L/usr/X11R6/lib
INCPATH= -I/usr/X11R6/include

FLAGS= $(INCPATH) $(LIBPATH) $(CFLAGS) $(LIBS)

all: hootwm

hootwm:
	$(CC) $(FLAGS) $(NAME).c -o $(NAME)

clean:
	rm -f $(NAME) *.core
