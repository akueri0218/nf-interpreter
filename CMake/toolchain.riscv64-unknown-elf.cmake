#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

include(CMakeForceCompiler)

# the name of the operating system for which CMake is to build
set(CMAKE_SYSTEM_NAME Generic)

# name of the CPU CMake is building for
set(CMAKE_SYSTEM_PROCESSOR riscv64-unknown)

# macro to setup compilers
macro(nf_set_compiler_var var name)
   find_program(
      CMAKE_${var}
      riscv64-unknown-elf-${name}
      CMAKE_FIND_ROOT_PATH_BOTH
      REQUIRED)
endmacro()

# safer to have these here as a check if the toolchain are accessible in the PATH

# setup C compiler
nf_set_compiler_var(C_COMPILER gcc)

# setup C++ compiler
nf_set_compiler_var(CXX_COMPILER g++)

# setup Assembler compiler
nf_set_compiler_var(ASM_COMPILER gcc)
 
# other toolchain configurations  
find_program(
   CMAKE_OBJCOPY
   riscv64-unknown-elf-objcopy
   CMAKE_FIND_ROOT_PATH_BOTH)

find_program(
   CMAKE_OBJDUMP
   riscv64-unknown-elf-objdump
   CMAKE_FIND_ROOT_PATH_BOTH)

find_program(
   CMAKE_SIZE
   riscv64-unknown-elf-size
   CMAKE_FIND_ROOT_PATH_BOTH)

set(COMMON_FLAGS "-mcmodel=medany -mabi=lp64f -march=rv64imafc -fno-common -ffunction-sections -fdata-sections -fstrict-volatile-bitfields -ffast-math -fno-math-errno -fsingle-precision-constant -O2 -ggdb -Wall -Werror=all -Wno-error=comment -Wno-error=unused-function -Wno-error=unused-but-set-variable -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wno-error=maybe-uninitialized -Wextra -Werror=frame-larger-than=65536 -Wno-unused-parameter -Wno-unused-function -Wno-implicit-fallthrough -Wno-sign-compare -Wno-error=missing-braces")

set(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu11 -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-old-style-declaration" CACHE STRING "C Compiler Base Flags")

set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=gnu++17 -fpermissive -Wno-class-memaccess -fno-use-cxa-atexit" CACHE STRING "C++ Compiler Base Flags")


# root paths to search on the filesystem for cross-compiling
get_filename_component(CMAKE_FIND_ROOT_PATH ${CMAKE_C_COMPILER} DIRECTORY CACHE)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# set required C and C++ standard for ALL targets
# set(CMAKE_C_STANDARD 11 CACHE INTERNAL "C standard for all targets")
# set(CMAKE_CXX_STANDARD 17 CACHE INTERNAL "C++ standard for all targets")

# Perform compiler test with static library
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
