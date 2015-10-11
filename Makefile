CXX=g++-5
PREPROCESS=cpp
EXPANDER=expander.py --eval "makefile_dir=\"$(shell pwd)\""
CXXFLAGS= -Werror -Wall -std=c++1y -fno-rtti
DEBUG_FLAGS= -rdynamic -ggdb
INCLUDE= -Isrc -Isrc/Standard -IThirdParty/include

SOURCES= $(wildcard src/*.cpp) $(wildcard src/Standard/*.cpp)

ifeq ($(MAKECMDGOALS), tests)
SOURCES:= $(filter-out src/main.cpp, $(SOURCES))
SOURCES+= $(wildcard tests/*.cpp)
INCLUDE+= -Itests
endif

EXPANSIONS=$(SOURCES:.cpp=.ii)
OBJECTS=$(SOURCES:.cpp=.o)

#Thanks, Jonathan Cline from StackOverflow
ifeq ($(MAKECMDGOALS),clean)
DEPENDS=
else
DEPENDS=$(SOURCES:.cpp=.d)
endif

OUTDIR= bin/
EXECUTABLE= $(OUTDIR)peace
TEST_EXECUTABLE = $(OUTDIR)test

UNAME= $(shell uname)

ifeq ($(UNAME), Linux)
LIBS= -lX11 -lXxf86vm -lXcursor -lXinerama -lXrandr -lXext -lXi -lGL
LIBDIR= -LThirdParty/lib/Linux
endif
ifeq ($(UNAME), Darwin)
LIBS= -framework UpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation
LIBDIR= -LThirdParty/lib/OSX
endif

LIBS+= -lGLEW -lglfw3 -lSOIL -lpthread

debug: CXXFLAGS+= $(DEBUG_FLAGS)
debug: $(SOURCES) $(EXECUTABLE)

release: CXXFLAGS+= -O3
release: $(SOURCES) $(EXECUTABLE)

tests: EXECUTABLE= $(TEST_EXECUTABLE)	
tests: debug

all: debug

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LIBDIR) $(LIBS) $(LIBS) $(OBJECTS) -o $@ $(LIBS) $(LIBS)

%.o: %.ii
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

%.ii: %.d
	$(PREPROCESS) -x c++ $(CXXFLAGS) $*.cpp $(INCLUDE) | tee $*.exp | $(EXPANDER) $(INCLUDE) "/dev/stdin" > $@

%.d:%.cpp
#echo -n "$*.d ">$*.d
	$(CXX) $(CXXFLAGS) $(INCLUDE) -MM -MT $*.d $*.cpp > $*.d

-include $(DEPENDS)

clean:
	@rm -f $(OBJECTS) $(EXPANSIONS) $(EXECUTABLE) $(DEPENDS)

.SUFFIXES:
.SECONDARY:
