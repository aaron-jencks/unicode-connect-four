CC = gcc
export INCLUDE_PATH = include/
export SOURCE_PATH = source/
export CFLAGS = -g -Ddebug

objects = utf8.o player.o game.o stringbuilder.o display.o
targets = main

all: $(targets) $(objects);

main: $(objects) main.c
	$(CC) $(CFLAGS) -o main main.c $(objects) -lreadline

utf8.o: utf8.c utf8.h
	$(CC) $(CFLAGS) -o utf8.o -c $<

stringbuilder.o: stringbuilder.c stringbuilder.h utf8.h
	$(CC) $(CFLAGS) -o stringbuilder.o -c $<

player.o: player.c player.h gamedefs.h
	$(CC) $(CFLAGS) -o player.o -c $<

game.o: game.c game.h gamedefs.h
	$(CC) $(CFLAGS) -o game.o -c $<

display.o: display.c display.h utf8.h
	$(CC) $(CFLAGS) -o display.o -c $<

.PROXY: clean
clean:
	rm $(targets) $(objects)
