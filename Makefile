CXX=clang++
CXXFLAGS= -Wall -std=c++11
INCLUDE = -Isrc/Standard -IThirdParty/include
SOURCES=$(wildcard src/*.cpp) $(wildcard src/Standard/*.cpp)
#OBJDIR = obj/
OBJECTS=$(SOURCES:.cpp=.o)
OUTDIR = bin/
EXECUTABLE= $(OUTDIR)peace

UNAME = $(shell uname)

ifeq ($(UNAME), Linux)
LIBS = -lXxf86vm -lXcursor -lXinerama -lX11 -lXrandr -lpthread -lXi -lGL
LIBDIR = -LThirdParty/lib/Linux
endif
ifeq ($(UNAME), Darwin)
LIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation
LIBDIR = -LThirdParty/lib/OSX
endif

LIBS += -lGLEW -lglfw3 -lSOIL

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LIBDIR) $(LIBS) $(OBJECTS) -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@rm $(OBJECTS) $(EXECUTABLE)
