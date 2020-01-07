
CXX = g++
CXXFLAGS = -Wall -g
default:
	$(CXX) -c main.cpp
	$(CXX) -c cube.cpp
	$(CXX) -c plane.cpp
	$(CXX) -c vector3.cpp
	$(CXX) $(CXXFLAGS) -o runfile main.o cube.o plane.o vector3.o -lGL -lglut -lGLU

