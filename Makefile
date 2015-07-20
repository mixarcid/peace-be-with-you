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
LIBS = -lX11 -lXxf86vm -lXcursor -lXinerama -lXrandr -lXext -lXi -lGL
LIBDIR = -LThirdParty/lib/Linux
endif
ifeq ($(UNAME), Darwin)
LIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation
LIBDIR = -LThirdParty/lib/OSX
endif

LIBS += -lGLEW -lglfw3 -lSOIL -lpthread

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LIBDIR) $(LIBS) $(LIBS) $(OBJECTS) -o $@ $(LIBS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@rm $(OBJECTS) $(EXECUTABLE)
