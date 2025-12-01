# -*- Makefile -*-

CC = gcc

FLAGS = -Wall -Wextra -Iinclude

OBJ = ./build/main.o ./build/board.o ./build/king.o ./build/queen.o ./build/rook.o ./build/bishop.o ./build/knight.o ./build/pawn.o

INCLUDE = ./include/board.h ./include/king.h ./include/queen.h ./include/rook.h ./include/bishop.h ./include/knight.h ./include/pawn.h

TARGET = ./bin/chess

run: compile
	${TARGET}

compile : ${TARGET}

${TARGET}: ${OBJ}
	${CC} ${OBJ} -o ${TARGET} ${FLAGS}

./build/main.o: ./src/main.c ${INCLUDE}
	${CC} ./src/main.c -c -o ./build/main.o ${FLAGS}

./build/board.o: ./src/board.c ./include/board.h
	${CC} ./src/board.c -c -o ./build/board.o ${FLAGS}

./build/king.o: ./src/king.c ./include/king.h ./include/board.h
	${CC} ./src/king.c -c -o ./build/king.o ${FLAGS}

./build/queen.o: ./src/queen.c ./include/queen.h ./include/board.h ./include/rook.h ./include/bishop.h
	${CC} ./src/queen.c -c -o ./build/queen.o ${FLAGS}

./build/rook.o: ./src/rook.c ./include/rook.h ./include/board.h
	${CC} ./src/rook.c -c -o ./build/rook.o ${FLAGS}

./build/bishop.o: ./src/bishop.c ./include/bishop.h ./include/board.h
	${CC} ./src/bishop.c -c -o ./build/bishop.o ${FLAGS}

./build/knight.o: ./src/knight.c ./include/knight.h ./include/board.h
	${CC} ./src/knight.c -c -o ./build/knight.o ${FLAGS}

./build/pawn.o: ./src/pawn.c ./include/pawn.h ./include/board.h
	${CC} ./src/pawn.c -c -o ./build/pawn.o ${FLAGS}