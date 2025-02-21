cmake_minimum_required(VERSION 3.11)

set(VTK_DIR "~/vtk/build")

set(CMAKE_CXX_FLAGS "-std=c++11")

project(make_some_physics)
add_executable(make_net ./problems/make_net.cpp)
add_executable(some_physics ./problems/some_physics.cpp)

add_library(gmsh SHARED IMPORTED)
set_target_properties(gmsh PROPERTIES
    IMPORTED_LOCATION "/usr/lib/x86_64-linux-gnu/libgmsh.so"
    INTERFACE_INCLUDE_DIRECTORIES "/usr/include"
)
set(GMSH_LIBRARIES gmsh)

find_package(VTK REQUIRED)

# Укажем библиотеки для линковки
# ... тут только VTK
target_link_libraries(make_net ${VTK_LIBRARIES})
target_link_libraries(some_physics ${VTK_LIBRARIES})

# ... а тут ещё и gmsh
target_link_libraries(make_net ${GMSH_LIBRARIES})
target_link_libraries(some_physics ${GMSH_LIBRARIES})

# target_link_libraries(tetr3d ${GMSH_LIBRARIES})
