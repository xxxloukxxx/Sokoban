CC=gcc
# -w pas de warning        -W -Wall -Wextra -Wpedantic : Full warning
CFLAGS=-W -Wall -Wextra -Wpedantic -O3 -L.\SDL2\lib -I.\SDL2\include
#CFLAGS=-w -O3 -L.\SDL2\lib -I.\SDL2\include
# -mwindows pour sans console
LDFLAGS=-L.\SDL2\lib -I.\SDL2\include -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
EXEC=main
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

main: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@del /F /Q *.o 2> NUL

mrproper: clean
	@del /F /Q $(EXEC).exe 2> NUL
	
run: mrproper all clean
	@del /F /Q *.o 2> NUL
	.\$(EXEC).exe