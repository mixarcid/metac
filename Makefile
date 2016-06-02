rwildcard= $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

CXX= clang++
CXXFLAGS= -Wall -Werror -std=c++1y -fno-rtti -Wunknown-pragmas
DEBUG_FLAGS= -rdynamic -ggdb -Wno-error=unused -D DEBUG
RELEASE_FLAGS= -Ofast -D NDEBUG
INCLUDE= $(shell find Source -type d | sed 's/[^\ ]*[^\ ]/-I&/g')
INCLUDE+= -IThirdParty/include
SOURCES= $(call rwildcard, Source/, *.cpp)
OUTDIR= Bin/
EXECUTABLE= $(OUTDIR)metac

OBJECTS=$(SOURCES:.cpp=.o)
DEPENDS=$(SOURCES:.cpp=.d)

ifeq ($(CXX), clang++)
CXXFLAGS+= -Wno-unused-command-line-argument
endif

all: debug

debug: CXXFLAGS += $(DEBUG_FLAGS)
release: CXXFLAGS += $(RELEASE_FLAGS)

debug: $(EXECUTABLE)
release: $(EXECUTABLE)

%.d: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -MM -MT $*.d $*.cpp > $*.d

%.o: %.d
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $*.o $*.cpp

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LIBDIR) $(LIBS) $(LIBS) $(OBJECTS) -o $@ $(LIBS) $(LIBS)

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif

clean:
	@rm -f $(OBJECTS) $(EXECUTABLE) $(DEPENDS)

.SUFFIXES:
.SECONDARY:
