CC = gcc
CFLAGS = -lrt -lpthread

test : test.o LinkedList.o
	$(CC) -o $@ $^ $(CFLAGS)

test.o : test.c
	$(CC) -c $< -o $@

LinkedList.o : LinkedList.c
	$(CC) -c $< -o $@

clean :
	rm *.o test a.out