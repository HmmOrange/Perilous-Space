# Compiler
CC = g++

# Compiler flags
CFLAGS = -I./src/include

# Linker flags
LDFLAGS = -L./src/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Source files
# Find all cpp files in src directory, but main.cpp stays in root
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=obj/%.o)

# Target executable
TARGET = main.exe

all: $(TARGET)

$(TARGET): obj/main.o $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

obj/main.o: main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q obj\*.o $(TARGET)
