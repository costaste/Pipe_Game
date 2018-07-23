# Project created by Stephen Costa, Nicholas Smith, and Olivia DeFlumeri

## Windows Setup:
Ensure gcc/g++/MinGW are up to date

Note that we used Code::Blocks so instructions will be based on that platform.

Once you download Code::Blocks, you can choose "open existing project" and choose our "Pipes.cbp" file.

Next, you'll need to set up SDL.

SDL Setup:
- Download SDL2, SDL2_mixer, SDL2_ttf (MinGW dev version)
- We used the 32 bit development library, so once you download these libraries, you will look in the "i686-w64-mingw32" folder for proceding files/directories
- Place their .dll files (found in the 32bit bin folder) into the Windows system folder
- Ensure the library for SDL and its extension libraries is properly linked
	- Project > Build Options > Search Directories > Linker > add the lib folder
- Ensure the SDL include file is linked with Code::Blocks
	- Project > Build Options > Search Directories > Compiler > add the include/SDL2 folder
- Add the linking flags to tell the compiler to link against these libraries
	- Project > Build Options > Linker Settings > Under "Other linker options" add: "-lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf"

Now SDL should be set up. 
If there are errors, we found these guides helpful for setup:
SDL Library:
http://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/codeblocks/index.php

External SDL Libraries:
http://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/windows/codeblocks/index.php

From here, to run the game, you should only have to click the "build & run" option from Code::Blocks (yellow cog and green arrow button)


## Linux Setup:
Ensure gcc and g++ are up to date

Getting SDL2 using the package manager
From the command line run the following commands:
  - sudo apt-cache search libsdl2
  - sudo apt-get install libsdl2-dev
  - sudo apt-get install libsdl2-mixer
  - sudo apt-get install libsdl2-ttf

Compile code using Code::Blocks

Download and install Code::Blocks

Open the .cbp project file from the repo

Go to Project->Build options

	-In the "Linker settings" tab put the following flags into "Other linker options":
	
		-lSDL2 -lSDL2_mixer -lSDL2_ttf
		
Now if you can build and run the project through the Code::Blocks editor

## Test Setup:
The Tests are only guaranteed to run on a Linux machine. A copy of the binary can be found at Binary/runner.

Download the tar.gz file of cxxtest-4.4 from https://sourceforge.net/projects/cxxtest/files/cxxtest/ into the tests directory
	- Navigate to the tests directory and extract the downloaded files using *tar xzvf*
	- run make
	- A file named "runner" should appear. Run this file for test output.

## How to Play:
- Once you run the game, you will be at the start menu
- You can set difficulty options by selecting the relevent buttons
- Press Start to being playing
- Click on grey tiles to uncover pipes
- Click on pipe tiles to swap them with the pocket tile (off the board to the left)
- Flow will continue into pipes that are connected to the last pipe with water
- Once flow enters the faucet, you win
- Once flow cannot go into a pipe (hits wall, covered tile, junk tile or incorrect facing pipe) you lose

