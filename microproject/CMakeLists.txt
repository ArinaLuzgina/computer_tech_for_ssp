cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME holography)
project(${PROJECT_NAME} LANGUAGES C CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#add vtk
set(VTK_DIR "~/vtk/build")
find_package(VTK REQUIRED)

#add gmsh
add_library(gmsh SHARED IMPORTED)
set_target_properties(gmsh PROPERTIES
    IMPORTED_LOCATION "/usr/lib/x86_64-linux-gnu/libgmsh.so"
    INTERFACE_INCLUDE_DIRECTORIES "/usr/include"
)
set(GMSH_LIBRARIES gmsh)

#add files
add_executable(point ./src/point_emitter.cpp)
add_executable(1d_emitter ./src/1d_emitter.cpp)

add_executable(long ./src/long_emit.cpp)

# link libs
#GMSH
target_link_libraries(point ${GMSH_LIBRARIES})
target_link_libraries(1d_emitter ${GMSH_LIBRARIES})

target_link_libraries(long ${GMSH_LIBRARIES})

#vtk
target_link_libraries(point ${VTK_LIBRARIES})
target_link_libraries(1d_emitter ${VTK_LIBRARIES})

target_link_libraries(long ${VTK_LIBRARIES})

