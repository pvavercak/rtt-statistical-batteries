# RTT Statistical Batteries

Set of statistical batteries that are used by [Randomness Testing Toolkit](https://github.com/crocs-muni/randomness-testing-toolkit).

It is highly recommended to use this set as the batteries are modified to be fully compatible with the interface employed by RTT.

**Following batteries are included:**

* **NIST Statistical Test Suite** [Official page](http://csrc.nist.gov/groups/ST/toolkit/rng/documentation_software.html)  
Official implementation is not used due to being too slow. Instead optimized version is used: [NIST-STS-optimised](https://github.com/sysox/NIST-STS-optimised).
* **Dieharder: A Random Number Test Suite** [Official page](http://www.phy.duke.edu/~rgb/General/dieharder.php)  
Slightly modified version of Dieharder 3.31.1 is used. Modifications affect only battery output format, not the results itself.
* **TestU01** [Official page](http://simul.iro.umontreal.ca/testu01/tu01.html)  
This battery is shipped only as a library, we have custom command-line interface implemented.
* **BSI** Implementation was extracted from ParanoYa application (created at Faculty of Electrical Engineering and Information Technology, STU in Bratislava). Battery is not used by [RTT](https://github.com/crocs-muni/randomness-testing-toolkit).
* **FIPS** [Source code](https://salsa.debian.org/hmh/rng-tools/-/blob/master/fips.c) Battery is not used by [RTT](https://github.com/crocs-muni/randomness-testing-toolkit).

## Prerequisites
* C/C++ compiler
* CMake [official page](https://cmake.org/)
* Ninja (optional, but recommended because of its speed) [official page](https://ninja-build.org)
* nlohmann_json [github](https://github.com/nlohmann/json)
* GNU Scientific Library [official page](https://www.gnu.org/software/software.html)
* FFTW3 [official page](https://www.fftw.org/)

### C/C++ compiler
Depending on your platform, there are several ways of how to install such compiler.
`Keep in mind that on Windows`, compilation of this project was tested only with `MinGW GCC` compiler.

### CMake installation
It can be easily installed on linux via [the official shell scripts](https://cmake.org/download/):
```bash
chmod a+x cmake-<ver>-linux-x86_64.sh
sudo ./cmake-<ver>-linux-x86_64.sh --skip-license --prefix=/usr
```
On Windows, an official `exe` script will help you with CMake installation.

### nlohmann_json
This dependency's source code is stored inside `.third_party` subfolder and it's built as an external `CMake` project in a project that needs this dependency. For more information about how CMake's `ExternalProject_Add` works, see [the documentation](https://cmake.org/cmake/help/latest/module/ExternalProject.html).

### FFTW3
Similar to `nlohmann`, this dependency is also stored inside `.third_party` subfolder and it's going to be built as an external `CMake` project whenever a dependent project needs to pre-compile it. For more information about how CMake's `ExternalProject_Add` works, see [the documentation](https://cmake.org/cmake/help/latest/module/ExternalProject.html).

### GNU Scientific Library (GSL)
This is the only dependency which has to be present in a system.
It is quite easy to install on most of the popular linux distributions, for example on Ubuntu, you just run:
```bash
sudo apt-get install libgsl-dev
```
Inside MinGW environment (based on your toolchain), you need to run:
```bash
pacman -S mingw-w64-x86_64-gsl # for gcc
# or
pacman -S mingw-w64-clang-x86_64-gsl # for clang
```
or you can just search for a package on [this official MSYS site](https://packages.msys2.org/queue).

## Compilation
This is a straightforward process and you can take a look at `buildall_linux.sh, buildall_mingw.sh` or `buildall_macos.sh`
to get the idea of compiling this project with `CMake`.
In all of the cases, you just need to run couple of commands inside build directory:
```bash
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=<some path> <path to CMakeLists.txt>
ninja
ninja install
```

### MacOS libgsl compilation
In order to prevent linking issues, libgsl is built from sources for MacOS platform.
This is required because of the `Non-Fat binary` vs `Fat binary` concept.
You can take a look at `buildall_macos.sh` to see how it's done.
Additionally, there is a special CMake variable `CMAKE_OSX_ARCHITECTURES` which is used
to define what architectures a user want to include in resulting binaries.
