NAME= catwm
CFLAGS= -Wall -Wextra -pedantic
CFLAGS+= -std=c99 -O2
LIBS= -lX11
CC= gcc

# Only for OpenBSD:
LIBPATH= -L/usr/X11R6/lib
INCPATH= -I/usr/X11R6/include

FLAGS= $(INCPATH) $(LIBPATH) $(CFLAGS) $(LIBS)

all: catwm

catwm:
	$(CC) $(FLAGS) $(NAME).c -o $(NAME)

clean:
	rm -f $(NAME)
