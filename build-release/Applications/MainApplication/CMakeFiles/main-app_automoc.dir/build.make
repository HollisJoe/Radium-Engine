# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /export/home/kahlo/aghazanf/Radium-Engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /export/home/kahlo/aghazanf/Radium-Engine/build-release

# Utility rule file for main-app_automoc.

# Include the progress variables for this target.
include Applications/MainApplication/CMakeFiles/main-app_automoc.dir/progress.make

Applications/MainApplication/CMakeFiles/main-app_automoc:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/export/home/kahlo/aghazanf/Radium-Engine/build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic moc and rcc for target main-app"
	cd /export/home/kahlo/aghazanf/Radium-Engine/build-release/Applications/MainApplication && /usr/bin/cmake -E cmake_autogen /export/home/kahlo/aghazanf/Radium-Engine/build-release/Applications/MainApplication/CMakeFiles/main-app_automoc.dir/ Release

main-app_automoc: Applications/MainApplication/CMakeFiles/main-app_automoc
main-app_automoc: Applications/MainApplication/CMakeFiles/main-app_automoc.dir/build.make

.PHONY : main-app_automoc

# Rule to build all files generated by this target.
Applications/MainApplication/CMakeFiles/main-app_automoc.dir/build: main-app_automoc

.PHONY : Applications/MainApplication/CMakeFiles/main-app_automoc.dir/build

Applications/MainApplication/CMakeFiles/main-app_automoc.dir/clean:
	cd /export/home/kahlo/aghazanf/Radium-Engine/build-release/Applications/MainApplication && $(CMAKE_COMMAND) -P CMakeFiles/main-app_automoc.dir/cmake_clean.cmake
.PHONY : Applications/MainApplication/CMakeFiles/main-app_automoc.dir/clean

Applications/MainApplication/CMakeFiles/main-app_automoc.dir/depend:
	cd /export/home/kahlo/aghazanf/Radium-Engine/build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /export/home/kahlo/aghazanf/Radium-Engine /export/home/kahlo/aghazanf/Radium-Engine/Applications/MainApplication /export/home/kahlo/aghazanf/Radium-Engine/build-release /export/home/kahlo/aghazanf/Radium-Engine/build-release/Applications/MainApplication /export/home/kahlo/aghazanf/Radium-Engine/build-release/Applications/MainApplication/CMakeFiles/main-app_automoc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Applications/MainApplication/CMakeFiles/main-app_automoc.dir/depend

