# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = D:\mingw64\bin\cmake.exe

# The command to remove a file.
RM = D:\mingw64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\Code\Cpp\2024 Fall Data Structure Code\Project_2\MapSystem"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\Code\Cpp\2024 Fall Data Structure Code\Project_2\MapSystem\build"

# Utility rule file for MyQtProject_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/MyQtProject_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MyQtProject_autogen.dir/progress.make

CMakeFiles/MyQtProject_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir="D:\Code\Cpp\2024 Fall Data Structure Code\Project_2\MapSystem\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC for target MyQtProject"
	D:\mingw64\bin\cmake.exe -E cmake_autogen "D:/Code/Cpp/2024 Fall Data Structure Code/Project_2/MapSystem/build/CMakeFiles/MyQtProject_autogen.dir/AutogenInfo.json" Debug

MyQtProject_autogen: CMakeFiles/MyQtProject_autogen
MyQtProject_autogen: CMakeFiles/MyQtProject_autogen.dir/build.make
.PHONY : MyQtProject_autogen

# Rule to build all files generated by this target.
CMakeFiles/MyQtProject_autogen.dir/build: MyQtProject_autogen
.PHONY : CMakeFiles/MyQtProject_autogen.dir/build

CMakeFiles/MyQtProject_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\MyQtProject_autogen.dir\cmake_clean.cmake
.PHONY : CMakeFiles/MyQtProject_autogen.dir/clean

CMakeFiles/MyQtProject_autogen.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\Code\Cpp\2024 Fall Data Structure Code\Project_2\MapSystem" "D:\Code\Cpp\2024 Fall Data Structure Code\Project_2\MapSystem" "D:\Code\Cpp\2024 Fall Data Structure Code\Project_2\MapSystem\build" "D:\Code\Cpp\2024 Fall Data Structure Code\Project_2\MapSystem\build" "D:\Code\Cpp\2024 Fall Data Structure Code\Project_2\MapSystem\build\CMakeFiles\MyQtProject_autogen.dir\DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/MyQtProject_autogen.dir/depend

