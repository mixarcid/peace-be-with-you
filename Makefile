rwildcard= $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
UNAME= $(shell uname)

ifeq ($(UNAME), Darwin)
CXX= clang++
else
CXX= g++
endif

PREPROCESS= cpp
EXPANDER= pyexpander/expander.py --eval "makefile_dir=\"$(shell pwd)\""
CXXFLAGS= -Wall -std=c++1y -fno-rtti -Wunknown-pragmas
DEBUG_FLAGS= -rdynamic -ggdb -Wno-error=unused -D DEBUG
RELEASE_FLAGS= -Ofast -D NDEBUG
INCLUDE= $(shell find Source -type d | sed 's/[^\ ]*[^\ ]/-I&/g')
INCLUDE+= -IThirdParty/include
SOURCES= $(call rwildcard, Source/, *.cpp)
OUTDIR= bin/

GOALS = $(MAKECMDGOALS)
ifeq ($(MAKECMDGOALD), all)
GOALS = debug
endif

ifeq ($(GOALS), tests)
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
LIBS= -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation -framework CoreMIDI -framework CoreAudio -framework AudioToolbox -framework Accelerate
LIBDIR= -LThirdParty/lib/OSX -Wl,-rpath,ThirdParty/lib/OSX
endif

LIBS+= -lGLEW -lglfw3 -lSOIL -lpthread -lyse

ifeq ($(CXX), clang++)
CXXFLAGS+= -Wno-unused-command-line-argument
endif

all: debug

debug: pre-debug
release: pre-release

pre-debug: CXXFLAGS += $(DEBUG_FLAGS)
pre-debug:
	@echo about to compile debug
	$(MAKE) main-build CXXFLAGS="$(CXXFLAGS)"

pre-release: CXXFLAGS += $(RELEASE_FLAGS)
pre-release:
	@echo about to compile release
	$(MAKE) main-build CXXFLAGS="$(CXXFLAGS)"

main-build: $(EXECUTABLE)

ifeq ($(GOALS), main-build)
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
endif

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif

clean:
	@rm -f $(OBJECTS) $(EXPANSIONS) $(EXECUTABLE) $(DEPENDS) $(PYEXPANSIONS)

log:
	@echo $(SOURCES)

.SUFFIXES:
.SECONDARY:
