OPT=-Wall -g
EXEC=rbtree_to_dot

all: rbtree_to_dot

rbtree_to_dot: main.o key.o rbtree.o queue.o
	gcc main.o key.o rbtree.o queue.o -o $(EXEC) $(OPT)

%.o: %.c
	$(CC) -o $@ -c $< $(OPT)

clean:
	rm -rf *.o $(EXEC)

# vim:ft=make
#
