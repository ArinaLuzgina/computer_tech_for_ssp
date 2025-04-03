cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME holography)
project(${PROJECT_NAME} LANGUAGES C CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3") 

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
add_executable(1d_find ./src/1d_find.cpp)
add_executable(1d_find_precise ./src/1d_find_precise.cpp)
add_executable(2d_find ./src/2d_find.cpp)

add_executable(2d_emitter ./src/2d_emitter.cpp)
add_executable(2d_em_in3d ./src/2d_em_in3d.cpp)

# link libs
#GMSH
target_link_libraries(point ${GMSH_LIBRARIES})
target_link_libraries(1d_emitter ${GMSH_LIBRARIES})
target_link_libraries(2d_emitter ${GMSH_LIBRARIES})
target_link_libraries(1d_find ${GMSH_LIBRARIES})
target_link_libraries(1d_find_precise ${GMSH_LIBRARIES})
target_link_libraries(2d_find ${GMSH_LIBRARIES})
target_link_libraries(2d_em_in3d ${GMSH_LIBRARIES})

#vtk
target_link_libraries(point ${VTK_LIBRARIES})
target_link_libraries(1d_emitter ${VTK_LIBRARIES})
target_link_libraries(2d_emitter ${VTK_LIBRARIES})
target_link_libraries(1d_find ${VTK_LIBRARIES})
target_link_libraries(1d_find_precise ${VTK_LIBRARIES})
target_link_libraries(2d_find ${VTK_LIBRARIES})
target_link_libraries(2d_em_in3d ${VTK_LIBRARIES})


