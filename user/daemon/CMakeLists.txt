cmake_minimum_required(VERSION 3.10)
project(kdexg_daemon C)

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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../includes/uthash/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../shared)

# --- Executable ---
add_executable(kdexg_daemon
    kdexg_daemon.c
    ${SRC_FILES}
    ${SHARED_SRC_FILES}
    ../includes/cJSON/cJSON.c
)

# --- Compiler options ---
target_compile_features(kdexg_daemon PRIVATE c_std_99)
target_compile_options(kdexg_daemon PRIVATE -Wall -Wextra)

# --- Find and link libnl (Netlink) ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBNL REQUIRED libnl-3.0 libnl-genl-3.0)

include_directories(${LIBNL_INCLUDE_DIRS})
link_directories(${LIBNL_LIBRARY_DIRS})

target_link_libraries(kdexg_daemon ${LIBNL_LIBRARIES})

# --- Output directories ---
set_target_properties(kdexg_daemon PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${USER_BUILD_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${USER_BUILD_DIR}
    COMPILE_PDB_OUTPUT_DIRECTORY ${USER_BUILD_DIR}
    PDB_OUTPUT_DIRECTORY ${USER_BUILD_DIR}
)
