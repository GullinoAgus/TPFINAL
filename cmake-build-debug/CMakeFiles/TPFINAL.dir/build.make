# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/agus/CLion/clion-2020.2.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/agus/CLion/clion-2020.2.4/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/agus/CLionProjects/TPFINAL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/agus/CLionProjects/TPFINAL/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/TPFINAL.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TPFINAL.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TPFINAL.dir/flags.make

CMakeFiles/TPFINAL.dir/main.c.o: CMakeFiles/TPFINAL.dir/flags.make
CMakeFiles/TPFINAL.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/agus/CLionProjects/TPFINAL/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/TPFINAL.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/TPFINAL.dir/main.c.o   -c /home/agus/CLionProjects/TPFINAL/main.c

CMakeFiles/TPFINAL.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/TPFINAL.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/agus/CLionProjects/TPFINAL/main.c > CMakeFiles/TPFINAL.dir/main.c.i

CMakeFiles/TPFINAL.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/TPFINAL.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/agus/CLionProjects/TPFINAL/main.c -o CMakeFiles/TPFINAL.dir/main.c.s

# Object files for target TPFINAL
TPFINAL_OBJECTS = \
"CMakeFiles/TPFINAL.dir/main.c.o"

# External object files for target TPFINAL
TPFINAL_EXTERNAL_OBJECTS =

TPFINAL: CMakeFiles/TPFINAL.dir/main.c.o
TPFINAL: CMakeFiles/TPFINAL.dir/build.make
TPFINAL: CMakeFiles/TPFINAL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/agus/CLionProjects/TPFINAL/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable TPFINAL"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TPFINAL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TPFINAL.dir/build: TPFINAL

.PHONY : CMakeFiles/TPFINAL.dir/build

CMakeFiles/TPFINAL.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TPFINAL.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TPFINAL.dir/clean

CMakeFiles/TPFINAL.dir/depend:
	cd /home/agus/CLionProjects/TPFINAL/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/agus/CLionProjects/TPFINAL /home/agus/CLionProjects/TPFINAL /home/agus/CLionProjects/TPFINAL/cmake-build-debug /home/agus/CLionProjects/TPFINAL/cmake-build-debug /home/agus/CLionProjects/TPFINAL/cmake-build-debug/CMakeFiles/TPFINAL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TPFINAL.dir/depend

