CXX = g++
CXXFLAGS = -O2 -ggdb -DREMOVE_HH_ZCTAS
LDFLAGS = -lgmp -lgmpxx -ggdb

SHAREDSRC := $(wildcard algorithm/*.cpp math/*.cpp memory/*.cpp)
MAINSRC := Main.cpp io/Printer.cpp
TRANSLATESRC := Translate.cpp io/Parser.cpp
SOURCES := $(SHAREDSRC) $(MAINSRC) $(TRANSLATESRC)

SHAREDOBJ = $(SHAREDSRC:.cpp=.o)
MAINOBJ = $(MAINSRC:.cpp=.o)
TRANSLATEOBJ = $(TRANSLATESRC:.cpp=.o)
OBJECTS := $(SHAREDOBJ) $(MAINOBJ) $(TRANSLATEOBJ)

all: zcta.exe translate.exe

zcta.exe: $(SHAREDOBJ) $(MAINOBJ)
		$(CXX) $(SHAREDOBJ) $(MAINOBJ) $(LDFLAGS) -o zcta.exe
		
translate.exe: $(SHAREDOBJ) $(TRANSLATEOBJ)
		$(CXX) $(SHAREDOBJ) $(TRANSLATEOBJ) $(LDFLAGS) -lboost_regex -o translate.exe

%.d: %.cpp
		@set -e; rm -f $@; \
		$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

include $(SOURCES:.cpp=.d)

.PHONY: clean

clean:
		rm -f $(OBJECTS) $(SOURCES:.cpp=.d) zcta.exe translate.exe
