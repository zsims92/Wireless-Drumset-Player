# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/mfay/UNL/advembbedded/workspace/project_drummiest/communication/Receiver2DrumSticks

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mfay/UNL/advembbedded/workspace/project_drummiest/communication/Receiver2DrumSticks/build

# Include any dependencies generated for this target.
include CMakeFiles/receiver.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/receiver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/receiver.dir/flags.make

CMakeFiles/receiver.dir/receive.c.o: CMakeFiles/receiver.dir/flags.make
CMakeFiles/receiver.dir/receive.c.o: ../receive.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mfay/UNL/advembbedded/workspace/project_drummiest/communication/Receiver2DrumSticks/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/receiver.dir/receive.c.o"
	msp430-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/receiver.dir/receive.c.o   -c /home/mfay/UNL/advembbedded/workspace/project_drummiest/communication/Receiver2DrumSticks/receive.c

CMakeFiles/receiver.dir/receive.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/receiver.dir/receive.c.i"
	msp430-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/mfay/UNL/advembbedded/workspace/project_drummiest/communication/Receiver2DrumSticks/receive.c > CMakeFiles/receiver.dir/receive.c.i

CMakeFiles/receiver.dir/receive.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/receiver.dir/receive.c.s"
	msp430-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/mfay/UNL/advembbedded/workspace/project_drummiest/communication/Receiver2DrumSticks/receive.c -o CMakeFiles/receiver.dir/receive.c.s

CMakeFiles/receiver.dir/receive.c.o.requires:
.PHONY : CMakeFiles/receiver.dir/receive.c.o.requires

CMakeFiles/receiver.dir/receive.c.o.provides: CMakeFiles/receiver.dir/receive.c.o.requires
	$(MAKE) -f CMakeFiles/receiver.dir/build.make CMakeFiles/receiver.dir/receive.c.o.provides.build
.PHONY : CMakeFiles/receiver.dir/receive.c.o.provides

CMakeFiles/receiver.dir/receive.c.o.provides.build: CMakeFiles/receiver.dir/receive.c.o

# Object files for target receiver
receiver_OBJECTS = \
"CMakeFiles/receiver.dir/receive.c.o"

# External object files for target receiver
receiver_EXTERNAL_OBJECTS =

receiver: CMakeFiles/receiver.dir/receive.c.o
receiver: CMakeFiles/receiver.dir/build.make
receiver: CMakeFiles/receiver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable receiver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/receiver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/receiver.dir/build: receiver
.PHONY : CMakeFiles/receiver.dir/build

CMakeFiles/receiver.dir/requires: CMakeFiles/receiver.dir/receive.c.o.requires
.PHONY : CMakeFiles/receiver.dir/requires

CMakeFiles/receiver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/receiver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/receiver.dir/clean

CMakeFiles/receiver.dir/depend:
	cd /home/mfay/UNL/advembbedded/workspace/project_drummiest/communication/Receiver2DrumSticks/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mfay/UNL/advembbedded/workspace/project_drummiest/communication/Receiver2DrumSticks /home/mfay/UNL/advembbedded/workspace/project_drummiest/communication/Receiver2DrumSticks /home/mfay/UNL/advembbedded/workspace/project_drummiest/communication/Receiver2DrumSticks/build /home/mfay/UNL/advembbedded/workspace/project_drummiest/communication/Receiver2DrumSticks/build /home/mfay/UNL/advembbedded/workspace/project_drummiest/communication/Receiver2DrumSticks/build/CMakeFiles/receiver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/receiver.dir/depend

