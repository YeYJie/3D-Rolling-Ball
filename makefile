CXX = g++

CXXFLAGS = -g -std=c++11
CXXFLAGS += -I./SOIL -I./glm
CXXFLAGS += $(shell pkg-config --cflags glfw3)

LDFLAGS += $(shell pkg-config --static --libs glfw3)
LDFLAGS += ./SOIL/libSOIL.a
LDFLAGS += -lGLEW -lGLU -lGL

OBJS = main.o terrain.o loader.o camera.o ball.o skybox.o \
			gui.o water.o text.o menu.o sun.o \
			shadow.o blur.o hdr.o utils.o\
			level1.o level2.o level3.o

EXE = rollingBall

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all : $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS) $(LDFLAGS)

test : test.o
	$(CXX) -O0 -o test test.o $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f *.o $(EXE)
