# simplePlot
simplePlot is a simple c++ library that lets you plot 2D graphs using [gnuplot](http://gnuplot.sourceforge.net) in your c++ code. 
## Prerequisites
The only dependecy for simplePlot is [gnuplot](http://gnuplot.sourceforge.net) which can be downloaded from you package manager. For Debian/Ubuntu
```
sudo apt-get install gnuplot
```
for macOS
```
brew install gnuplot
```
## Build
To build simplePlot you can use CMake to generate the MakeFile:
```
cmake . & make
```
which will build the static library (.a) and the test program in the bin/ subdirectory. The file CMakeLists.txt can be modified to generate the shared/dynamic library (.so or .dylib) instead of the static one. Check the CMake [documentation](https://cmake.org/cmake/help/v3.0/command/add_library.html).
## License
This software is distributed under the MIT license, check the LICENSE.txt file.
