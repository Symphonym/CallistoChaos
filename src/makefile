# Project name goes in the PROJ_NAME variable, it has to be passed
# to the makefile externally

# Default compiler flags and options
CXX := g++
CXXFLAGS := -Iinclude -std=c++11 -Wall

CXX_DEBUG_FLAGS := -Wl,-R,'$$ORIGIN/lib' -L../debug/lib	\
	-lsfml-audio-d -lsfml-graphics-d -lsfml-window-d -lsfml-network-d -lsfml-system-d

CXX_RELEASE_FLAGS := -Wl,-R,'$$ORIGIN/lib' -L../release/lib	\
	-lsfml-audio -lsfml-graphics -lsfml-window -lsfml-network -lsfml-system

# List of source files
SRCS := $(wildcard *.cpp)
SRCS_OBJECT := ${SRCS:.cpp=.o}

# release target, for maximum performance
.PHONY: release<<
release: ${SRCS_OBJECT}
	${CXX} -o ../release/${PROJ_NAME} ${SRCS_OBJECT} ${CXX_RELEASE_FLAGS}

# debug target, uses debug symbols when compiling
.PHONY: debug
debug: CXXFLAGS += -g
debug: ${SRCS_OBJECT}
	${CXX} -o ../debug/${PROJ_NAME} ${SRCS_OBJECT} ${CXX_DEBUG_FLAGS}

# clean target, cleans the directory from object (.o) files
.PHONY: clean
clean:
	rm *.o


# For effective, fast and correct compiling(not skipping files):
# specify each .h file that each .o file is dependent on in the
# form of:
# objectfile.o: headerfile.h

main.o: GameState.h Engine.h
Engine.o: Settings.h Utility.h AssetManager.h StateManager.h SoundManager.h
GameState.o: Engine.h FrameAnimation.h State.h SoundManager.h
AssetManager.o: Asset.h TextureAsset.h FontAsset.h SoundBufferAsset.h MusicAsset.h
TextureAsset.o: Asset.h
FontAsset.o: Asset.h
SoundBufferAsset.o: Asset.h
MusicAsset.o: Asset.h
SoundManager.o: AssetManager.h
FrameAnimation.o: SoundManager.h
StateManager.o: State.h
State.o: StopWatch.h Engine.h
TileMap.o: Tile.h
Tile.o: TileCharacter.h
TileCharacter: FrameAnimation.h TileMap.h

