cmake_minimum_required (VERSION 2.6)
project (Descent)

include_directories( ${Descent_SOURCE_DIR}/src ) 
include_directories( ${Descent_SOURCE_DIR}/.. ) 

include ( ../CMake_Externals.txt )

if (UNIX) 
add_executable(DescentGame
	src/Main.cpp
)
endif()

if (WIN32)
add_executable(DescentGame
	src/WinMain.cpp
)
endif()

# use SDL to setup all of our open GL stuff 
add_definitions( -DUSE_SDL_OPENGL )

# static link with unix as the boost libraries are hard to get
# in the correct version  
if (UNIX)
	target_link_libraries( DescentGame boost_system.a )
	target_link_libraries( DescentGame boost_program_options.a )
endif()

if (WIN32)
	target_link_libraries( DescentGame boost_system )
	target_link_libraries( DescentGame boost_program_options )
endif()

target_link_libraries( DescentGame DescentEngine DescentLogic )

## perf tools 
if ( COMPILE_WITH_PROFILER )
	target_link_libraries( DescentGame profiler )
endif()


## packaging and installation

# don't give a specific installation path, as it will only be shipped as a gzip package
install ( TARGETS DescentGame 
		  DESTINATION . )
install ( DIRECTORY ../../ingame_assets/images ../../ingame_assets/levels ../../ingame_assets/sounds 
		  DESTINATION . )



