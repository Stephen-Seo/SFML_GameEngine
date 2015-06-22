SRC_ROOT = ../src
SFML_ROOT = /home/stephen/Downloads/SFML-2.3-Source/buildWindows/install
CXX = x86_64-w64-mingw32-g++
CXXFLAGS = -c -std=c++11 -Wall -Wextra -O3 -D NDEBUG \
           -I${SFML_ROOT}/include \
           -I${SRC_ROOT} \
           -mwindows
CXXDEBUGFLAGS = -c -std=c++11 -Wall -Wextra -O0 -g -D_GLIBCXX_DEBUG \
           -I${SFML_ROOT}/include \
           -I${SRC_ROOT} \
           -mwindows
LINKER_FLAGS = -mwindows
LINKER_LIBS = ${SFML_ROOT}/bin/sfml-audio-2.dll \
              ${SFML_ROOT}/bin/sfml-network-2.dll \
              ${SFML_ROOT}/bin/sfml-graphics-2.dll \
              ${SFML_ROOT}/bin/sfml-window-2.dll \
              ${SFML_ROOT}/bin/sfml-system-2.dll

Game_Project_SOURCES =  \
          ${SRC_ROOT}/engine/animatedSprite.o \
          ${SRC_ROOT}/engine/connection.o \
          ${SRC_ROOT}/engine/context.o \
          ${SRC_ROOT}/engine/game.o \
          ${SRC_ROOT}/engine/gui.o \
          ${SRC_ROOT}/engine/musicPlayer.o \
          ${SRC_ROOT}/engine/networkIdentifiers.o \
          ${SRC_ROOT}/engine/resourceIdentifiers.o \
          ${SRC_ROOT}/engine/resourceManager.o \
          ${SRC_ROOT}/engine/sceneNode.o \
          ${SRC_ROOT}/engine/soundPlayer.o \
          ${SRC_ROOT}/engine/state.o \
          ${SRC_ROOT}/engine/stateStack.o \
          ${SRC_ROOT}/engine/utility.o \
          ${SRC_ROOT}/engine/ec/engine.o \
          ${SRC_ROOT}/engine/ec/entity.o \
          ${SRC_ROOT}/engine/ec/nMove.o \
          ${SRC_ROOT}/engine/ec/system.o

NetworkingTest_SOURCES = ${SRC_ROOT}/../NetworkingTest/Main.o \
          ${SRC_ROOT}/engine/connection.o \
          ${SRC_ROOT}/engine/networkIdentifiers.o

all:    Game_Project NetworkingTest

Game_Project: ${Game_Project_SOURCES} ${SRC_ROOT}/main.o
		$(CXX) $(LINKER_FLAGS) -o Game_Project.exe ${LINKER_LIBS} ${Game_Project_SOURCES} ${SRC_ROOT}/main.o
		touch Game_Project

NetworkingTest: ${NetworkingTest_SOURCES}
		$(CXX) $(LINKER_FLAGS) -o NetworkingTest.exe ${LINKER_LIBS} ${NetworkingTest_SOURCES}
		touch NetworkingTest

clean:	../src.clean ../src/engine.clean ../src/engine/ec.clean ../NetworkingTest.clean
		rm -f Game_Project.exe
		rm -f Game_Project
		rm -f NetworkingTest.exe
		rm -f NetworkingTest

%.clean:
		rm -f $*/*.o

%.o:		%.cpp
ifeq (${DEBUG},1)
		$(CXX) $(CXXDEBUGFLAGS) -o $*.o $*.cpp
else
		$(CXX) $(CXXFLAGS) -o $*.o $*.cpp
endif
