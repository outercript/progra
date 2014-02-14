CC?=gcc
CFLAGS?=-Wall
LDFLAGS?=
APPS=vector simple

.PHONY: all clean $(APPS)
all: vector simple

vector: vector.c vectortest.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

simple: vector.c simpletest.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o $(APPS)
