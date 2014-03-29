CXX = g++ -W -Wall -Wextra -pedantic -std=c++0x
CFLAGS = -c -g
INCLUDE = -I/home/stephen/SFML2/include/
LINK = -L/home/stephen/SFML2/shared/ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
BUILDDIR = build
BINDIR = bin
SRCDIR = src
SOURCES = \
		main.o \
        game.o \
        sceneNode.o \
        stateStack.o \
        state.o \
        musicPlayer.o \
        soundPlayer.o

all: $(BUILDDIR) $(BINDIR) $(BINDIR)/2DPrimer

test: tests/UnitTests

$(BUILDDIR):
		mkdir -p $(BUILDDIR)

$(BINDIR):
		mkdir -p $(BINDIR)

$(BINDIR)/2DPrimer: $(addprefix $(BUILDDIR)/,$(SOURCES))
		$(CXX) -o $(BINDIR)/2DPrimer $(addprefix $(BUILDDIR)/,$(SOURCES)) $(LINK) 

tests/UnitTests: $(addprefix $(BUILDDIR)/,$(SOURCES)) $(BUILDDIR)/unitTests.o
		$(CXX) -o tests/UnitTests $(addprefix $(BUILDDIR)/,$(SOURCES)) build/UnitTests.o $(LINK)

clean:		$(BUILDDIR).clean tests.clean
		rm -f $(BINDIR)/2DPrimer

%.clean:
		rm -f $*/*.o

%.o:		$(%.cpp:$(BUILDDIR)/=$(SRCDIR)/)
		$(CXX) $(INCLUDE) $(CFLAGS) -o $@ $(subst $(BUILDDIR)/,$(SRCDIR)/,$*.cpp)
