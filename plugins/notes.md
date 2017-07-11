# Overview
Using this space to play around with S2E plugins. Installing a new plugin involves:

1. Copying plugin .h and .cpp files into a directory, such as:
  - /home/s2e/s2e/source/s2e/libs2eplugins/src/s2e/Plugins/<dir>/here.cpp
  - /home/s2e/s2e/source/s2e/libs2eplugins/src/s2e/Plugins/ExecutionTracers/Blerg.cpp
1. Adding the appropriate compile line in the Makefile that exists in <dir>
1. Recompiling everything :(

# Recompiling
Recompiling can be a pain. To recompile as I have it setup in my docker image, do the following:

```bash
cd /home/s2e/s2e/build/s2e
find -name libs2eplugins -exec cp -r /home/s2e/s2e/source/s2e/libs2eplugins/* {}/.  \;
CMAKE_PREFIX_PATH=llvm-release/cmake/modules/CMakeFiles:/home/s2e/s2e/source/s2e/libs2eplugins S2EPREFIX=/home/s2e/s2e/install make -f ../../source/s2e/Makefile install
```

That puts you in the right directory, populates the plugins directories, sets your env variables and recompiles.

Here's a bash function to call for rebuilding in one command:
```bash
function s2e_rebuild { pushd .; cd /home/s2e/s2e/build/s2e; rm stamps/libs2e-*; CMAKE_PREFIX_PATH=llvm-release/cmake/modules/CMakeFiles:/home/s2e/s2e/source/s2e/libs2eplugins S2EPREFIX=/home/s2e/s2e/install make -f ../../source/s2e/Makefile install; find -name libs2eplugins -exec cp -r /home/s2e/s2e/source/s2e/libs2eplugins/* {}/.  \;; CMAKE_PREFIX_PATH=llvm-release/cmake/modules/CMakeFiles:/home/s2e/s2e/source/s2e/libs2eplugins S2EPREFIX=/home/s2e/s2e/install make -f ../../source/s2e/Makefile install; popd;}
```

# Problems
Dunno why. Sometimes it will not compile correctly the first time, but if I run the exact same command a second time (the cmake one), it works...
