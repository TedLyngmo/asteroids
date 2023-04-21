SRC := $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=build/%.o)
AOBJ = $(SRC:%.cpp=build/%.asan.o)

SFMLINCS := $(shell pkg-config --cflags sfml-all)
SFMLLIBS := $(shell pkg-config --libs sfml-all)
TARGET = asteroids

CCMD = $(CXX) -std=c++20 $(SFMLINCS) -c -o $@ $< -Wall -Wextra -Woverloaded-virtual -pedantic-errors $(OPTS)
LCMD = $(CXX) -o $@ -std=c++20 $^ $(SFMLLIBS) $(OPTS)

$(TARGET): $(OBJ)
	$(LCMD) -O3

asan: $(AOBJ)
	$(LCMD) -Og -g --fsanitize=address,undefined,leak

build/%.o : %.cpp Makefile $(HEADERS) | build
	$(CCMD) -O3

build/%.asan.o : %.cpp Makefile $(HEADERS) | build
	$(CCMD) -Og -g -fsanitize=address,undefined,leak


build:
	mkdir -p build

clean:
	rm -rf build $(TARGET)
