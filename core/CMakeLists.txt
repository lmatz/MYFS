cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

include_directories(.)

file(GLOB_RECURSE CoreSources src/*.cpp)
add_library(LibMyfsCore STATIC ${CoreSources})

if (${ENABLE_MYFS_TEST})
	file(GLOB_RECURSE CoreTest test/*.cpp)
	add_library(MyfsCoreTest STATIC ${CoreTest})

	add_executable(MyfsCoreTests ${CoreTest})
	target_link_libraries(MyfsCoreTests LibMyfsCore MyfsCoreTest ${ExtTestLibs} pthread)
endif ()
