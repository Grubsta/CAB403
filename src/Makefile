# CAB403 - Minesweeper

SRC=ms_client.c ms_server.c
OBJ = $(SRC:.c=.o)

FLAGS=-std=gnu99 -Wall -Werror -g

build: $(OBJ)
	gcc $(FLAGS) $@ -o $(OBJ) $(DIRS) $(LIBS)
