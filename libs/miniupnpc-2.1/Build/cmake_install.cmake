# Install script for directory: R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/miniupnpc")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/bin/objdump.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1/Build/libminiupnpc.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1/Build/libminiupnpc.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1/Build/libminiupnpc.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libminiupnpc.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libminiupnpc.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libminiupnpc.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/miniupnpc" TYPE FILE FILES
    "R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1/miniupnpc.h"
    "R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1/miniwget.h"
    "R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1/upnpcommands.h"
    "R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1/igd_desc_parse.h"
    "R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1/upnpreplyparse.h"
    "R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1/upnperrors.h"
    "R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1/upnpdev.h"
    "R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1/miniupnpctypes.h"
    "R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1/portlistingparse.h"
    "R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1/miniupnpc_declspec.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "R:/Dev/games/GameJam/WPI-game-jam-2020/libs/miniupnpc-2.1/Build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
