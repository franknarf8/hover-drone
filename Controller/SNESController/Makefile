CC=g++
TARGET=testcomm
SOURCES=main.cpp CommSNES.cpp
HEADERS=CommSNES.h
DYNLIBS=libhidapi-0.dll
FLAGS=--std=c++11

$(TARGET): CommSNES.o main.cpp
	$(CC) -Iincludes $(DYNLIBS) CommSNES.o main.cpp -o $(TARGET) $(FLAGS)

CommSNES.o: CommSNES.h CommSNES.cpp
	$(CC) -c CommSNES.h CommSNES.cpp $(FLAGS)
clean:
	rm -f *.o

print-%: ; @echo $*=$($*)
