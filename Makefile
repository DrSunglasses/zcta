CXX = g++
CXXFLAGS = -O2 -gdwarf-2 -g -std=c++0x -DMOVE
LDFLAGS = -lboost_regex-gcc43-mt -lgmp -lgmpxx -gdwarf-2 -g

SOURCES := $(wildcard *.cpp algorithm/*.cpp io/*.cpp math/*.cpp memory/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

zcta.exe: $(OBJECTS)
		$(CXX) $(OBJECTS) $(LDFLAGS) -o zcta.exe

%.d: %.cpp
		@set -e; rm -f $@; \
		$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

include $(SOURCES:.cpp=.d)

.PHONY: clean

clean:
		rm -f $(OBJECTS) $(SOURCES:.cpp=.d) zcta.exe
