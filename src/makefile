# Project name goes in the PROJ_NAME variable, it has to be passed
# to the makefile externally

# Default compiler flags and options
CXX := g++
CXXFLAGS := -Iinclude -std=c++11 -Wall

CXX_DEBUG_FLAGS := -Wl,-R,'$$ORIGIN/lib' -L../debug/lib	\
	-lsfml-graphics-d -lsfml-audio-d -lsfml-window-d -lsfml-network-d -lsfml-system-d

CXX_RELEASE_FLAGS := -Wl,-R,'$$ORIGIN/lib' -L../release/lib	\
	-lsfml-graphics -lsfml-audio -lsfml-window -lsfml-network -lsfml-system

# List of source files
SRCS := $(wildcard *.cpp)
SRCS_OBJECT := ${SRCS:.cpp=.o}

# release target, for maximum performance
.PHONY: release
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

main.o: GameEngine.h Locator.h
Engine.o: Settings.h Locator.h State.h
GameEngine.o: Engine.h
Locator.o: Settings.h
