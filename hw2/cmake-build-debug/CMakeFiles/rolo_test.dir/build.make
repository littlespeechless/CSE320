# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/zhengyu/Desktop/CSE320/hw2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/rolo_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/rolo_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rolo_test.dir/flags.make

CMakeFiles/rolo_test.dir/tests/base_tests.c.o: CMakeFiles/rolo_test.dir/flags.make
CMakeFiles/rolo_test.dir/tests/base_tests.c.o: ../tests/base_tests.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/rolo_test.dir/tests/base_tests.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo_test.dir/tests/base_tests.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/tests/base_tests.c

CMakeFiles/rolo_test.dir/tests/base_tests.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo_test.dir/tests/base_tests.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/tests/base_tests.c > CMakeFiles/rolo_test.dir/tests/base_tests.c.i

CMakeFiles/rolo_test.dir/tests/base_tests.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo_test.dir/tests/base_tests.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/tests/base_tests.c -o CMakeFiles/rolo_test.dir/tests/base_tests.c.s

# Object files for target rolo_test
rolo_test_OBJECTS = \
"CMakeFiles/rolo_test.dir/tests/base_tests.c.o"

# External object files for target rolo_test
rolo_test_EXTERNAL_OBJECTS =

rolo_test: CMakeFiles/rolo_test.dir/tests/base_tests.c.o
rolo_test: CMakeFiles/rolo_test.dir/build.make
rolo_test: /usr/local/Cellar/criterion/2.3.3/lib/libcriterion.dylib
rolo_test: CMakeFiles/rolo_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable rolo_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rolo_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rolo_test.dir/build: rolo_test

.PHONY : CMakeFiles/rolo_test.dir/build

CMakeFiles/rolo_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rolo_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rolo_test.dir/clean

CMakeFiles/rolo_test.dir/depend:
	cd /Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/zhengyu/Desktop/CSE320/hw2 /Users/zhengyu/Desktop/CSE320/hw2 /Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug /Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug /Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles/rolo_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rolo_test.dir/depend

