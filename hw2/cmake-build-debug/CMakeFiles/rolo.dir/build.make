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
include CMakeFiles/rolo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/rolo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rolo.dir/flags.make

CMakeFiles/rolo.dir/src/toolsdir/args.c.o: CMakeFiles/rolo.dir/flags.make
CMakeFiles/rolo.dir/src/toolsdir/args.c.o: ../src/toolsdir/args.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/rolo.dir/src/toolsdir/args.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo.dir/src/toolsdir/args.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/src/toolsdir/args.c

CMakeFiles/rolo.dir/src/toolsdir/args.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo.dir/src/toolsdir/args.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/src/toolsdir/args.c > CMakeFiles/rolo.dir/src/toolsdir/args.c.i

CMakeFiles/rolo.dir/src/toolsdir/args.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo.dir/src/toolsdir/args.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/src/toolsdir/args.c -o CMakeFiles/rolo.dir/src/toolsdir/args.c.s

CMakeFiles/rolo.dir/src/toolsdir/ctools.c.o: CMakeFiles/rolo.dir/flags.make
CMakeFiles/rolo.dir/src/toolsdir/ctools.c.o: ../src/toolsdir/ctools.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/rolo.dir/src/toolsdir/ctools.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo.dir/src/toolsdir/ctools.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/src/toolsdir/ctools.c

CMakeFiles/rolo.dir/src/toolsdir/ctools.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo.dir/src/toolsdir/ctools.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/src/toolsdir/ctools.c > CMakeFiles/rolo.dir/src/toolsdir/ctools.c.i

CMakeFiles/rolo.dir/src/toolsdir/ctools.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo.dir/src/toolsdir/ctools.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/src/toolsdir/ctools.c -o CMakeFiles/rolo.dir/src/toolsdir/ctools.c.s

CMakeFiles/rolo.dir/src/toolsdir/mem.c.o: CMakeFiles/rolo.dir/flags.make
CMakeFiles/rolo.dir/src/toolsdir/mem.c.o: ../src/toolsdir/mem.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/rolo.dir/src/toolsdir/mem.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo.dir/src/toolsdir/mem.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/src/toolsdir/mem.c

CMakeFiles/rolo.dir/src/toolsdir/mem.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo.dir/src/toolsdir/mem.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/src/toolsdir/mem.c > CMakeFiles/rolo.dir/src/toolsdir/mem.c.i

CMakeFiles/rolo.dir/src/toolsdir/mem.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo.dir/src/toolsdir/mem.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/src/toolsdir/mem.c -o CMakeFiles/rolo.dir/src/toolsdir/mem.c.s

CMakeFiles/rolo.dir/src/toolsdir/menu.c.o: CMakeFiles/rolo.dir/flags.make
CMakeFiles/rolo.dir/src/toolsdir/menu.c.o: ../src/toolsdir/menu.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/rolo.dir/src/toolsdir/menu.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo.dir/src/toolsdir/menu.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/src/toolsdir/menu.c

CMakeFiles/rolo.dir/src/toolsdir/menu.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo.dir/src/toolsdir/menu.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/src/toolsdir/menu.c > CMakeFiles/rolo.dir/src/toolsdir/menu.c.i

CMakeFiles/rolo.dir/src/toolsdir/menu.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo.dir/src/toolsdir/menu.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/src/toolsdir/menu.c -o CMakeFiles/rolo.dir/src/toolsdir/menu.c.s

CMakeFiles/rolo.dir/src/clear.c.o: CMakeFiles/rolo.dir/flags.make
CMakeFiles/rolo.dir/src/clear.c.o: ../src/clear.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/rolo.dir/src/clear.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo.dir/src/clear.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/src/clear.c

CMakeFiles/rolo.dir/src/clear.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo.dir/src/clear.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/src/clear.c > CMakeFiles/rolo.dir/src/clear.c.i

CMakeFiles/rolo.dir/src/clear.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo.dir/src/clear.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/src/clear.c -o CMakeFiles/rolo.dir/src/clear.c.s

CMakeFiles/rolo.dir/src/io.c.o: CMakeFiles/rolo.dir/flags.make
CMakeFiles/rolo.dir/src/io.c.o: ../src/io.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/rolo.dir/src/io.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo.dir/src/io.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/src/io.c

CMakeFiles/rolo.dir/src/io.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo.dir/src/io.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/src/io.c > CMakeFiles/rolo.dir/src/io.c.i

CMakeFiles/rolo.dir/src/io.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo.dir/src/io.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/src/io.c -o CMakeFiles/rolo.dir/src/io.c.s

CMakeFiles/rolo.dir/src/main.c.o: CMakeFiles/rolo.dir/flags.make
CMakeFiles/rolo.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/rolo.dir/src/main.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo.dir/src/main.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/src/main.c

CMakeFiles/rolo.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo.dir/src/main.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/src/main.c > CMakeFiles/rolo.dir/src/main.c.i

CMakeFiles/rolo.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo.dir/src/main.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/src/main.c -o CMakeFiles/rolo.dir/src/main.c.s

CMakeFiles/rolo.dir/src/menuaux.c.o: CMakeFiles/rolo.dir/flags.make
CMakeFiles/rolo.dir/src/menuaux.c.o: ../src/menuaux.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/rolo.dir/src/menuaux.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo.dir/src/menuaux.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/src/menuaux.c

CMakeFiles/rolo.dir/src/menuaux.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo.dir/src/menuaux.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/src/menuaux.c > CMakeFiles/rolo.dir/src/menuaux.c.i

CMakeFiles/rolo.dir/src/menuaux.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo.dir/src/menuaux.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/src/menuaux.c -o CMakeFiles/rolo.dir/src/menuaux.c.s

CMakeFiles/rolo.dir/src/operations.c.o: CMakeFiles/rolo.dir/flags.make
CMakeFiles/rolo.dir/src/operations.c.o: ../src/operations.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/rolo.dir/src/operations.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo.dir/src/operations.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/src/operations.c

CMakeFiles/rolo.dir/src/operations.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo.dir/src/operations.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/src/operations.c > CMakeFiles/rolo.dir/src/operations.c.i

CMakeFiles/rolo.dir/src/operations.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo.dir/src/operations.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/src/operations.c -o CMakeFiles/rolo.dir/src/operations.c.s

CMakeFiles/rolo.dir/src/options.c.o: CMakeFiles/rolo.dir/flags.make
CMakeFiles/rolo.dir/src/options.c.o: ../src/options.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/rolo.dir/src/options.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo.dir/src/options.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/src/options.c

CMakeFiles/rolo.dir/src/options.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo.dir/src/options.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/src/options.c > CMakeFiles/rolo.dir/src/options.c.i

CMakeFiles/rolo.dir/src/options.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo.dir/src/options.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/src/options.c -o CMakeFiles/rolo.dir/src/options.c.s

CMakeFiles/rolo.dir/src/rlist.c.o: CMakeFiles/rolo.dir/flags.make
CMakeFiles/rolo.dir/src/rlist.c.o: ../src/rlist.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/rolo.dir/src/rlist.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo.dir/src/rlist.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/src/rlist.c

CMakeFiles/rolo.dir/src/rlist.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo.dir/src/rlist.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/src/rlist.c > CMakeFiles/rolo.dir/src/rlist.c.i

CMakeFiles/rolo.dir/src/rlist.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo.dir/src/rlist.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/src/rlist.c -o CMakeFiles/rolo.dir/src/rlist.c.s

CMakeFiles/rolo.dir/src/rolo.c.o: CMakeFiles/rolo.dir/flags.make
CMakeFiles/rolo.dir/src/rolo.c.o: ../src/rolo.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object CMakeFiles/rolo.dir/src/rolo.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo.dir/src/rolo.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/src/rolo.c

CMakeFiles/rolo.dir/src/rolo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo.dir/src/rolo.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/src/rolo.c > CMakeFiles/rolo.dir/src/rolo.c.i

CMakeFiles/rolo.dir/src/rolo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo.dir/src/rolo.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/src/rolo.c -o CMakeFiles/rolo.dir/src/rolo.c.s

CMakeFiles/rolo.dir/src/search.c.o: CMakeFiles/rolo.dir/flags.make
CMakeFiles/rolo.dir/src/search.c.o: ../src/search.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building C object CMakeFiles/rolo.dir/src/search.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo.dir/src/search.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/src/search.c

CMakeFiles/rolo.dir/src/search.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo.dir/src/search.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/src/search.c > CMakeFiles/rolo.dir/src/search.c.i

CMakeFiles/rolo.dir/src/search.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo.dir/src/search.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/src/search.c -o CMakeFiles/rolo.dir/src/search.c.s

CMakeFiles/rolo.dir/src/update.c.o: CMakeFiles/rolo.dir/flags.make
CMakeFiles/rolo.dir/src/update.c.o: ../src/update.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building C object CMakeFiles/rolo.dir/src/update.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rolo.dir/src/update.c.o   -c /Users/zhengyu/Desktop/CSE320/hw2/src/update.c

CMakeFiles/rolo.dir/src/update.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rolo.dir/src/update.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhengyu/Desktop/CSE320/hw2/src/update.c > CMakeFiles/rolo.dir/src/update.c.i

CMakeFiles/rolo.dir/src/update.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rolo.dir/src/update.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhengyu/Desktop/CSE320/hw2/src/update.c -o CMakeFiles/rolo.dir/src/update.c.s

# Object files for target rolo
rolo_OBJECTS = \
"CMakeFiles/rolo.dir/src/toolsdir/args.c.o" \
"CMakeFiles/rolo.dir/src/toolsdir/ctools.c.o" \
"CMakeFiles/rolo.dir/src/toolsdir/mem.c.o" \
"CMakeFiles/rolo.dir/src/toolsdir/menu.c.o" \
"CMakeFiles/rolo.dir/src/clear.c.o" \
"CMakeFiles/rolo.dir/src/io.c.o" \
"CMakeFiles/rolo.dir/src/main.c.o" \
"CMakeFiles/rolo.dir/src/menuaux.c.o" \
"CMakeFiles/rolo.dir/src/operations.c.o" \
"CMakeFiles/rolo.dir/src/options.c.o" \
"CMakeFiles/rolo.dir/src/rlist.c.o" \
"CMakeFiles/rolo.dir/src/rolo.c.o" \
"CMakeFiles/rolo.dir/src/search.c.o" \
"CMakeFiles/rolo.dir/src/update.c.o"

# External object files for target rolo
rolo_EXTERNAL_OBJECTS =

rolo: CMakeFiles/rolo.dir/src/toolsdir/args.c.o
rolo: CMakeFiles/rolo.dir/src/toolsdir/ctools.c.o
rolo: CMakeFiles/rolo.dir/src/toolsdir/mem.c.o
rolo: CMakeFiles/rolo.dir/src/toolsdir/menu.c.o
rolo: CMakeFiles/rolo.dir/src/clear.c.o
rolo: CMakeFiles/rolo.dir/src/io.c.o
rolo: CMakeFiles/rolo.dir/src/main.c.o
rolo: CMakeFiles/rolo.dir/src/menuaux.c.o
rolo: CMakeFiles/rolo.dir/src/operations.c.o
rolo: CMakeFiles/rolo.dir/src/options.c.o
rolo: CMakeFiles/rolo.dir/src/rlist.c.o
rolo: CMakeFiles/rolo.dir/src/rolo.c.o
rolo: CMakeFiles/rolo.dir/src/search.c.o
rolo: CMakeFiles/rolo.dir/src/update.c.o
rolo: CMakeFiles/rolo.dir/build.make
rolo: /usr/local/Cellar/criterion/2.3.3/lib/libcriterion.dylib
rolo: CMakeFiles/rolo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Linking C executable rolo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rolo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rolo.dir/build: rolo

.PHONY : CMakeFiles/rolo.dir/build

CMakeFiles/rolo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rolo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rolo.dir/clean

CMakeFiles/rolo.dir/depend:
	cd /Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/zhengyu/Desktop/CSE320/hw2 /Users/zhengyu/Desktop/CSE320/hw2 /Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug /Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug /Users/zhengyu/Desktop/CSE320/hw2/cmake-build-debug/CMakeFiles/rolo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rolo.dir/depend

