#
# makefile
# Mael Valais, 2013-04-06 22:51
#
#
OPT=-Wall -g
EXEC=a.out

all:main

main: main.o key.o rbtree.o queue.o
	gcc main.o key.o rbtree.o queue.o -o $(EXEC) $(OPT)

%.o: %.c
	$(CC) -o $@ -c $< $(OPT)

clean:
	rm -rf *.o $(EXEC)

# vim:ft=make
#

