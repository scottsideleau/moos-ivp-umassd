# moos-ivp-umassd
MOOS-IvP applications and behaviors maintained by UMassD staff and students.

## Directory Structure
The directory structure for moos-ivp-umassd is decribed below:

Directory/File   | Description
---              | ---
bin/             | Directory for generated executable files
build/           | Directory for build object files
data/            | Directory for storing data
lib/             | Directory for generated library files
missions/        | Directory for mission files
scripts/         | Directory for script files
src/             | Directory for source code
build.sh         | Script for building moos-ivp-umassd
CMakeLists.txt   | CMake configuration file for the project
LICENSE          | File containing re-use and distribution terms
README.md        | Contains helpful information - (this file).

## Build Instructions
To build on Linux and Apple platforms, execute the build script within this
directory:

  ```bash
  $ ./build.sh
  ```

To build without using the supplied script, execute the following commands
within this directory:

  ```bash
  $ mkdir -p build
  $ cd build
  $ cmake ../
  $ make
  $ cd ..
  ```

## Environment variables
The moos-ivp-umassd binaries files should be added to your path to allow them
to be launched from pAntler. 

  ```bash
  export PATH=$PATH:/path/to/moos-ivp-umassd
  ```

In order for generated IvP Behaviors to be recognized by the IvP Helm, you
should add the library directory to the "IVP_BEHAVIOR_DIRS" environment 
variable.

  ```bash
  export IVP_BEHAVIOR_DIRS=$IVP_BEHAVIOR_DIRS:/path/to/moos-ivp-umassd/lib
  ```
