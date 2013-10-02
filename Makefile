CXX = g++ -W -Wall -Wextra -pedantic -std=c++0x
CFLAGS = -c -g
INCLUDE = -I/home/stephen/Downloads/SFML-latestish/include
LINK = -L/home/stephen/Downloads/SFML-latestish/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
BINDIR = bin
SOURCES = \
        src/game.o \
        src/sceneNode.o \
        src/stateStack.o \
        src/state.o

all: $(BINDIR)/2DPrimer

test: tests/UnitTests

$(BINDIR)/2DPrimer: $(SOURCES) src/main.o
		$(CXX) -o $(BINDIR)/2DPrimer $(SOURCES) src/main.o $(LINK) 

tests/UnitTests: $(SOURCES) src/unitTests.o
		$(CXX) -o tests/UnitTests $(SOURCES) src/UnitTests.o $(LINK)

clean:		src.clean tests.clean
		rm -f $(BINDIR)/2DPrimer

%.clean:
		rm -f $*/*.o

%.o:		%.cpp
		$(CXX) $(INCLUDE) $(CFLAGS) -o $*.o $*.cpp
