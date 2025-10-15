cmake_minimum_required(VERSION 3.10)
project(kdexgctl C)

set(USER_BUILD_DIR ${CMAKE_BINARY_DIR}/user)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${USER_BUILD_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${USER_BUILD_DIR})
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${USER_BUILD_DIR})
set(CMAKE_PDB_OUTPUT_DIRECTORY ${USER_BUILD_DIR})
set(CMAKE_OBJECT_PATH_MAX 4096)

# --- Local source files ---
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

# --- Shared source files ---
file(GLOB_RECURSE SHARED_SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/*.c
)

# --- Include directories ---
file(GLOB SRC_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
foreach(subdir ${SRC_DIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/${subdir})
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/${subdir})
    endif()
endforeach()

# Include all shared subdirectories
file(GLOB SHARED_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/../shared ${CMAKE_CURRENT_SOURCE_DIR}/../shared/*)
foreach(subdir ${SHARED_DIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../shared/${subdir})
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../shared/${subdir})
    endif()
endforeach()

# --- Common include directories ---
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../includes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../includes/cJSON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../shared)

# --- Executable ---
add_executable(kdexgctl
    kdexgctl.c
    ${SRC_FILES}
    ${SHARED_SRC_FILES}
    ../includes/cJSON/cJSON.c
)

# --- Compiler options ---
target_compile_features(kdexgctl PRIVATE c_std_99)
target_compile_options(kdexgctl PRIVATE -Wall -Wextra)

# --- Output directories ---
set_target_properties(kdexgctl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${USER_BUILD_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${USER_BUILD_DIR}
    COMPILE_PDB_OUTPUT_DIRECTORY ${USER_BUILD_DIR}
    PDB_OUTPUT_DIRECTORY ${USER_BUILD_DIR}
)
