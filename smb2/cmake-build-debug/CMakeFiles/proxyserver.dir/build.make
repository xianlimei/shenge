# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /home/ted/ted/clion/clion-2018.1.2/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/ted/ted/clion/clion-2018.1.2/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ted/projects/smb1/smb2_analyse-master/program/smb2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ted/projects/smb1/smb2_analyse-master/program/smb2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/proxyserver.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/proxyserver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/proxyserver.dir/flags.make

CMakeFiles/proxyserver.dir/smb2Name.c.o: CMakeFiles/proxyserver.dir/flags.make
CMakeFiles/proxyserver.dir/smb2Name.c.o: ../smb2Name.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ted/projects/smb1/smb2_analyse-master/program/smb2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/proxyserver.dir/smb2Name.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proxyserver.dir/smb2Name.c.o   -c /home/ted/projects/smb1/smb2_analyse-master/program/smb2/smb2Name.c

CMakeFiles/proxyserver.dir/smb2Name.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proxyserver.dir/smb2Name.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ted/projects/smb1/smb2_analyse-master/program/smb2/smb2Name.c > CMakeFiles/proxyserver.dir/smb2Name.c.i

CMakeFiles/proxyserver.dir/smb2Name.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proxyserver.dir/smb2Name.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ted/projects/smb1/smb2_analyse-master/program/smb2/smb2Name.c -o CMakeFiles/proxyserver.dir/smb2Name.c.s

CMakeFiles/proxyserver.dir/smb2Name.c.o.requires:

.PHONY : CMakeFiles/proxyserver.dir/smb2Name.c.o.requires

CMakeFiles/proxyserver.dir/smb2Name.c.o.provides: CMakeFiles/proxyserver.dir/smb2Name.c.o.requires
	$(MAKE) -f CMakeFiles/proxyserver.dir/build.make CMakeFiles/proxyserver.dir/smb2Name.c.o.provides.build
.PHONY : CMakeFiles/proxyserver.dir/smb2Name.c.o.provides

CMakeFiles/proxyserver.dir/smb2Name.c.o.provides.build: CMakeFiles/proxyserver.dir/smb2Name.c.o


CMakeFiles/proxyserver.dir/smb2tools.c.o: CMakeFiles/proxyserver.dir/flags.make
CMakeFiles/proxyserver.dir/smb2tools.c.o: ../smb2tools.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ted/projects/smb1/smb2_analyse-master/program/smb2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/proxyserver.dir/smb2tools.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proxyserver.dir/smb2tools.c.o   -c /home/ted/projects/smb1/smb2_analyse-master/program/smb2/smb2tools.c

CMakeFiles/proxyserver.dir/smb2tools.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proxyserver.dir/smb2tools.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ted/projects/smb1/smb2_analyse-master/program/smb2/smb2tools.c > CMakeFiles/proxyserver.dir/smb2tools.c.i

CMakeFiles/proxyserver.dir/smb2tools.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proxyserver.dir/smb2tools.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ted/projects/smb1/smb2_analyse-master/program/smb2/smb2tools.c -o CMakeFiles/proxyserver.dir/smb2tools.c.s

CMakeFiles/proxyserver.dir/smb2tools.c.o.requires:

.PHONY : CMakeFiles/proxyserver.dir/smb2tools.c.o.requires

CMakeFiles/proxyserver.dir/smb2tools.c.o.provides: CMakeFiles/proxyserver.dir/smb2tools.c.o.requires
	$(MAKE) -f CMakeFiles/proxyserver.dir/build.make CMakeFiles/proxyserver.dir/smb2tools.c.o.provides.build
.PHONY : CMakeFiles/proxyserver.dir/smb2tools.c.o.provides

CMakeFiles/proxyserver.dir/smb2tools.c.o.provides.build: CMakeFiles/proxyserver.dir/smb2tools.c.o


CMakeFiles/proxyserver.dir/short_name_list.c.o: CMakeFiles/proxyserver.dir/flags.make
CMakeFiles/proxyserver.dir/short_name_list.c.o: ../short_name_list.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ted/projects/smb1/smb2_analyse-master/program/smb2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/proxyserver.dir/short_name_list.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proxyserver.dir/short_name_list.c.o   -c /home/ted/projects/smb1/smb2_analyse-master/program/smb2/short_name_list.c

CMakeFiles/proxyserver.dir/short_name_list.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proxyserver.dir/short_name_list.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ted/projects/smb1/smb2_analyse-master/program/smb2/short_name_list.c > CMakeFiles/proxyserver.dir/short_name_list.c.i

CMakeFiles/proxyserver.dir/short_name_list.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proxyserver.dir/short_name_list.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ted/projects/smb1/smb2_analyse-master/program/smb2/short_name_list.c -o CMakeFiles/proxyserver.dir/short_name_list.c.s

CMakeFiles/proxyserver.dir/short_name_list.c.o.requires:

.PHONY : CMakeFiles/proxyserver.dir/short_name_list.c.o.requires

CMakeFiles/proxyserver.dir/short_name_list.c.o.provides: CMakeFiles/proxyserver.dir/short_name_list.c.o.requires
	$(MAKE) -f CMakeFiles/proxyserver.dir/build.make CMakeFiles/proxyserver.dir/short_name_list.c.o.provides.build
.PHONY : CMakeFiles/proxyserver.dir/short_name_list.c.o.provides

CMakeFiles/proxyserver.dir/short_name_list.c.o.provides.build: CMakeFiles/proxyserver.dir/short_name_list.c.o


CMakeFiles/proxyserver.dir/proxyserver1.c.o: CMakeFiles/proxyserver.dir/flags.make
CMakeFiles/proxyserver.dir/proxyserver1.c.o: ../proxyserver1.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ted/projects/smb1/smb2_analyse-master/program/smb2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/proxyserver.dir/proxyserver1.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proxyserver.dir/proxyserver1.c.o   -c /home/ted/projects/smb1/smb2_analyse-master/program/smb2/proxyserver1.c

CMakeFiles/proxyserver.dir/proxyserver1.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proxyserver.dir/proxyserver1.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ted/projects/smb1/smb2_analyse-master/program/smb2/proxyserver1.c > CMakeFiles/proxyserver.dir/proxyserver1.c.i

CMakeFiles/proxyserver.dir/proxyserver1.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proxyserver.dir/proxyserver1.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ted/projects/smb1/smb2_analyse-master/program/smb2/proxyserver1.c -o CMakeFiles/proxyserver.dir/proxyserver1.c.s

CMakeFiles/proxyserver.dir/proxyserver1.c.o.requires:

.PHONY : CMakeFiles/proxyserver.dir/proxyserver1.c.o.requires

CMakeFiles/proxyserver.dir/proxyserver1.c.o.provides: CMakeFiles/proxyserver.dir/proxyserver1.c.o.requires
	$(MAKE) -f CMakeFiles/proxyserver.dir/build.make CMakeFiles/proxyserver.dir/proxyserver1.c.o.provides.build
.PHONY : CMakeFiles/proxyserver.dir/proxyserver1.c.o.provides

CMakeFiles/proxyserver.dir/proxyserver1.c.o.provides.build: CMakeFiles/proxyserver.dir/proxyserver1.c.o


CMakeFiles/proxyserver.dir/smb_session_list.c.o: CMakeFiles/proxyserver.dir/flags.make
CMakeFiles/proxyserver.dir/smb_session_list.c.o: ../smb_session_list.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ted/projects/smb1/smb2_analyse-master/program/smb2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/proxyserver.dir/smb_session_list.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proxyserver.dir/smb_session_list.c.o   -c /home/ted/projects/smb1/smb2_analyse-master/program/smb2/smb_session_list.c

CMakeFiles/proxyserver.dir/smb_session_list.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proxyserver.dir/smb_session_list.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ted/projects/smb1/smb2_analyse-master/program/smb2/smb_session_list.c > CMakeFiles/proxyserver.dir/smb_session_list.c.i

CMakeFiles/proxyserver.dir/smb_session_list.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proxyserver.dir/smb_session_list.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ted/projects/smb1/smb2_analyse-master/program/smb2/smb_session_list.c -o CMakeFiles/proxyserver.dir/smb_session_list.c.s

CMakeFiles/proxyserver.dir/smb_session_list.c.o.requires:

.PHONY : CMakeFiles/proxyserver.dir/smb_session_list.c.o.requires

CMakeFiles/proxyserver.dir/smb_session_list.c.o.provides: CMakeFiles/proxyserver.dir/smb_session_list.c.o.requires
	$(MAKE) -f CMakeFiles/proxyserver.dir/build.make CMakeFiles/proxyserver.dir/smb_session_list.c.o.provides.build
.PHONY : CMakeFiles/proxyserver.dir/smb_session_list.c.o.provides

CMakeFiles/proxyserver.dir/smb_session_list.c.o.provides.build: CMakeFiles/proxyserver.dir/smb_session_list.c.o


CMakeFiles/proxyserver.dir/unicode2utf8.c.o: CMakeFiles/proxyserver.dir/flags.make
CMakeFiles/proxyserver.dir/unicode2utf8.c.o: ../unicode2utf8.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ted/projects/smb1/smb2_analyse-master/program/smb2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/proxyserver.dir/unicode2utf8.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proxyserver.dir/unicode2utf8.c.o   -c /home/ted/projects/smb1/smb2_analyse-master/program/smb2/unicode2utf8.c

CMakeFiles/proxyserver.dir/unicode2utf8.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proxyserver.dir/unicode2utf8.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ted/projects/smb1/smb2_analyse-master/program/smb2/unicode2utf8.c > CMakeFiles/proxyserver.dir/unicode2utf8.c.i

CMakeFiles/proxyserver.dir/unicode2utf8.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proxyserver.dir/unicode2utf8.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ted/projects/smb1/smb2_analyse-master/program/smb2/unicode2utf8.c -o CMakeFiles/proxyserver.dir/unicode2utf8.c.s

CMakeFiles/proxyserver.dir/unicode2utf8.c.o.requires:

.PHONY : CMakeFiles/proxyserver.dir/unicode2utf8.c.o.requires

CMakeFiles/proxyserver.dir/unicode2utf8.c.o.provides: CMakeFiles/proxyserver.dir/unicode2utf8.c.o.requires
	$(MAKE) -f CMakeFiles/proxyserver.dir/build.make CMakeFiles/proxyserver.dir/unicode2utf8.c.o.provides.build
.PHONY : CMakeFiles/proxyserver.dir/unicode2utf8.c.o.provides

CMakeFiles/proxyserver.dir/unicode2utf8.c.o.provides.build: CMakeFiles/proxyserver.dir/unicode2utf8.c.o


CMakeFiles/proxyserver.dir/fname_decode.c.o: CMakeFiles/proxyserver.dir/flags.make
CMakeFiles/proxyserver.dir/fname_decode.c.o: ../fname_decode.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ted/projects/smb1/smb2_analyse-master/program/smb2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/proxyserver.dir/fname_decode.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proxyserver.dir/fname_decode.c.o   -c /home/ted/projects/smb1/smb2_analyse-master/program/smb2/fname_decode.c

CMakeFiles/proxyserver.dir/fname_decode.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proxyserver.dir/fname_decode.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ted/projects/smb1/smb2_analyse-master/program/smb2/fname_decode.c > CMakeFiles/proxyserver.dir/fname_decode.c.i

CMakeFiles/proxyserver.dir/fname_decode.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proxyserver.dir/fname_decode.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ted/projects/smb1/smb2_analyse-master/program/smb2/fname_decode.c -o CMakeFiles/proxyserver.dir/fname_decode.c.s

CMakeFiles/proxyserver.dir/fname_decode.c.o.requires:

.PHONY : CMakeFiles/proxyserver.dir/fname_decode.c.o.requires

CMakeFiles/proxyserver.dir/fname_decode.c.o.provides: CMakeFiles/proxyserver.dir/fname_decode.c.o.requires
	$(MAKE) -f CMakeFiles/proxyserver.dir/build.make CMakeFiles/proxyserver.dir/fname_decode.c.o.provides.build
.PHONY : CMakeFiles/proxyserver.dir/fname_decode.c.o.provides

CMakeFiles/proxyserver.dir/fname_decode.c.o.provides.build: CMakeFiles/proxyserver.dir/fname_decode.c.o


# Object files for target proxyserver
proxyserver_OBJECTS = \
"CMakeFiles/proxyserver.dir/smb2Name.c.o" \
"CMakeFiles/proxyserver.dir/smb2tools.c.o" \
"CMakeFiles/proxyserver.dir/short_name_list.c.o" \
"CMakeFiles/proxyserver.dir/proxyserver1.c.o" \
"CMakeFiles/proxyserver.dir/smb_session_list.c.o" \
"CMakeFiles/proxyserver.dir/unicode2utf8.c.o" \
"CMakeFiles/proxyserver.dir/fname_decode.c.o"

# External object files for target proxyserver
proxyserver_EXTERNAL_OBJECTS =

proxyserver: CMakeFiles/proxyserver.dir/smb2Name.c.o
proxyserver: CMakeFiles/proxyserver.dir/smb2tools.c.o
proxyserver: CMakeFiles/proxyserver.dir/short_name_list.c.o
proxyserver: CMakeFiles/proxyserver.dir/proxyserver1.c.o
proxyserver: CMakeFiles/proxyserver.dir/smb_session_list.c.o
proxyserver: CMakeFiles/proxyserver.dir/unicode2utf8.c.o
proxyserver: CMakeFiles/proxyserver.dir/fname_decode.c.o
proxyserver: CMakeFiles/proxyserver.dir/build.make
proxyserver: CMakeFiles/proxyserver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ted/projects/smb1/smb2_analyse-master/program/smb2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C executable proxyserver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/proxyserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/proxyserver.dir/build: proxyserver

.PHONY : CMakeFiles/proxyserver.dir/build

CMakeFiles/proxyserver.dir/requires: CMakeFiles/proxyserver.dir/smb2Name.c.o.requires
CMakeFiles/proxyserver.dir/requires: CMakeFiles/proxyserver.dir/smb2tools.c.o.requires
CMakeFiles/proxyserver.dir/requires: CMakeFiles/proxyserver.dir/short_name_list.c.o.requires
CMakeFiles/proxyserver.dir/requires: CMakeFiles/proxyserver.dir/proxyserver1.c.o.requires
CMakeFiles/proxyserver.dir/requires: CMakeFiles/proxyserver.dir/smb_session_list.c.o.requires
CMakeFiles/proxyserver.dir/requires: CMakeFiles/proxyserver.dir/unicode2utf8.c.o.requires
CMakeFiles/proxyserver.dir/requires: CMakeFiles/proxyserver.dir/fname_decode.c.o.requires

.PHONY : CMakeFiles/proxyserver.dir/requires

CMakeFiles/proxyserver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/proxyserver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/proxyserver.dir/clean

CMakeFiles/proxyserver.dir/depend:
	cd /home/ted/projects/smb1/smb2_analyse-master/program/smb2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ted/projects/smb1/smb2_analyse-master/program/smb2 /home/ted/projects/smb1/smb2_analyse-master/program/smb2 /home/ted/projects/smb1/smb2_analyse-master/program/smb2/cmake-build-debug /home/ted/projects/smb1/smb2_analyse-master/program/smb2/cmake-build-debug /home/ted/projects/smb1/smb2_analyse-master/program/smb2/cmake-build-debug/CMakeFiles/proxyserver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/proxyserver.dir/depend

