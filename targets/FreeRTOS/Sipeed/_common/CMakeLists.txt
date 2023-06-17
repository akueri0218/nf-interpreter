#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/bsp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/drivers)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/freertos)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/hal)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/posix)


list(APPEND TARGET_MAIXDUINO_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/WireProtocol_HAL_Interface.c)

# append Target files
list(APPEND TARGET_MAIXDUINO_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Device_BlockStorage.c)
list(APPEND TARGET_MAIXDUINO_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Target_BlockStorage_iMXRTFlashDriver.c)
list(APPEND TARGET_MAIXDUINO_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/platform_BlockStorage.c)

# include configuration manager file, if feature is enabled
if(NF_FEATURE_HAS_CONFIG_BLOCK)
    list(APPEND TARGET_MAIXDUINO_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/targetHAL_ConfigurationManager.cpp)
endif()

# make var global
set(TARGET_MAIXDUINO_COMMON_SOURCES ${TARGET_MAIXDUINO_COMMON_SOURCES} CACHE INTERNAL "make global")
set(TARGET_MAIXDUINO_COMMON_INCLUDE_DIRS ${TARGET_MAIXDUINO_COMMON_INCLUDE_DIRS} CACHE INTERNAL "make global")
