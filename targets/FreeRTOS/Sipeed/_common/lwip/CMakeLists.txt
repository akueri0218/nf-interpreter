cmake_minimum_required(VERSION 3.7)

project(lwIP)

set(LWIP_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${LWIP_DIR}/src/include ${MAIXDUINO_FREERTOS_DIR}/include ${MAIXDUINO_FREERTOS_DIR}/conf  ${MAIXDUINO_FREERTOS_DIR}/portable)
include(src/Filelists.cmake)