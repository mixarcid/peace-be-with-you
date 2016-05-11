rwildcard= $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
UNAME= $(shell uname)

ifeq ($(UNAME), Darwin)
CXX= clang++
else
CXX= g++
endif

PREPROCESS= cpp
EXPANDER= pyexpander/expander.py --eval "makefile_dir=\"$(shell pwd)\""
CXXFLAGS= -Wall -std=c++1y -fno-rtti
DEBUG_FLAGS= -rdynamic -ggdb -Wno-error=unused
RELEASE_FLAGS= -Ofast
INCLUDE= $(shell find Source -type d | sed 's/[^\ ]*[^\ ]/-I&/g')
INCLUDE+= -IThirdParty/include
SOURCES= $(call rwildcard, Source/, *.cpp)
OUTDIR= bin/

ifeq ($(MAKECMDGOALS), tests)
SOURCES:= $(filter-out Source/main.cpp, $(SOURCES))
SOURCES+= $(wildcard Tests/*.cpp)
INCLUDE+= -ITests
EXECUTABLE = $(OUTDIR)test
else
EXECUTABLE= $(OUTDIR)peace
endif

EXPANSIONS=$(SOURCES:.cpp=.ii)
PYEXPANSIONS=$(SOURCES:.cpp=.exp)
OBJECTS=$(SOURCES:.cpp=.o)

DEPENDS=$(SOURCES:.cpp=.d)

ifeq ($(UNAME), Linux)
LIBS= -lX11 -lXxf86vm -lXcursor -lXinerama -lXrandr -lXext -lXi -lGL
LIBDIR= -LThirdParty/lib/Linux
endif
ifeq ($(UNAME), Darwin)
LIBS= -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation
LIBDIR= -LThirdParty/lib/OSX
endif

LIBS+= -lGLEW -lglfw3 -lSOIL -lpthread

ifeq ($(CXX), clang++)
CXXFLAGS+= -Wno-unused-command-line-argument
endif

debug: CXXFLAGS+= $(DEBUG_FLAGS)
debug: $(SOURCES) $(EXECUTABLE)

release: CXXFLAGS+= $(RELEASE_FLAGS)
release: $(SOURCES) $(EXECUTABLE)

tests: debug
all: debug

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LIBDIR) $(LIBS) $(LIBS) $(OBJECTS) -o $@ $(LIBS) $(LIBS)

%.o: %.ii
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

%.ii: %.exp
	$(EXPANDER) $(INCLUDE) -f $*.exp > $*.ii

%.exp: %.d
	$(CXX) -E $(CXXFLAGS) $*.cpp $(INCLUDE) | ./pyexpander-prepare.py > $*.exp

%.d: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -MM -MT $*.d $*.cpp > $*.d

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif

clean:
	@rm -f $(OBJECTS) $(EXPANSIONS) $(EXECUTABLE) $(DEPENDS) $(PYEXPANSIONS)

.SUFFIXES:
.SECONDARY:
