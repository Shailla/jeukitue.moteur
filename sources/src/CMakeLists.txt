cmake_minimum_required(VERSION 2.6)
		
#Configuration du projet
project(jkt)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

include_directories(.)
include_directories("../Libs/SDL-1.2.14-MinGW/include/SDL")
include_directories("../Libs/SDL_net-1.2.7/include")
include_directories("../Libs/SDL_image-1.2.10/include")
include_directories("../Libs/agar-1.3.4-recompile/include")
include_directories("../Libs/ziparchive/ZipArchive")
include_directories("../Libs/fmodapi375win/api/inc")
include_directories("../Libs/glFont")
include_directories("../Libs/glext")
include_directories("../Libs/tinyxml_2_6_1")

link_directories("H:/jkt/svn/trunk/moteur_cmake/Libs/SDL-1.2.14-MinGW/lib")
link_directories("H:/jkt/svn/trunk/moteur_cmake/Libs/SDL_net-1.2.7/lib")
link_directories("H:/jkt/svn/trunk/moteur_cmake/Libs/SDL_image-1.2.10/lib")
link_directories("H:/jkt/svn/trunk/moteur_cmake/Libs/agar-1.3.4-recompile/lib")
link_directories("H:/jkt/svn/trunk/moteur_cmake/Libs/ziparchive/ZipArchive")
link_directories("H:/jkt/svn/trunk/moteur_cmake/Libs/fmodapi375win/api/lib")
link_directories("H:/jkt/svn/trunk/moteur_cmake/Libs/Pre-built.2/lib")
link_directories("H:/jkt/svn/trunk/moteur_cmake/Libs/freetype-2.3.5-1-bin/lib")

# Sélection des fichiers source
file(
	GLOB_RECURSE
	source_files
	*.cpp
	../Libs/tinyxml_2_6_1/tinyxml.cpp
	../Libs/tinyxml_2_6_1/tinystr.cpp
	../Libs/tinyxml_2_6_1/tinyxmlerror.cpp
	../Libs/tinyxml_2_6_1/tinyxmlparser.cpp
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



