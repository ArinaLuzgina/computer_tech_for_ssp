# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/arinal/Desktop/phystech/4_semester/prog/computer_tech_for_ssp/lab1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/arinal/Desktop/phystech/4_semester/prog/computer_tech_for_ssp/lab1/build

# Include any dependencies generated for this target.
include CMakeFiles/tokamaka.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/tokamaka.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/tokamaka.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tokamaka.dir/flags.make

CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.o: CMakeFiles/tokamaka.dir/flags.make
CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.o: ../problems/tokamaka.cpp
CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.o: CMakeFiles/tokamaka.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/arinal/Desktop/phystech/4_semester/prog/computer_tech_for_ssp/lab1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.o -MF CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.o.d -o CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.o -c /home/arinal/Desktop/phystech/4_semester/prog/computer_tech_for_ssp/lab1/problems/tokamaka.cpp

CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/arinal/Desktop/phystech/4_semester/prog/computer_tech_for_ssp/lab1/problems/tokamaka.cpp > CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.i

CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/arinal/Desktop/phystech/4_semester/prog/computer_tech_for_ssp/lab1/problems/tokamaka.cpp -o CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.s

# Object files for target tokamaka
tokamaka_OBJECTS = \
"CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.o"

# External object files for target tokamaka
tokamaka_EXTERNAL_OBJECTS =

tokamaka: CMakeFiles/tokamaka.dir/problems/tokamaka.cpp.o
tokamaka: CMakeFiles/tokamaka.dir/build.make
tokamaka: /usr/lib/x86_64-linux-gnu/libgmsh.so
tokamaka: CMakeFiles/tokamaka.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/arinal/Desktop/phystech/4_semester/prog/computer_tech_for_ssp/lab1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tokamaka"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tokamaka.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tokamaka.dir/build: tokamaka
.PHONY : CMakeFiles/tokamaka.dir/build

CMakeFiles/tokamaka.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tokamaka.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tokamaka.dir/clean

CMakeFiles/tokamaka.dir/depend:
	cd /home/arinal/Desktop/phystech/4_semester/prog/computer_tech_for_ssp/lab1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/arinal/Desktop/phystech/4_semester/prog/computer_tech_for_ssp/lab1 /home/arinal/Desktop/phystech/4_semester/prog/computer_tech_for_ssp/lab1 /home/arinal/Desktop/phystech/4_semester/prog/computer_tech_for_ssp/lab1/build /home/arinal/Desktop/phystech/4_semester/prog/computer_tech_for_ssp/lab1/build /home/arinal/Desktop/phystech/4_semester/prog/computer_tech_for_ssp/lab1/build/CMakeFiles/tokamaka.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tokamaka.dir/depend

