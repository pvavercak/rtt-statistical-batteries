#!/bin/bash
set -e

# Configuration:
install_destination=$PWD/install
subdirectories=(
  bsi-src
  fips-src
  nist-sts-src
  testu01-src
)
build_type=Release

# Build:
for subdir in "${subdirectories[@]}"; do
  build_dir=$subdir/build

  # Cleanup
  if [ -d $build_dir ]; then
    rm -rf $build_dir
  fi

  mkdir -p $subdir/build && cd $_
  cmake -G Ninja -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -DCMAKE_BUILD_TYPE=$build_type -DCMAKE_INSTALL_PREFIX=$install_destination ..
  ninja
  ninja install
  cd ../../
done

# build libgsl:
cd dieharder-src
curl -o gsl-2.7.tar.gz https://ftp.gnu.org/gnu/gsl/gsl-2.7.tar.gz
tar xf gsl-2.7.tar.gz
cd gsl-2.7

# libgsl as a fat binary:
CC=clang \
CXX=clang++ \
CFLAGS="-arch arm64 -arch x86_64 -O3" \
CXXFLAGS="-arch arm64 -arch x86_64 -O3" \
./configure --prefix=$PWD/.gsl/
make -j8
make install

# build dieharder:
cd ..
mkdir build && cd $_
cmake -G Ninja -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -DCMAKE_BUILD_TYPE=$build_type -DCMAKE_INSTALL_PREFIX=$install_destination ..
ninja
ninja install
cd ../../
