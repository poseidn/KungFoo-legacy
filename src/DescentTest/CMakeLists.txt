cmake_minimum_required (VERSION 2.6)
project (Descent)

include ( ../CMake_Defaults.txt )
include ( ../CMake_Externals.txt )

# pthread must be linked explicitly for llvm/clang builds
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread" )

include_directories (external/gtest/include)
include_directories (external/gtest/)

include_directories( ${Descent_SOURCE_DIR}/../DescentEngine/external/tinyxml2 ) 

include_directories ( ../ )

add_executable( DescentTest 
				external/gtest/src/gtest-all.cc
				src/DescentEngine/TimeSliceActionTest.cpp
				src/DescentEngine/ConcurrentStackTest.cpp
				src/DescentEngine/StaticVectorTest.cpp
				src/DescentEngine/FloatCharTest.cpp
				src/DescentEngine/PhysicsTest.cpp
				src/test_main.cc
)

target_link_libraries( DescentTest DescentEngine DescentLogic )


