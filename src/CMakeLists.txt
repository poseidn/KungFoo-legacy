cmake_minimum_required (VERSION 2.6)
project (descent)

# CPack Setup 
#SET(CPACK_GENERATOR "DEB")
SET(CPACK_GENERATOR "TGZ")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "David Doria")

include ( CPack )

include ( CMake_Defaults.txt )

add_subdirectory( DescentEngine )
add_subdirectory( DescentLogic )
# disabled game build in default config, as Travis-Ci has trouble with
# conflicting boost versions
#add_subdirectory( DescentGame )

if(UNIX)
	add_subdirectory( DescentTest )
	add_subdirectory( EngineBench )
	add_subdirectory( LevelEditor )
endif()

enable_testing()
add_test( NAME DescentTest 
		  COMMAND DescentTest/DescentTest ) 
