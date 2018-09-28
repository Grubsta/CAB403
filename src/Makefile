# CAB403 - Minesweeper

SRC=ms_client.c ms_server.c
OBJ = $(SRC:.c=.o)

FLAGS=-std=gnu99 -Wall -Werror -g

all: build

clean:
	for file in $(OBJ); do
		if [ -f $$file ]; then
			rm "$$file"
		fi 
	done


build: $(OBJ)
	gcc $(FLAGS) $@ -o $(OBJ) $(DIRS) $(LIBS)
