x86_64-w64-mingw32-g++ $(find src -name "*.cpp") -ggdb -static-libgcc -static-libstdc++ -lpthread -lmingw32 -lSDL2main -lSDL2 -Wall -o build.exe
