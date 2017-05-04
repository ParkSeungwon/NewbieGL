CC = g++
OBJ = $(wildcard *.o)
EXE = $(patsubst %.x, ../%.x, $(wildcard *.x))
glLIB = -lGLEW -lGLU -lGL -lglut -lglfw
cvLIB = $(shell pkg-config opencv --libs)

all : $(EXE)

../%.x : %.x $(OBJ)
	$(CC) -o $@ $< $(OBJ) $(glLIB) $(cvLIB)


