#  Check for CMake
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

INCLUDE(CheckCXXCompilerFlag) 

#  Load projects
PROJECT("bceplus-core")

#  Check C++11 support
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSEIF(COMPILER_SUPPORTS_CXX0X)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
ELSE()
	MESSAGE(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
ENDIF()

#  Load include directories
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include")

#  Library sources
ADD_SUBDIRECTORY(sources)

#  Unit tests sources
IF (NOT NO_UNIT_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF (NOT NO_UNIT_TESTS)
