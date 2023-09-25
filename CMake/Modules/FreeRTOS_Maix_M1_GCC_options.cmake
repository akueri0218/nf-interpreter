#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

#################################################################
# WHEN ADDING A NEW SERIES add the appropriate GCC options bellow
#################################################################

# need to specify this for assembler
set(CMAKE_ASM_FLAGS "-mcmodel=medany -mabi=lp64f -march=rv64imafc -x assembler-with-cpp" CACHE INTERNAL "asm compiler flags")

# need to specify linker flags here
set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -static -Wl,--gc-sections -Wl,-static -Wl,--start-group -Wl,--whole-archive -Wl,--no-whole-archive -Wl,--end-group" CACHE INTERNAL "executable linker flags")

# TARGET parameter to set the target that's setting them for
# optional EXTRA_COMPILE_OPTIONS with compile options to be added
macro(nf_set_compile_options)

    # parse arguments
    cmake_parse_arguments(NFSCO "" "TARGET" "EXTRA_COMPILE_OPTIONS" ${ARGN})
    
    if(NOT NFSCO_TARGET OR "${NFSCO_TARGET}" STREQUAL "")
        message(FATAL_ERROR "Need to set TARGET argument when calling nf_set_compile_options()")
    endif()

    # include any extra options comming from any extra args?
    # TODO: removed -Wundef until fix with FatFS is merged (https://github.com/abbrev/fatfs/pull/8)
    # add -fpermissive -Wno-old-style-declaration -Wno-class-memaccess
    target_compile_options(${NFSCO_TARGET} PUBLIC  ${NFSCO__EXTRA_COMPILE_OPTIONS})

    # define riscv64
    target_compile_definitions(${NFSCO_TARGET} PUBLIC -DPLATFORM_RISCV -D__riscv64 -DCONFIG_LOG_LEVEL=LOG_INFO -DCONFIG_LOG_ENABLE -DCONFIG_LOG_COLORS -DLOG_KERNEL) 

endmacro()


# TARGET parameter to set the target that's setting them for
# optional EXTRA_LINK_FLAGS with link flags to be added
macro(nf_set_link_options)

    # parse arguments
    cmake_parse_arguments(NFSLO "" "TARGET;EXTRA_LINK_FLAGS" "" ${ARGN})
    
    if(NOT NFSLO_TARGET OR "${NFSLO_TARGET}" STREQUAL "")
        message(FATAL_ERROR "Need to set TARGET argument when calling nf_set_link_options()")
    endif()

    # request specs from newlib nano
    # TODO fix: cannnot find -lg_nano -lc_nano
    # remove "--specs=nano.specs --specs=nosys.specs"
    # these libraries are NOT contained to k210 toolchain
    set_property(TARGET ${NFSLO_TARGET} APPEND_STRING PROPERTY LINK_FLAGS " -Wl,--start-group -Xlinker --gc-sections -Xlinker --sort-section=alignment -Xlinker -print-memory-usage")

    # set optimization linker flags for RELEASE and MinSizeRel
    if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        set_property(TARGET ${NFSLO_TARGET} APPEND_STRING PROPERTY LINK_FLAGS " -flto -fuse-linker-plugin -Os ")
    endif()
      
    # include libraries in build
    nf_include_libraries_in_build(${NFSLO_TARGET})

    # set extra linker flags
    set_property(TARGET ${NFSLO_TARGET} APPEND_STRING PROPERTY LINK_FLAGS " ${NFSLO_EXTRA_LINK_FLAGS} ")
      
    # set optimization flags
    nf_set_optimization_options(${NFSLO_TARGET})

endmacro()
