asteroids: *.cpp
	$(CXX) -std=c++20 -O3 -o $@ *.cpp $(shell pkg-config --cflags --libs sfml-all)
