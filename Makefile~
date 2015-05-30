CXX = clang++
CXXFLAGS = -Wall -std=c++11 -fno-exceptions
INCLUDE = -Isrc/Standard
LIB_DIR = -L/usr/local/lib -L/usr/lib64
LIBS = -lXxf86vm -lXcursor -lXinerama -lX11 -lXrandr -lpthread -lXi -lGL -lGLEW -lglfw3 -lGLC
#`pkg-config --libs --static glfw3`
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
