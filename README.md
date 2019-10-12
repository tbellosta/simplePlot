# simplePlot
simplePlot is a simple c++ library that lets you plot 2D graphs using [gnuplot](http://gnuplot.sourceforge.net) in your c++ code. 
## Prerequisites
The only dependecy for simplePlot is [gnuplot](http://gnuplot.sourceforge.net) which can be downloaded from you package manager. For Debian/Ubuntu
```
$ sudo apt-get install gnuplot
```
for macOS
```
$ brew install gnuplot
```
## Build
To build simplePlot you can use CMake to generate the MakeFile:
```
$ cmake . 
$  make
```
which will build the static library (.a) and the test program in the bin/ subdirectory. Alternatively the shared library (.so/.dylib) can be built by passing the following variable to CMake
```
$ cmake -DBUILD_SHARED_LIBS=ON . 
$  make
```
## License
This software is distributed under the MIT license, check the [LICENSE.txt](LICENSE.txt) file.
