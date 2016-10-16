CXXFLAGS = -Wall -Wextra -g -O2 -std=c++0x

FBO: main.o tga.o gl-utils.o matrix.o Mesh.o TextureTGA.o
	g++ $^ -o $@ -lglut -lGLEW -lGLU -lGL -pthread

clean:
	@rm -f *o *~ FBO

gl-utils.o: gl-utils.cc gl-utils.h
main.o: main.cc gl-utils.h matrix.h image.h
matrix.o: matrix.cc matrix.h
tga.o: tga.cc image.h
Mesh.o: Mesh.cpp Mesh.hpp
TextureTGA.o: TextureTGA.cpp TextureTGA.hpp
