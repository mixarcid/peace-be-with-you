CXX=clang++
CXXFLAGS= -Wall -std=c++11
INCLUDE = -I../src -I../src/Standard -I../ThirdParty/include
TMP_SOURCES= $(wildcard ../src/*.cpp) $(wildcard ../src/Standard/*.cpp)
SOURCES = $(filter-out ../src/main.cpp,$(TMP_SOURCES))
SOURCES += main.cpp
#OBJDIR = obj/
OBJECTS=$(SOURCES:.cpp=.o)
OUTDIR = bin/
EXECUTABLE= $(OUTDIR)test

UNAME = $(shell uname)

ifeq ($(UNAME), Linux)
LIBS = -lX11 -lXxf86vm -lXcursor -lXinerama -lXrandr -lXext -lXi -lGL
LIBDIR = -L../ThirdParty/lib/Linux
endif
ifeq ($(UNAME), Darwin)
LIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation
LIBDIR = -LThirdParty/lib/OSX
endif

LIBS += -lGLEW -lglfw3 -lSOIL -lpthread

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@rm -f ../src/main.o
	$(CXX) $(LIBDIR) $(LIBS) $(LIBS) $(OBJECTS) -o $@ $(LIBS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@rm -f $(OBJECTS) $(EXECUTABLE)
