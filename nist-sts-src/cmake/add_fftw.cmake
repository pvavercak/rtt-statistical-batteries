include(${CMAKE_CURRENT_LIST_DIR}/build_external_project.cmake)

set(fftw_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install/fftw")

set(fftw_CMAKE_ARGS
  "-DBUILD_SHARED_LIBS=OFF"
  "-DCMAKE_INSTALL_PREFIX=${fftw_INSTALL_PREFIX}"
  "-DCMAKE_INSTALL_LIBDIR=lib"
)

if(APPLE)
  string(REPLACE ";" "$<SEMICOLON>" "${CMAKE_OSX_ARCHITECTURES}" CMAKE_OSX_ARCHITECTURES_)
  list(APPEND nlohmann_json_CMAKE_ARGS "-DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES_}")
endif()

build_external_project(FFTW3 "${CMAKE_SOURCE_DIR}/../.third_party/fftw-3.3.9.zip" ${fftw_CMAKE_ARGS})

find_package(FFTW3 REQUIRED PATHS "${fftw_INSTALL_PREFIX}" NO_DEFAULT_PATH)
