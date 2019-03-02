cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cudatest)

add_executable(cudatest app.cpp)

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(OPENCV "opencv")
  if (OPENCV_FOUND)
    target_link_libraries(cudatest ${OPENCV_LIBRARIES})
    add_definitions(${OPNECV_CFLAGS} ${OPENCV_CFLAGS_OTHER})
  endif()
endif()
