# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.28

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\GitHub\max-s-bundle

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\GitHub\max-s-bundle\build

# Include any dependencies generated for this target.
include CMakeFiles/MSB.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MSB.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MSB.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MSB.dir/flags.make

CMakeFiles/MSB.dir/MSB_liuyang.cpp.obj: CMakeFiles/MSB.dir/flags.make
CMakeFiles/MSB.dir/MSB_liuyang.cpp.obj: CMakeFiles/MSB.dir/includes_CXX.rsp
CMakeFiles/MSB.dir/MSB_liuyang.cpp.obj: E:/GitHub/max-s-bundle/MSB_liuyang.cpp
CMakeFiles/MSB.dir/MSB_liuyang.cpp.obj: CMakeFiles/MSB.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=E:\GitHub\max-s-bundle\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MSB.dir/MSB_liuyang.cpp.obj"
	"E:\application 2\x86_64-8.1.0-release-posix-sjlj-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MSB.dir/MSB_liuyang.cpp.obj -MF CMakeFiles\MSB.dir\MSB_liuyang.cpp.obj.d -o CMakeFiles\MSB.dir\MSB_liuyang.cpp.obj -c E:\GitHub\max-s-bundle\MSB_liuyang.cpp

CMakeFiles/MSB.dir/MSB_liuyang.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/MSB.dir/MSB_liuyang.cpp.i"
	"E:\application 2\x86_64-8.1.0-release-posix-sjlj-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\GitHub\max-s-bundle\MSB_liuyang.cpp > CMakeFiles\MSB.dir\MSB_liuyang.cpp.i

CMakeFiles/MSB.dir/MSB_liuyang.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/MSB.dir/MSB_liuyang.cpp.s"
	"E:\application 2\x86_64-8.1.0-release-posix-sjlj-rt_v6-rev0\mingw64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\GitHub\max-s-bundle\MSB_liuyang.cpp -o CMakeFiles\MSB.dir\MSB_liuyang.cpp.s

# Object files for target MSB
MSB_OBJECTS = \
"CMakeFiles/MSB.dir/MSB_liuyang.cpp.obj"

# External object files for target MSB
MSB_EXTERNAL_OBJECTS =

E:/GitHub/max-s-bundle/bin/MSB.exe: CMakeFiles/MSB.dir/MSB_liuyang.cpp.obj
E:/GitHub/max-s-bundle/bin/MSB.exe: CMakeFiles/MSB.dir/build.make
E:/GitHub/max-s-bundle/bin/MSB.exe: CMakeFiles/MSB.dir/linkLibs.rsp
E:/GitHub/max-s-bundle/bin/MSB.exe: CMakeFiles/MSB.dir/objects1.rsp
E:/GitHub/max-s-bundle/bin/MSB.exe: CMakeFiles/MSB.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=E:\GitHub\max-s-bundle\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable E:\GitHub\max-s-bundle\bin\MSB.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\MSB.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MSB.dir/build: E:/GitHub/max-s-bundle/bin/MSB.exe
.PHONY : CMakeFiles/MSB.dir/build

CMakeFiles/MSB.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\MSB.dir\cmake_clean.cmake
.PHONY : CMakeFiles/MSB.dir/clean

CMakeFiles/MSB.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\GitHub\max-s-bundle E:\GitHub\max-s-bundle E:\GitHub\max-s-bundle\build E:\GitHub\max-s-bundle\build E:\GitHub\max-s-bundle\build\CMakeFiles\MSB.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/MSB.dir/depend

