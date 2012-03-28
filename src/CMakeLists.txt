cmake_minimum_required(VERSION 2.6)
		
#Configuration du projet
project(jkt)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

# Sélection des fichiers source
file(
	GLOB_RECURSE
	source_files
	src/*
	../Menu/*
)

# Configuration de l'exécutable
add_executable(
	jkt
	${source_files}
)