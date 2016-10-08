CXXFLAGS = -Wall -Wextra -O2 -g -std=c++11

image_simple: main.o tga.o gl-utils.o
	g++ $^ -o $@ -lglut -lGLEW -lGLU -lGL -pthread

clean:
	@rm -f *o *~ image_simple

