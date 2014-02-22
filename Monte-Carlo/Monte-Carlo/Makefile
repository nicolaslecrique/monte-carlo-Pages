CC=g++
CFLAGS= -W -Wall -std=c++11 -I E:\devs\libs\boost\boost_1_55_0
LDFLAGS=
EXEC=monte-carlo-gcc.exe
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