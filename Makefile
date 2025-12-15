# -*- Makefile -*-

CC = gcc

FLAGS = -Wall -Wextra -Iinclude 

OBJ = ./build/main.o ./build/board.o ./build/king.o ./build/queen.o ./build/rook.o ./build/bishop.o ./build/knight.o ./build/pawn.o

HEADERS = ./include/board.h ./include/king.h ./include/queen.h ./include/rook.h ./include/bishop.h ./include/knight.h ./include/pawn.h

ifeq ($(OS),Windows_NT)
    TARGET = bin/chess.exe
else
    TARGET = bin/chess
endif

run: compile
ifeq ($(OS),Windows_NT)
	.\$(TARGET)
else
	./$(TARGET)
endif

compile : dirs $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(FLAGS) -o $(TARGET) 

dirs:
ifeq ($(OS),Windows_NT)
	if not exist build mkdir build
	if not exist bin mkdir bin
else
	mkdir -p build bin
endif

./build/main.o: ./src/main.c $(HEADERS)
	$(CC) ./src/main.c -c  $(FLAGS) -o ./build/main.o 

./build/board.o: ./src/board.c ./include/board.h
	$(CC) ./src/board.c -c  $(FLAGS) -o ./build/board.o 

./build/king.o: ./src/king.c ./include/king.h ./include/board.h
	$(CC) ./src/king.c -c $(FLAGS) -o ./build/king.o 

./build/queen.o: ./src/queen.c ./include/queen.h ./include/board.h ./include/rook.h ./include/bishop.h
	$(CC) ./src/queen.c -c  $(FLAGS) -o ./build/queen.o 

./build/rook.o: ./src/rook.c ./include/rook.h ./include/board.h
	$(CC) ./src/rook.c -c $(FLAGS) -o ./build/rook.o 

./build/bishop.o: ./src/bishop.c ./include/bishop.h ./include/board.h
	$(CC) ./src/bishop.c -c $(FLAGS) -o ./build/bishop.o 

./build/knight.o: ./src/knight.c ./include/knight.h ./include/board.h
	$(CC) ./src/knight.c -c $(FLAGS) -o ./build/knight.o 

./build/pawn.o: ./src/pawn.c ./include/pawn.h ./include/board.h
	$(CC) ./src/pawn.c -c $(FLAGS) -o ./build/pawn.o 