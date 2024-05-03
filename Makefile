# cc and flags
CC = g++
CXXFLAGS = -std=c++11 -g -Wall
#CXXFLAGS = -std=c++11 -O3 -Wall

# folders
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

# all sources, objs, and header files
MAIN = Main
TARGET = tp2.out
SRC = src/main.cpp
OBJ = obj/main.o

$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

all: $(OBJ)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(OBJ)

build: $(OBJ)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(OBJ)

run:
	$ ./$(TARGET)

clean:
	@rm -rf $(OBJ_FOLDER)* $(TARGET)
