SRC_ROOT = ../src
SFML_ROOT = /home/stephen/Downloads/SFML-2.3.2-source/build64Win/install
CXX = x86_64-w64-mingw32-g++
CXXFLAGS = -c -std=c++11 -Wall -Wextra -O3 -D NDEBUG \
           -I${SFML_ROOT}/include \
           -I${SRC_ROOT} \
           -mwindows
CXXDEBUGFLAGS = -c -std=c++11 -Wall -Wextra -O0 -g -D_GLIBCXX_DEBUG \
           -I${SFML_ROOT}/include \
           -I${SRC_ROOT} \
           -mwindows
LINKER_FLAGS = -mwindows -Wall
LINKER_LIBS = ${SFML_ROOT}/bin/sfml-audio-2.dll \
              ${SFML_ROOT}/bin/sfml-network-2.dll \
              ${SFML_ROOT}/bin/sfml-graphics-2.dll \
              ${SFML_ROOT}/bin/sfml-window-2.dll \
              ${SFML_ROOT}/bin/sfml-system-2.dll

SFML_GameEngine_SOURCES =  \
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

all:    libSFML_GameEngine.so.1.0 NetworkingTest.exe

libSFML_GameEngine.so.1.0: ${SFML_GameEngine_SOURCES}
		$(CXX) $(LINKER_FLAGS) -shared -Wl,-soname,libSFML_GameEngine.so.1 -o libSFML_GameEngine.so.1.0 ${LINKER_LIBS} ${SFML_GameEngine_SOURCES}

NetworkingTest.exe: ${NetworkingTest_SOURCES}
		$(CXX) $(LINKER_FLAGS) -o NetworkingTest.exe ${LINKER_LIBS} ${NetworkingTest_SOURCES}

clean:	../src.clean ../src/engine.clean ../src/engine/ec.clean ../NetworkingTest.clean
		rm -f libSFML_GameEngine.so.1.0
		rm -f NetworkingTest.exe

objectOnlyClean:	../src.clean ../src/engine.clean ../src/engine/ec.clean ../NetworkingTest.clean


%.clean:
		rm -f $*/*.o

%.o:		%.cpp
ifeq (${DEBUG},1)
		$(CXX) $(CXXDEBUGFLAGS) -o $*.o $*.cpp
else
		$(CXX) $(CXXFLAGS) -o $*.o $*.cpp
endif
