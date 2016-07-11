cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)

#Configuration du projet
project(JeuKiTue)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(EXECUTABLE_OUTPUT_PATH ../../deploy/${CMAKE_BUILD_TYPE}-g++)
set(CMAKE_CXX_FLAGS "-ggdb -Wall -Wno-unknown-pragmas -std=gnu++0x -fpermissive")
 
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(LUA REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(FreeType REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_net REQUIRED)
find_package(SDL_image REQUIRED)


MESSAGE( STATUS "INCLUDES")
MESSAGE( STATUS " OpenGL & GLU:   " ${OPENGL_INCLUDE_DIR} )
MESSAGE( STATUS " GLEW:           " ${GLEW_INCLUDE_DIRS} )
MESSAGE( STATUS " LUA:            " ${LUA_INCLUDE_DIR} )
MESSAGE( STATUS " JPEG:           " ${JPEG_INCLUDE_DIR} )
MESSAGE( STATUS " PNG:            " ${PNG_INCLUDE_DIR} )
MESSAGE( STATUS " FreeType:       " ${FREETYPE_INCLUDE_DIRS} )
MESSAGE( STATUS " SDL:            " ${SDL_INCLUDE_DIR})
MESSAGE( STATUS " SDL_Net:        " ${SDL_IMAGE_INCLUDE_DIR} )
MESSAGE( STATUS " SDL_Image:      " ${SDL_NET_INCLUDE_DIR} )

MESSAGE( STATUS "LIBRARIES")
MESSAGE( STATUS " OpenGL & GLU:   " ${OPENGL_LIBRARY} )
MESSAGE( STATUS " GLEW:           " ${GLEW_LIBRARIES} )
MESSAGE( STATUS " LUA:            " ${LUA_LIBRARIES} )
MESSAGE( STATUS " JPEG:           " ${JPEG_LIBRARIES} )
MESSAGE( STATUS " PNG:            " ${PNG_LIBRARIES} )
MESSAGE( STATUS " FreeType:       " ${FREETYPE_LIBRARY} )
MESSAGE( STATUS " SDL:            " ${SDL_LIBRARY} )
MESSAGE( STATUS " SDL_Net         " ${SDL_NET_LIBRARY} )
MESSAGE( STATUS " SDL_Image       " ${SDL_IMAGE_LIBRARY} )

include_directories("src")
include_directories(${TREADS_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${JPEG_INCLUDE_DIR})
include_directories(${PNG_INCLUDE_DIR})
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(${SDL_INCLUDE_DIR})
include_directories(${SDL_NET_INCLUDE_DIR})
include_directories(${SDL_IMAGE_INCLUDE_DIR})
include_directories("../libs/tinyxml_2_6_1")
include_directories("../libs/fmodapi375win/api/inc")

link_directories("../libs/fmodapi375win/api/lib")

# Selection des fichiers source
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
	../libs/agar-1.5.0/core/*.c
	../libs/agar-1.5.0/dev/*.c
	../libs/agar-1.5.0/math/*.c
	../libs/agar-1.5.0/gui/*.c
)

set_source_files_properties(${LIB_AGAR_SRC} PROPERTIES HEADER_FILE_ONLY TRUE)

# Configuration de l'executable
add_executable(
	jkt
	${SOURCE_FILES}
	${LIB_AGAR_SRC}
)

target_link_libraries(
	jkt
	ag_core ag_gui ag_dev
	${SDL_IMAGE_LIBRARY} ${SDL_NET_LIBRARY} ${SDL_LIBRARY}
	fmod64vc
	Iphlpapi ziparch mingw32 winmm ${FREETYPE_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ws2_32 pthread ${LUA_LIBRARIES} ${JPEG_LIBRARIES} ${PNG_LIBRARIES} dxguid
)
