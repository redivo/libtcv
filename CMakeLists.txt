##
# Libtcv & test
#
# cmake -DCMAKE_BUILD_TYPE=<BUILD_TYPE> <OPTIONS> <PROJECT_DIR>
#
# BUILD_TYPE:
#  Debug Release RelWithDebInfo
#
# Options:
# -DBUILD_DEMO=On
# -DBUILD_TEST=OFF
# -DPROFILE=Off
# -DTEST_COVERAGE=Off
#
# To choose different compiler: (target)
# cmake  -DCMAKE_C_COMPILER=ppc_6xx-gcc
#        -DCMAKE_CXX_COMPILER=ppc_6xx-g++
##
IF(NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Debug
       CACHE STRING "Choose the type of build :
                     Debug Release RelWithDebInfo"
       FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)
message(" Configuring for : ${CMAKE_BUILD_TYPE}")


# minumum cmake version (required to have a good cmakelists.txt)
cmake_minimum_required (VERSION 2.8)
# Project Name
project (libtcv)
# Make will run in verbose mode - for debugging makefiles
# SET(CMAKE_VERBOSE_MAKEFILE On)
# Enable ExternalProject CMake module
include(ExternalProject)

################################################################################
# Output configuration
################################################################################


option(BUILD_DEMO "Build Demonstration" ON)
option(BUILD_TEST "Build Tests" OFF)
option(PROFILE "Build with Profiling" OFF)
option(TEST_COVERAGE "Test Coverage" OFF)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(SUFFIX "_dbg")
endif()

# How to call the executable - Some executable to run the lib
SET(BINARY_NAME "LibTCV-Demo${SUFFIX}")
# Name of binary compiled with coverage and unit-test framework
SET(TEST_BINARY_NAME "Libtcv_test")
# Static Library containig all classes of project without main()
SET(LIBRARY_NAME  "tcv${SUFFIX}")

# Version for libraries
SET(VERSION "0.1")

# Folder for binaries
SET(BIN_DIR	"bin/")
# Folder for libraries
SET(LIB_DIR	"lib/")
# Folder for headers
SET(INC_DIR	"include/")

# where the result is to be installed on the target:
# ${PREFIX}/include/${TARGET_SUBDIR_NAME} (contains library headers)
# ${PREFIX}/lib/${TARGET_SUBDIR_NAME} (library)
SET(TARGET_SUBDIR_NAME  "libtcv")


################################################################################
# Input configuration
################################################################################
SET(PROJECT_INCLUDE_DIR "include")

# all include files
include_directories (${PROJECT_INCLUDE_DIR})

# Sources
add_subdirectory(${PROJECT_SOURCE_DIR}/src/)

#where to look for source files of unit tests
add_subdirectory(${PROJECT_SOURCE_DIR}/tests/)


################################################################################
# Check System and resolve libraries
################################################################################

# Check Compiler version
# borrowed from http://stackoverflow.com/questions/4058565/check-gcc-minor-in-cmake
execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE COMPILER_VERSION)

message(STATUS ${CMAKE_CXX_COMPILER_ID})

# GCC specifics for C++11
if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" )
  message(STATUS "Using gnu compiler")

  if (COMPILER_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
	message(STATUS "C++11 activated.")
	SET(CXX11 "-std=c++11" CACHE STRING "C++11 compiler standard selector" FORCE)
  elseif (COMPILER_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3)
	message(WARNING "Preview C++0x - your compiler may not have full C++11 support")
	SET(CXX11 "-std=gnu++0x"  CACHE STRING "C++11 compiler standard selector" FORCE)
  else ()
	message(WARNING "Your GCC is quite old - C++11 will not work!")
  endif()

  # input Library for profiling is not found by find_library
  # find_library(COVERAGE_LIB gcov /usr/lib/gcc/x86_64-linux-gnu/4.8 REQUIRED)
  set(COVERAGE_LIB gcov)

  if(TEST_COVERAGE)
	message(STATUS "setting GCC coverage flags")
    SET(CMAKE_C_FLAGS     " ${CMAKE_C_FLAGS} -g -fprofile-arcs -ftest-coverage ")
    SET(CMAKE_CXX_FLAGS   " ${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage ")
    SET(CMAKE_LINK_FLAGS  " ${CMAKE_LINK_FLAGS} -O0 -g ")
  endif()


#Clang specific settings for C++11
elseif( ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" )
  message(STATUS "Using clang compiler")
  if (COMPILER_VERSION VERSION_GREATER 2.9 OR GCC_VERSION VERSION_EQUAL 2.9)
	message(STATUS "C++11 activated.")
	SET(CXX11 "-std=c++11")
  endif()

  # LLVM provides code coverage in libprofile_rt 
  if(TEST_COVERAGE)
	message(STATUS "setting clang/llvm coverage flags")
    SET(CMAKE_C_FLAGS     " ${CMAKE_C_FLAGS} --coverage ")
    SET(CMAKE_CXX_FLAGS   " ${CMAKE_CXX_FLAGS} --coverage ")
    SET(CMAKE_LINK_FLAGS  " ${CMAKE_LINK_FLAGS} --coverage ")
  endif()

endif()

# Threading library for gtest
find_library(THREAD_LIB pthread)


#--------------------------------------------------------------------------------
# Google Test as external project
# Why - see http://code.google.com/p/googletest/wiki/FAQ#
# How - borrowed form stackexchange
#		(http://stackoverflow.com/questions/9689183/cmake-googletest)
# http://www.kitware.com/media/html/BuildingExternalProjectsWithCMake2.8.html
#--------------------------------------------------------------------------------
if(BUILD_TEST)
    ExternalProject_Add(
	googletest
	SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
	SVN_REVISION -r 687
	TIMEOUT 10
	# PATCH_COMMMAND "" # Thomas: empty patch we dont patch google test
	#PATCH_COMMAND svn patch ${CMAKE_SOURCE_DIR}/gtest.patch ${CMAKE_BINARY_DIR}/ThirdParty/src/googletest
	# Force separate output paths for debug and release builds to allow easy
	# identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
	#CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	# -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
	# -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
	# -Dgtest_force_shared_crt=ON
	# Disable install step
	INSTALL_COMMAND ""
	# Empty update command to speed up make - we don't need to constantly check
 	UPDATE_COMMAND ""
 	)
endif(BUILD_TEST)
################################################################################
# Compiler & Linker  configuration
################################################################################

SET(GCC_WARNING_FLAGS   " -Wall -Wextra -fmessage-length=0 -Wno-unused-parameter " )

SET(CMAKE_C_FLAGS       " ${CMAKE_C_FLAGS} ${GCC_WARNING_FLAGS} ")
SET(CMAKE_CXX_FLAGS     " ${CXX11} ${CMAKE_CXX_FLAGS} ${GCC_WARNING_FLAGS} ")

SET(CMAKE_CXX_FLAGS_RELEASE  " ${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections -fdata-sections ${CXX11}")
SET(CMAKE_C_FLAGS_RELEASE  " ${CMAKE_C_FLAGS_RELEASE} -ffunction-sections -fdata-sections ")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE 
    " ${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--gc-sections -Wl,-Map,${PROJECT_NAME}.map ")

if(PROFILE)
    SET(CMAKE_C_FLAGS      " ${CMAKE_C_FLAGS} -g  -pg ")
    SET(CMAKE_CXX_FLAGS    " ${CMAKE_CXX_FLAGS} -g -pg ")
    SET(CMAKE_LINK_FLAGS   " ${CMAKE_LINK_FLAGS} -O2 -g -pg ")
endif()

################################################################################
# Targets for Executables / Test
################################################################################

#-------------------------------------------------------------------------------
# Targets for intermeritate Archives / Libraries
#-------------------------------------------------------------------------------
# Directory of project libraries (static & dynamic)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})

# Create a Library with all object files of project except main
add_library( ${LIBRARY_NAME} SHARED ${LIB_SRCS})
SET_TARGET_PROPERTIES(${LIBRARY_NAME}  PROPERTIES
    VERSION ${VERSION})
#SET_TARGET_PROPERTIES(${LIBRARY_NAME}  PROPERTIES
#    LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})
#SET_TARGET_PROPERTIES(${LIBRARY_NAME}  PROPERTIES
#    ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


#-------------------------------------------------------------------------------
# Unit Test using gtest
#-------------------------------------------------------------------------------
if(BUILD_TEST)
    ExternalProject_Get_Property(googletest source_dir)
    include_directories(${source_dir}/include)

    ExternalProject_Get_Property(googletest binary_dir)
    set(UNIT_TEST_LIB ${binary_dir}/libgtest.a)

    #Could also be your own main.cpp
    set(RUN_TEST_MAIN ${binary_dir}/libgtest_main.a)

    # Note: we do not add the project as binary library (libtransmogrify.a)
    #       to build with debug / coverage flags
    add_executable(${TEST_BINARY_NAME} ${LIB_SRCS} ${TEST_HARNESS_SRC} )
    add_dependencies(${TEST_BINARY_NAME} googletest)

    SET_TARGET_PROPERTIES(${TEST_BINARY_NAME}  PROPERTIES COMPILE_DEFINITIONS "TESTING" )
    SET_TARGET_PROPERTIES(${TEST_BINARY_NAME}  PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

    # Add Test-Framework and main() to execute test
    TARGET_LINK_LIBRARIES(${TEST_BINARY_NAME} ${RUN_TEST_MAIN} ${UNIT_TEST_LIB} ${THREAD_LIB})

    if(TEST_COVERAGE)
        TARGET_LINK_LIBRARIES(${TEST_BINARY_NAME}  ${COVERAGE_LIB})
    endif()
      
    # enable Cmake's make test  
    ENABLE_TESTING()
    # Call the testBinary with junit-xml output
    add_test(junitout "./${TEST_BIN_DIR}/${TEST_BINARY_NAME}"
	--gtest_output=xml:gtestresults.xml )
endif(BUILD_TEST)

#-------------------------------------------------------------------------------
# Optimized release build
#-------------------------------------------------------------------------------
add_executable(${BINARY_NAME} ${DEMO_SRCS})
SET_TARGET_PROPERTIES(${BINARY_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
TARGET_LINK_LIBRARIES(${BINARY_NAME} ${LIBRARY_NAME} ) # Final Linkage without any helper libs


################################################################################
# Install
################################################################################
#TODO: need something better here *.(h, hpp, h++)
# INSTALL(DIRECTORY ${PROJECT_INCLUDE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.hpp")
INSTALL(DIRECTORY
   ${PROJECT_INCLUDE_DIR}/${TARGET_SUBDIR_NAME}/
   DESTINATION include/${TARGET_SUBDIR_NAME})

# library
INSTALL(TARGETS ${LIBRARY_NAME} LIBRARY DESTINATION  lib)

#Demo binary
if(BUILD_DEMO)
    INSTALL(TARGETS ${BINARY_NAME} RUNTIME DESTINATION  bin )
endif()
