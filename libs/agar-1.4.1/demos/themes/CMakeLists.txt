cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)

#Configuration du projet
project(Themes)
set(CMAKE_BUILD_TYPE Debug)

include_directories(".")
include_directories("../../../SDL-1.2.14-MinGW/include/SDL")
include_directories("../../../agar-1.4.1-mingw32-sdl-debug-pthread/include")

link_directories("../../../SDL-1.2.14-MinGW/lib")
link_directories("../../../agar-1.4.1-mingw32-sdl-debug-pthread/libs")
link_directories("../../../Pre-built.2/lib")
link_directories("../../../freetype-2.3.5-1-bin/lib")

# Sélection des fichiers source
file(
	GLOB_RECURSE
	source_files
	*.c
)

# Configuration de l'exécutable
add_executable(
	Themes
	${source_files}
)

target_link_libraries(
    Themes
    mingw32
    ag_gui
    ag_core
	ag_dev
    SDL
    SDL.dll
    SDLmain
    mingw32
    winmm
    freetype
    opengl32
    glu32
    pthreadGC2
    mingwex
)
