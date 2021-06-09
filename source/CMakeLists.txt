# ------------------------------------------------------------------------------------ #
# Check if cmake has the required version
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

# ------------------------------------------------------------------------------------ #
# Declaring the project
project(tools)
set(REPOSITORY_DIR ${PROJECT_SOURCE_DIR}/../)

# ------------------------------------------------------------------------------------ #
# Set version from the git tag
find_package(Git)
if(NOT GIT_FOUND)
    message(FATAL_ERROR "Git not found")
endif()
execute_process(
    COMMAND "${GIT_EXECUTABLE}" describe --tags
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    RESULT_VARIABLE git_result
    OUTPUT_VARIABLE git_tag
    ERROR_VARIABLE git_error
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE
)
if(NOT git_result EQUAL 0)
    message(FATAL_ERROR "Failed to execute Git: ${git_error}")
endif()
if(git_tag MATCHES
   "^v([1-9][0-9]*)[.]([0-9]+)([.]([0-9]+))?-?([0-9]+)?(-g([0-9a-f-]+))?$"
)
    set(TOOLS_MAJOR_VERSION "${CMAKE_MATCH_1}")
    set(TOOLS_MINOR_VERSION "${CMAKE_MATCH_2}")
    set(TOOLS_PATCH_VERSION "${CMAKE_MATCH_4}")
    set(NB_COMMIT "${CMAKE_MATCH_5}")
    set(GIT_HASH "${CMAKE_MATCH_7}")
else()
    message(FATAL_ERROR "Git tag isn't valid semantic version: [${git_tag}]")
endif()

if("${TOOLS_PATCH_VERSION}" STREQUAL "")
    set(TOOLS_PATCH_VERSION 0)
endif()
set(TOOLS_VERSION ${TOOLS_MAJOR_VERSION}.${TOOLS_MINOR_VERSION}.${TOOLS_PATCH_VERSION})
if(NOT "${GIT_HASH}" EQUAL "")
    message(STATUS "Tools base version: ${TOOLS_VERSION}")
    message(STATUS "Commit: ${GIT_HASH}, ${NB_COMMIT} commits above latest version")
else()
    message(STATUS "Tools version: ${TOOLS_VERSION}")
endif()

# ------------------------------------------------------------------------------------ #
# Set compilation version
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(DEBUG 1)
    add_definitions(-DDEBUG=${DEBUG})
    message(STATUS "Debug build type selected, level ${DEBUG}")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RELEASE")
    message(STATUS "No build type selected, default to Release")
endif()

# ------------------------------------------------------------------------------------ #
# Offer the user the choice of overriding the installation directories
set(CMAKE_INSTALL_PREFIX
    ${CMAKE_BINARY_DIR}/../../install/
    CACHE PATH "Prefix for installation" FORCE
)
set(INSTALL_LIB_DIR
    lib
    CACHE PATH "Installation directory for libraries" FORCE
)
set(INSTALL_INCLUDE_DIR
    include
    CACHE PATH "Installation directory for header files" FORCE
)
set(INSTALL_CMAKE_DIR
    CMake
    CACHE PATH "Installation directory for CMake files" FORCE
)

# ------------------------------------------------------------------------------------ #
# Defining default install dir
set(CMAKE_INSTALL_LIBDIR lib/) # Search for GNUInstallDirs in CMake documentation
set(CMAKE_INSTALL_BINDIR bin/) # Search for GNUInstallDirs in CMake documentation

# ------------------------------------------------------------------------------------ #
# Defining global default install dirs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/arch)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)

# ------------------------------------------------------------------------------------ #
# Make relative paths absolute(needed later on)
foreach(p LIB BIN CMAKE INCLUDE CMAKE)
    set(var INSTALL_${p}_DIR)
    if(NOT IS_ABSOLUTE "${${var}}")
        set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
    endif()
endforeach()

# ------------------------------------------------------------------------------------ #
# Do we have ROOT(to link the data base with ROOT) ?
option(USE_ROOT "Use ROOT to build the data base" OFF)

# Add the ROOT libraries ?
if(USE_ROOT)
    # Look for ROOT, at least 6.00, and compiled with C++ 11
    list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
    find_package(ROOT 6.00 REQUIRED)
    include(${ROOT_USE_FILE})

    # Now check for C++ 17
    if(NOT ROOT_CXX_FLAGS MATCHES "-std=c\\+\\+17")
        message(
            FATAL_ERROR "Your ROOT version is good, but was not compiled with C++17"
        )
    endif()

    # Setting the compiler flags for Root
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

    # Add the ROOT libraries to the potential EXTRA libraries
    set(EXTRA_LIBS ${EXTRA_LIBS} ${ROOT_LIBRARIES})

    # Add the path to the ROOT headers
    include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})
endif(USE_ROOT)

# ------------------------------------------------------------------------------------ #
# Set compiler option Set C++ 17 if not already set
if(NOT CMAKE_CXX_FLAGS MATCHES "-std=c\\+\\+")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()
# Set warnings
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-long-long -Wno-vla"
)

# ------------------------------------------------------------------------------------ #
# Set the headers directory
include_directories(${PROJECT_SOURCE_DIR}/inc ${PROJECT_BINARY_DIR})

# ------------------------------------------------------------------------------------ #
# Configure a header file to pass version to the source code
configure_file(
    "${PROJECT_SOURCE_DIR}/ToolsConfig.hh.in" "${PROJECT_BINARY_DIR}/ToolsConfig.hh"
)

# ------------------------------------------------------------------------------------ #
# Target library Tools

# Generating file lists
file(GLOB TOOLS_SRC ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB TOOLS_INC ${PROJECT_SOURCE_DIR}/inc/*.hh)

# Generating the library
add_library(Tools SHARED ${TOOLS_SRC} ${TOOLS_INC} ToolsConfig.hh.in)
set_target_properties(
    Tools PROPERTIES PUBLIC_HEADER "${TOOLS_INC};${INSTALL_INCLUDE_DIR}/ToolsConfig.hh"
)
target_link_libraries(Tools ${EXTRA_LIBS})

# Install the library and the configuration file
install(FILES "${PROJECT_BINARY_DIR}/ToolsConfig.hh" DESTINATION include)
install(
    TARGETS Tools
    EXPORT ToolsTargets
    LIBRARY DESTINATION ${INSTALL_LIB_DIR} COMPONENT lib
    PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR} COMPONENT dev
)
set(TOOLS_LIB ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libTools.so)

# ------------------------------------------------------------------------------------ #
# Create the package: add all targets to the build-tree export set
export(TARGETS Tools FILE "${PROJECT_BINARY_DIR}/ToolsTargets.cmake")

# Export the package for use from the build-tree (this registers the build-tree with a
# global CMake-registry)
export(PACKAGE Tools)

# Define the paths for ToolsConfig.cmake Define the path to headers
file(RELATIVE_PATH REL_INCLUDE_DIR "${PROJECT_BINARY_DIR}" "${INSTALL_INCLUDE_DIR}")
set(CONF_INCLUDE_DIR "\${TOOLS_CMAKE_DIR}/${REL_INCLUDE_DIR}")
# Define the path to libraries
file(RELATIVE_PATH REL_LIBRARY_DIR "${PROJECT_BINARY_DIR}" "${INSTALL_LIB_DIR}")
set(CONF_LIBRARY_DIR "\${TOOLS_CMAKE_DIR}/${REL_LIBRARY_DIR}")

# Create ToolsConfig.cmake
configure_file(ToolsConfig.cmake.in "${PROJECT_BINARY_DIR}/ToolsConfig.cmake" @ONLY)
# Create ToolsConfigVersion.cmake
configure_file(
    ToolsConfigVersion.cmake.in
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ToolsConfigVersion.cmake" @ONLY
)

# Install the ToolsConfig.cmake and ToolsConfigVersion.cmake
install(
    FILES "${PROJECT_BINARY_DIR}/ToolsConfig.cmake"
          "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ToolsConfigVersion.cmake"
    DESTINATION "${INSTALL_CMAKE_DIR}"
    COMPONENT dev
)

# Install the export set for use with the install-tree
install(
    EXPORT ToolsTargets
    DESTINATION "${INSTALL_CMAKE_DIR}"
    COMPONENT dev
)

# ------------------------------------------------------------------------------------ #
# Get the Cppyy and clang package
execute_process(
    COMMAND bash -c "poetry show cppyy-backend | cut -d: -f2 | sed -n 2p"
    OUTPUT_VARIABLE CPPYY_BACKEND_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "CPPYY_BACKEND_VERSION: ${CPPYY_BACKEND_VERSION}")

if(${CPPYY_BACKEND_VERSION} LESS_EQUAL 1.14.5)
    # Until version 1.14.5, use corrected cmake scripts to find Cppyy and libClang
    list(APPEND CMAKE_MODULE_PATH ${REPOSITORY_DIR}/cmake)
endif()
execute_process(
    COMMAND cling-config --cmake
    OUTPUT_VARIABLE CPPYY_MODULE_PATH
    ERROR_VARIABLE CPPYY_MODULE_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE
)
if (NOT CPPYY_MODULE_ERROR EQUAL 0)
    message(STATUS "Cppyy has not been installed, skipping the Python binding")
    return()
endif()
list(APPEND CMAKE_MODULE_PATH ${CPPYY_MODULE_PATH})
find_package(Cppyy REQUIRED)

# ------------------------------------------------------------------------------------ #
# Target Cppyy binding
message(STATUS "INSTALL_INCLUDE_DIR: ${INSTALL_INCLUDE_DIR}")
# Generating the library
cppyy_add_bindings(
    "PyTools"
    "${TOOLS_VERSION}"
    "jgaffiot"
    "j.gaffiot@laposte.net"
    LICENSE
    "LGPL-3.0-or-later"
    LICENSE_FILE
    ${REPOSITORY_DIR}/LICENSE
    README_FILE
    ${REPOSITORY_DIR}/README.md
    LANGUAGE_STANDARD
    "17"
    NAMESPACES
    "tools"
    LINKDEFS
    ${PROJECT_SOURCE_DIR}/Linkdef.h
    INCLUDE_DIRS
    ${PROJECT_BINARY_DIR}/
    LINK_LIBRARIES
    ${TOOLS_LIB}
    H_DIRS
    ${PROJECT_SOURCE_DIR}/inc/
    H_FILES
    ${TOOLS_INC}
)
add_dependencies(PyToolsCppyy Tools)
install(CODE "message(STATUS \"Attempting pip install\" ${PY_WHEEL_FILE})"
        CODE "execute_process(COMMAND poetry run pip install --force-reinstall
        ${PY_WHEEL_FILE})"
)
