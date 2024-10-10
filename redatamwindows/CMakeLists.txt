cmake_minimum_required(VERSION 3.10)
project(redatam)

set(CMAKE_CXX_STANDARD 11)

# Paths
set(SRC_PATH "../src")
set(INCLUDE_PATH "../include")
set(PUGIXML_PATH "../vendor/pugixml")

# Debug: Print the paths being used
message(STATUS "Source path: ${SRC_PATH}")
message(STATUS "Include path: ${INCLUDE_PATH}")
message(STATUS "PugiXML path: ${PUGIXML_PATH}")

# Verify that the paths exist
foreach(path ${SRC_PATH} ${INCLUDE_PATH} ${PUGIXML_PATH})
    if(NOT EXISTS ${path})
        message(FATAL_ERROR "Path does not exist: ${path}")
    endif()
endforeach()

# Include directories
include_directories(${INCLUDE_PATH})
include_directories(${INCLUDE_PATH}/entities)
include_directories(${INCLUDE_PATH}/readers)
include_directories(${INCLUDE_PATH}/exporters)
include_directories(${PUGIXML_PATH})

# Debug: Print the include directories
message(STATUS "Include directories: ${INCLUDE_PATH}, ${INCLUDE_PATH}/entities, ${INCLUDE_PATH}/readers, ${INCLUDE_PATH}/exporters, ${PUGIXML_PATH}")

# Source files
file(GLOB_RECURSE REDATAM_SRCS "${SRC_PATH}/*.cpp")
file(GLOB_RECURSE PUGIXML_SRCS "${PUGIXML_PATH}/*.cpp")

# Debug: Print the source files found
message(STATUS "Source files for redatam: ${REDATAM_SRCS}")
message(STATUS "Source files for pugixml: ${PUGIXML_SRCS}")

# Check if any source files were found
if(NOT REDATAM_SRCS)
    message(FATAL_ERROR "No source files found for redatam")
endif()

# Executable for redatam
add_executable(redatam ${REDATAM_SRCS} ${PUGIXML_SRCS})
