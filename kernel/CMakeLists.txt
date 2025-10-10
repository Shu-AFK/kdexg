cmake_minimum_required(VERSION 3.10)
project(kDEXG C)

set(MODULE_NAME kdexg)
set(KERNEL_DIR ${CMAKE_SOURCE_DIR}/build-5.15/ubuntu-headers/usr/src/linux-headers-5.15.0-157-generic)
set(KERNEL_BUILD_DIR ${CMAKE_BINARY_DIR}/kernel)
file(MAKE_DIRECTORY ${KERNEL_BUILD_DIR})

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/${MODULE_NAME}.ko
    COMMAND ${CMAKE_COMMAND} -E echo "Building kernel module with headers from ${KERNEL_DIR}"
    COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/* ${KERNEL_BUILD_DIR}/
    COMMAND ${CMAKE_MAKE_PROGRAM} -C ${KERNEL_DIR} M=${KERNEL_BUILD_DIR} modules
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${KERNEL_BUILD_DIR}/${MODULE_NAME}.ko
        ${CMAKE_BINARY_DIR}/${MODULE_NAME}.ko
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(${MODULE_NAME}_module ALL DEPENDS ${CMAKE_BINARY_DIR}/${MODULE_NAME}.ko)
