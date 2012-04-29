cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)

#Configuration du projet
project(JeuKiTue)
set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_OUTPUT_PATH ../../deploy/${CMAKE_BUILD_TYPE})

include_directories("src")
include_directories("../libs/SDL-1.2.14-MinGW/include/SDL")
include_directories("../libs/SDL_net-1.2.7/include")
include_directories("../libs/SDL_image-1.2.10/include")
include_directories("../libs/agar-1.3.4-recompile/include")
#include_directories("../libs/agar-1.4.1-win32-i386/Agar-1.4.1/include")
include_directories("../libs/ziparchive/ZipArchive")
include_directories("../libs/fmodapi375win/api/inc")
include_directories("../libs/glFont")
include_directories("../libs/glext")
include_directories("../libs/tinyxml_2_6_1")

link_directories("../libs/SDL-1.2.14-MinGW/lib")
link_directories("../libs/SDL_net-1.2.7/lib")
link_directories("../libs/SDL_image-1.2.10/lib")
link_directories("../libs/agar-1.3.4-recompile/lib")
#link_directories("../libs/agar-1.4.1-win32-i386-Agar-1.4.1/lib")
link_directories("../libs/ziparchive/ZipArchive")
link_directories("../libs/fmodapi375win/api/lib")
link_directories("../libs/Pre-built.2/lib")
link_directories("../libs/freetype-2.3.5-1-bin/lib")

# Sélection des fichiers source
file(
	GLOB_RECURSE
	source_files
	src/*.cpp
	../libs/tinyxml_2_6_1/tinyxml.cpp
	../libs/tinyxml_2_6_1/tinystr.cpp
	../libs/tinyxml_2_6_1/tinyxmlerror.cpp
	../libs/tinyxml_2_6_1/tinyxmlparser.cpp
)

# Configuration de l'exécutable
add_executable(
	jkt
	${source_files}
)

target_link_libraries(
    jkt
    mingw32
    ag_gui_static
    ag_core_static
    SDLmain
    SDL
    SDL.dll
    SDL_net
    SDL_image
    ziparch
    mingw32
    winmm
    freetype
    opengl32
    glu32
    fmodvc
    pthreadGC2
    mingwex
)
