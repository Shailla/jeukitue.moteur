cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)

#Configuration du projet
project(JeuKiTue)
set(CMAKE_BUILD_TYPE Debug)

IF(MSVC)
	set(EXECUTABLE_OUTPUT_PATH ../../deploy)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
	set(EXECUTABLE_OUTPUT_PATH ../../deploy/${CMAKE_BUILD_TYPE}-g++)
#	set(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++ -ggdb -Wall -Wno-unknown-pragmas -std=gnu++0x -fpermissive")
	set(CMAKE_CXX_FLAGS "-ggdb -Wall -Wno-unknown-pragmas -std=gnu++0x -fpermissive")
ENDIF(MSVC)

find_package(SDL REQUIRED)
find_package(SDL_net REQUIRED)

include_directories("src")
include_directories(${SDL_INCLUDE_DIR})
include_directories(${SDL_NET_INCLUDE_DIR})
include_directories("../libs/tinyxml_2_6_1")
include_directories("../libs/glFont")
include_directories("../libs/fmodapi375win/api/inc")

link_directories("../libs/fmodapi375win/api/lib")

# Sélection des fichiers source
file(
	GLOB_RECURSE SOURCE_FILES
	src/*.cpp
	../libs/tinyxml_2_6_1/tinyxml.cpp
	../libs/tinyxml_2_6_1/tinystr.cpp
	../libs/tinyxml_2_6_1/tinyxmlerror.cpp
	../libs/tinyxml_2_6_1/tinyxmlparser.cpp
	#../libs/nvwa-0.9/nvwa/debug_new.cpp
)

file(
	GLOB LIB_AGAR_SRC
	D:/jkt/moteur/libs/agar-trunk/core/*.c
	D:/jkt/moteur/libs/agar-trunk/dev/*.c
	D:/jkt/moteur/libs/agar-trunk/math/*.c
	D:/jkt/moteur/libs/agar-trunk/gui/*.c
)

set_source_files_properties(${LIB_AGAR_SRC} PROPERTIES HEADER_FILE_ONLY TRUE)

# Configuration de l'exécutable
add_executable(
	jkt
	${SOURCE_FILES}
	${LIB_AGAR_SRC}
)

IF(CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries(
		jkt
    	mingw32 ag_core ag_gui ag_dev ${SDL_LIBRARY} ${SDL_NET_LIBRARY} SDL_image ziparch mingw32 winmm freetype opengl32 glu32 fmod64vc pthread mingwex lua jpeg png
	)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
