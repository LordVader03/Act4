CC=gcc
CFLAGS=-c -Wall -O3 -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion
ACT4=act4.c
OBJECTS_ACT4=$(ACT4:.c=.o)
ACT4_EXECUTABLE=act4

all: $(ACT4) $(ACT4_EXECUTABLE)

$(ACT4_EXECUTABLE): $(OBJECTS_ACT4)
	$(CC) $(OBJECTS_ACT4) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

execute: $(ACT4_EXECUTABLE)
	 ./$(ACT4_EXECUTABLE)

clean:
	/bin/rm -rf *.o
	/bin/rm -rf $(ACT4_EXECUTABLE)