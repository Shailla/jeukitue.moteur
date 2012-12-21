cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)

#Configuration du projet
project(BasASableAgar)
set(CMAKE_BUILD_TYPE Debug)

IF(MSVC)
	set(EXECUTABLE_OUTPUT_PATH ../../deploy)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
	set(EXECUTABLE_OUTPUT_PATH ../../deploy/${CMAKE_BUILD_TYPE}-g++)
	set(CMAKE_CXX_FLAGS "-ggdb -Wall -Wno-unknown-pragmas -Wunused-variable")
ENDIF(MSVC)

IF(MSVC)
	include_directories("src")
	include_directories("../../libs/SDL-1.2.14/include")
	include_directories("../../libs/agar-1.4.1-win32-i386/include")
	include_directories("../../libs/glFont")
	include_directories("../../libs/glext")
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
	include_directories("src")
	include_directories("../../libs/SDL-1.2.14-MinGW/include/SDL")
	include_directories("../../libs/agar-1.4.1-mingw32/usr/local/include")
	include_directories("../../libs/glFont")
	include_directories("../../libs/glext")
ENDIF(MSVC)

IF(MSVC)
	link_directories("../../libs/SDL-1.2.14/lib")
	link_directories("../../libs/agar-1.4.1-win32-i386/lib")
	link_directories("../../libs/pthreads-w32-2-9-1-release/Pre-built.2/lib/x86")
	link_directories("../../libs/freetype-2.3.5-1-bin/lib")
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
	link_directories("../../libs/SDL-1.2.14-MinGW/lib")
	link_directories("../../libs/agar-1.4.1-mingw32/usr/local/lib")
	link_directories("../../libs/pthreads-w32-2-9-1-release/compiled-with-GCE-inlined-debug")
	link_directories("../../libs/freetype-2.3.5-1-bin/lib")
ENDIF(MSVC) 

# Sélection des fichiers source
file(
	GLOB_RECURSE
	source_files
	*.cpp
)

# Configuration de l'exécutable
add_executable(
	BasASableAgar
	${source_files}
)

IF(MSVC)
	target_link_libraries(
		BasASableAgar
		ag_gui ag_core ag_dev SDL SDL.dll SDLmain winmm freetype opengl32 glu32 pthreadVC2
	)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries(
		BasASableAgar
    	mingw32 ag_gui ag_core ag_dev SDL SDL.dll SDLmain mingw32 winmm freetype opengl32 glu32 pthreadGC2 mingwex
	)
ENDIF(MSVC)
