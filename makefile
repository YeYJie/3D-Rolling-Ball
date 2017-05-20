CXX = g++
CXXFLAGS = -g -std=c++11
CXXFLAGS += $(shell pkg-config --cflags glfw3)
CXXFLAGS += $(shell pkg-config --cflags --static freetype2)
# CFLAGS += $(shell pkg-config --cflags assimp)
LDFLAGS = $(shell pkg-config --cflags --static --libs freetype2) -lharfbuzz
LDFLAGS += $(shell pkg-config --static --libs glfw3)
# LDFLAGS += $(shell pkg-config --static --libs assimp)
LDFLAGS +=  -lSOIL
LDFLAGS += -lGLEW -lGLU -lGL
# CPPFILES = $(wildcard *.cpp)

OBJS = main.o terrain.o loader.o camera.o ball.o skybox.o \
			gui.o water.o text.o

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all : $(OBJS)
	$(CXX) -o rollingBall $(OBJS) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f *.o ./rollingBall
