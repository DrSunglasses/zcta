CXX = g++
CXXFLAGS = -O2 -ggdb -std=c++0x 
LDFLAGS = -lboost_regex -lgmp -lgmpxx -ggdb

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
