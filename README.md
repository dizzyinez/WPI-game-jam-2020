# WPI-game-jam-2020

What's This
-----
This is the source code to the game Locomotion Commotion, made in one week for the [WPI game jam](https://wp.wpi.edu/gamejam/)

Made by members of FRC team 4388 [Ridgebotics](https://www.ridgebotics.com/)

The game is incomplete and is missing many key features, but some core mechanics are implemented. 
It has functioning netcode, and UPNP so you don't have to port-forward to test it. 
More information can be found in the readme file in the build folder.

Compiling
-----
Compiling only works on windows, only tested on windows 10 but other versions should work (I think at least win7 and win8)
1. In order to compile you must install [CMake](https://cmake.org/) and [MinGW-w64](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download)

2. Open cmd and navigate to the root folder of this project. run

`cmake -BBuild -G"MinGW Makefiles"`

(if this throws an error, you probably need to delete the cmake cache in the build folder)

`cd Build`

`mingw32-make`
