SRC := $(wildcard src/*.cpp)
HEADERS := $(wildcard src/*.hpp)
OBJ = $(SRC:src/%.cpp=build/%.o)
AOBJ = $(SRC:src/%.cpp=build/%.asan.o)

TOOLSSRC := $(wildcard tools/*.cpp)
TOOLSHEADERS := $(wildcard tools/*.hpp)
TOOLSOBJ = $(TOOLSSRC:tools/%.cpp=build/%.o)

SFMLINCS := $(shell pkg-config --cflags sfml-all)
SFMLLIBS := $(shell pkg-config --libs sfml-all)
TARGET = asteroids

CCMD = $(CXX) -std=c++20 -g $(SFMLINCS) -c -o $@ $< -Wall -Wextra -Woverloaded-virtual -pedantic-errors $(OPTS)
LCMD = $(CXX) -o $@ -std=c++20 -g $^ $(SFMLLIBS) $(OPTS) -ltbb

$(TARGET): $(OBJ)
	$(LCMD) -O3

asan: $(AOBJ)
	$(LCMD) -Og -fsanitize=address,undefined,leak

build/%.o : src/%.cpp Makefile $(HEADERS) | build
	$(CCMD) -O3

build/%.o : tools/%.cpp Makefile $(HEADERS) | build
	$(CCMD) -O3

build/%.asan.o : src/%.cpp Makefile $(HEADERS) | build
	$(CCMD) -Og -fsanitize=address,undefined,leak

mkres: $(TOOLSOBJ)
	$(LCMD) -O3

build:
	mkdir -p build

clean:
	rm -rf build $(TARGET)
