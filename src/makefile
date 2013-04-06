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

.PHONY: JLautodep
JLautodep:
	./JLautodep -i ${SRCS} -m makefile -E "<" --empty-targets --self-dependency


# For effective, fast and correct compiling(not skipping files):
# specify each .h file that each .o file is dependent on in the
# form of:
# objectfile.o: headerfile.h

#JLautodep START
main.o: GameState.h Engine.h
WorkbenchFunctions.o: WorkbenchFunctions.h Weapon.h Player.h MessageLog.h
Workbench.o: Weapon.h AssetManager.h WorkbenchFunctions.h Workbench.h Utility.h GunWeapon.h RifleWeapon.h Player.h Settings.h MessageLog.h
Weapon.o: AssetManager.h FrameAnimation.h Weapon.h TileMap.h TileCharacter.h Utility.h MessageLog.h Player.h
WeakEnemy.o: TileCharacter.h WeakEnemy.h AstarAlgorithm.h GameState.h
TileOptionManager.o: Player.h AssetManager.h TileOptionManager.h TileMap.h Utility.h Settings.h
TileOptionActions.o: TileOptionActions.h TileMap.h MessageLog.h TileOptionManager.h Utility.h GameState.h
TileMap.o: Tile.h TileMap.h
TileCharacter.o: FrameAnimation.h AssetManager.h TileCharacter.h TileMap.h GameState.h
Tile.o: TileCharacter.h Tile.h
TextureAsset.o: Asset.h TextureAsset.h
StopWatch.o: StopWatch.h
StateManager.o: State.h StateManager.h
State.o: StopWatch.h State.h Engine.h
SoundManager.o: SoundManager.h AssetManager.h
SoundBufferAsset.o: Asset.h SoundBufferAsset.h
Settings.o: Settings.h
RifleWeapon.o: Weapon.h RifleWeapon.h TileMap.h
Player.o: TileCharacter.h Weapon.h Player.h TileMap.h MessageLog.h Utility.h GunWeapon.h RifleWeapon.h Settings.h GameState.h
MusicAsset.o: Asset.h MusicAsset.h
MessageLog.o: AssetManager.h MessageLog.h Settings.h
LootManager.o: AssetManager.h LootManager.h Utility.h Player.h TileMap.h
GunWeapon.o: Weapon.h GunWeapon.h TileMap.h
GameState.o: Engine.h TileMap.h State.h CharacterManager.h TileOptionManager.h Workbench.h BedControl.h LootManager.h GameState.h Player.h MessageLog.h TileOptionActions.h WeakEnemy.h Utility.h
FrameAnimation.o: FrameAnimation.h SoundManager.h
FontAsset.o: Asset.h FontAsset.h
Engine.o: AssetManager.h StateManager.h Settings.h Engine.h Utility.h SoundManager.h
CharacterManager.o: TileCharacter.h Player.h CharacterManager.h TileMap.h GameState.h
BedControl.o: BedControl.h Player.h TileMap.h MessageLog.h
AstarAlgorithm.o: AstarAlgorithm.h TileMap.h
AssetManager.o: Asset.h TextureAsset.h FontAsset.h SoundBufferAsset.h MusicAsset.h AssetManager.h
#JLautodep END
