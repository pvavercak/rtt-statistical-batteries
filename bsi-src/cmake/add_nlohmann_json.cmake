include(${CMAKE_CURRENT_LIST_DIR}/build_external_project.cmake)

set(nlohmann_json_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install/nlohmann_json")

set(nlohmann_json_CMAKE_ARGS
  "-DBUILD_SHARED_LIBS=OFF"
  "-DCMAKE_INSTALL_PREFIX=${nlohmann_json_INSTALL_PREFIX}"
  "-DCMAKE_INSTALL_LIBDIR=lib"
)

build_external_project(nlohmann_json "${CMAKE_SOURCE_DIR}/../.third_party/nlohmann_json-3.11.2.zip" ${nlohmann_json_CMAKE_ARGS})

find_package(nlohmann_json REQUIRED PATHS "${nlohmann_json_INSTALL_PREFIX}" NO_DEFAULT_PATH)
