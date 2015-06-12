UNAME = $(shell uname)
CXX = clang++
CXXFLAGS = -Wall -std=c++11
INCLUDE = -Isrc/Standard -IThirdParty/include #-I/usr/include -I/usr/local/include

ifeq ($(UNAME), Linux)
INCLUDE += -I/usr/local/include/SOIL
LIBS = -lXxf86vm -lXcursor -lXinerama -lX11 -lXrandr -lpthread -lXi -lGL
LIB_DIR = -L/usr/lib64
endif
ifeq ($(UNAME), Darwin)
LIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation
endif

LIBS += -lGLEW -lglfw3 -lSOIL
#LIB_DIR += —L/usr/local/lib L/usr/local/lib
LIB_DIR += -LThirdParty/lib

OUTPUT = peace
OUT_DIR = bin
OBJ_DIR = obj

OBJECTS = $(CXX) $(CXXFLAGS) $(INCLUDE) -c
EXECUTABLE = $(CXX) $(LIB_DIR) $(CXXFLAGS) -o $(OUT_DIR)/$(OUTPUT)

all: debug

debug: main

release: CXXFLAGS += -DNDEBUG -O3
release: main

main:
	$(OBJECTS) $(wildcard src/Standard/*.cpp) $(wildcard src/*.cpp)
	@mv *.o $(OBJ_DIR)
	$(EXECUTABLE) $(OBJ_DIR)/*.o $(LIBS)
clean:
	@rm $(OUT_DIR)/$(OUTPUT)
	@rm $(OBJ_DIR)/*.o
