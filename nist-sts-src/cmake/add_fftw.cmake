include(${CMAKE_CURRENT_LIST_DIR}/build_external_project.cmake)

set(fftw_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install/fftw")

set(fftw_CMAKE_ARGS
    "-DBUILD_SHARED_LIBS=OFF"
    "-DCMAKE_INSTALL_PREFIX=${fftw_INSTALL_PREFIX}"
)

build_external_project(fftw3 "${CMAKE_SOURCE_DIR}/3rd_party/fftw-3.3.9.zip" ${fftw_CMAKE_ARGS})

find_package(fftw3 REQUIRED PATHS "${fftw_INSTALL_PREFIX}" NO_DEFAULT_PATH)
