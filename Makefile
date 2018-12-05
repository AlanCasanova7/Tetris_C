CC = clang
CFLAGS = -Wall -Werror -O3 -I SDL/include -I include -Wno-pragma-pack
LDFLAGS = -lSDL2 -L SDL/lib/x64

all: Tetris.exe

Tetris.exe: main.o context.o tetrimino.o
	$(CC) $(LDFLAGS) -o $@ $^

main.o: tests/main.c
	$(CC) $(CFLAGS) -c -o $@ $^

context.o: src/context.c
	$(CC) $(CFLAGS) -c -o $@ $^

tetrimino.o: src/tetrimino.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f *.o