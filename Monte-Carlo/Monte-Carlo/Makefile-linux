CC=g++
CFLAGS=-std=c++11
LDFLAGS=
EXEC=monte-carlo
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)
BINDIR=build-gcc
 
all: $(EXEC)
 
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
 
%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)
 
.PHONY: clean mrproper
 
clean:
	del /F *.o
 
mrproper: clean
	del /F $(EXEC)
