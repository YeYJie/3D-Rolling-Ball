CFLAGS = -g -std=c++11
CFLAGS += $(shell pkg-config --cflags glfw3)
# CFLAGS += $(shell pkg-config --cflags assimp)
# LDFLAGS = $(shell pkg-config --cflags --static --libs freetype2) -lharfbuzz
LDFLAGS += $(shell pkg-config --static --libs glfw3)
# LDFLAGS += $(shell pkg-config --static --libs assimp)
LDFLAGS +=  -lSOIL
LDFLAGS += -lGLEW -lGLU -lGL
# CPPFILES = $(wildcard *.cpp)
CPPFILES = main.cpp terrain.cpp shader.cpp

all :
	g++ $(CFLAGS) -o rollingBall $(CPPFILES) $(LDFLAGS)

clean:
	rm ./rollingBall
