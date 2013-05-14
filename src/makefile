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
.PHONY: release
release: ${SRCS_OBJECT}
	${CXX} -o ../release/${PROJECT_NAME} ${SRCS_OBJECT} ${CXX_RELEASE_FLAGS} ${CXXFLAGS} -Wall

# debug target, uses debug symbols when compiling
.PHONY: debug
debug: CXXFLAGS += -g
debug: ${SRCS_OBJECT}
	${CXX} -o ../debug/${PROJECT_NAME} ${SRCS_OBJECT} ${CXX_DEBUG_FLAGS} ${CXXFLAGS} -Wall

# execute debug target, executes the debug configuration in gdb
.PHONY: exedebug
exedebug:
	xterm -geometry 121x50 -T "GDB Debugging - ${PROJECT_NAME}" -n "GDB Debug-${PROJECT_NAME}" -e "gdb ../debug/${PROJECT_NAME} -tui"

# execute release target, executes the release configuration
.PHONY: exerelease
exerelease:
	../release/${PROJECT_NAME}

# automatic dependency generation target, executes the JLautodep generator
.PHONY: autodep
autodep:
	./JLautodep -i ${SRCS} -m makefile -E "<" --empty-targets --self-dependency

# clean target, cleans the directory from object (.o) files
.PHONY: clean
clean:
	rm *.o

# For effective, fast and correct compiling(not skipping files):
# specify each .h file that each .o file is dependent on in the
# form of:
# objectfile.o: headerfile.h

#JLautodep START
main.o: MainMenuState.h Engine.h
WorkbenchFunctions.o: WorkbenchFunctions.h Weapon.h Player.h MessageLog.h
Workbench.o: AssetManager.h WorkbenchFunctions.h Workbench.h Utility.h Weapon.h GunWeapon.h RifleWeapon.h RailGun.h Player.h Settings.h MessageLog.h SoundManager.h
Weapon.o: AssetManager.h FrameAnimation.h Weapon.h TileMap.h TileCharacter.h Utility.h MessageLog.h Player.h ParticleManager.h SoundManager.h
WeakEnemy.o: TileCharacter.h WeakEnemy.h AstarAlgorithm.h GameState.h ParticleManager.h Utility.h
TileOptionManager.o: Player.h AssetManager.h TileOptionManager.h TileMap.h Utility.h Settings.h SoundManager.h
TileOptionActions.o: TileOptionActions.h TileMap.h MessageLog.h TileOptionManager.h Utility.h GameState.h
TileMap.o: Tile.h TileMap.h
TileCharacter.o: FrameAnimation.h AssetManager.h TileCharacter.h TileMap.h GameState.h
Tile.o: TileCharacter.h Tile.h Tile.h ParticleManager.h Utility.h TileMap.h SoundManager.h
StopWatch.o: StopWatch.h
StateManager.o: State.h StateManager.h
State.o: StopWatch.h State.h Engine.h
SoundManager.o: SoundManager.h AssetManager.h
SettingsConsole.o: SettingsConsole.h Utility.h GameState.h
Settings.o: Settings.h
RifleWeapon.o: Weapon.h RifleWeapon.h TileMap.h
RailGun.o: Weapon.h RailGun.h TileCharacter.h TileMap.h Utility.h SoundManager.h
Player.o: TileCharacter.h Weapon.h Player.h TileMap.h MessageLog.h Utility.h GunWeapon.h RifleWeapon.h Settings.h GameState.h SoundManager.h
ParticleManager.o: ParticleManager.h Utility.h
MessageLog.o: AssetManager.h MessageLog.h Settings.h
MainMenuState.o: State.h MainMenuState.h Utility.h Engine.h GameState.h ParticleManager.h GalaxyGenerator.h SoundManager.h Settings.h
LootManager.o: AssetManager.h LootManager.h Utility.h Player.h TileMap.h SoundManager.h
GunWeapon.o: Weapon.h GunWeapon.h TileMap.h
GameState.o: Engine.h TileMap.h State.h CharacterManager.h TileOptionManager.h Workbench.h BedControl.h LootManager.h EnemyWaveManager.h GameState.h Player.h MessageLog.h TileOptionActions.h WeakEnemy.h Utility.h ParticleManager.h SettingsConsole.h GalaxyGenerator.h SoundManager.h
GalaxyGenerator.o: GalaxyGenerator.h Utility.h
FrameAnimation.o: FrameAnimation.h SoundManager.h
Engine.o: AssetManager.h StateManager.h Settings.h Engine.h Utility.h SoundManager.h
EnemyWaveManager.o: AssetManager.h EnemyWaveManager.h Settings.h CharacterManager.h WeakEnemy.h TileMap.h Utility.h GameState.h
CharacterManager.o: TileCharacter.h Player.h CharacterManager.h TileMap.h GameState.h
BedControl.o: BedControl.h Player.h TileMap.h MessageLog.h TileOptionActions.h GameState.h
AstarAlgorithm.o: AstarAlgorithm.h TileMap.h Player.h
AssetManager.o: AssetManager.h
XboxInput.o: Utility.h
#JLautodep END
