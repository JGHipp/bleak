CC=g++
CFLAGS=-g -lSDL2
FILES=$(shell find src -name "*.cpp")
EXEC=build

bleak: $(FILES)
	$(CC) $(FILES) $(CFLAGS) -o $(EXEC)

run: $(EXEC)
	./$(EXEC)
