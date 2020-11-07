cmake_minimum_required (VERSION 3.10)
project (Trains VERSION 0.1)

configure_file(include/Config.h.in ${CMAKE_SOURCE_DIR}/include/Config.h)

include_directories(include)

SET (CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(CMAKE_MODULE_LINKER_FLAGS "-m32 -Wl,--disable-stdcall-fixup ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_FILES "src/*.c" "src/*.cc" "src/*.cpp")
file(GLOB_RECURSE INCS_H "include/*.h" "include/*.hpp")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${INCS_H})
# set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

include_directories(c   
"${CMAKE_SOURCE_DIR}/libs/SDL2/include/SDL2"
"${CMAKE_SOURCE_DIR}/libs/SDL2Image/include/SDL2"
"${CMAKE_SOURCE_DIR}/libs/SDL2ttf/include/SDL2"
"${CMAKE_SOURCE_DIR}/libs/SDL2Mixer/include/SDL2"
"${CMAKE_SOURCE_DIR}/libs/ENet/include/"
)

SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} 
"${CMAKE_SOURCE_DIR}/libs/SDL2/lib"
"${CMAKE_SOURCE_DIR}/libs/SDL2Image/lib"
"${CMAKE_SOURCE_DIR}/libs/SDL2ttf/lib"
"${CMAKE_SOURCE_DIR}/libs/SDL2Mixer/lib"
"${CMAKE_SOURCE_DIR}/libs/ENet/lib"
"C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/x86_64-w64-mingw32/lib"
)

macro(find_library_wrapper)          
    find_library(${ARGV0} ${ARGV1})    
    if (NOT ${ARGV0})
        message(FATAL_ERROR "-- couldn't find library ${ARGV1}")
    endif(NOT ${ARGV0})
	if (DEFINED CMAKEDEBUG)
	endif()
    message("-- Library ${ARGV1} found as ${${ARGV0}}")   
endmacro()

find_library_wrapper(sdlmain libSDL2main.a)
find_library_wrapper(sdl libSDL2.dll.a)
find_library_wrapper(sdlimage libSDL2_image.dll.a)
find_library_wrapper(sdlttf libSDL2_ttf.dll.a)
find_library_wrapper(sdlmixer libSDL2_mixer.dll.a)
find_library_wrapper(enet libenet.a)

target_link_libraries(${PROJECT_NAME}
${sdlmain}
${sdl}
${sdlimage}
${sdl}
${sdl}
${enet}
)