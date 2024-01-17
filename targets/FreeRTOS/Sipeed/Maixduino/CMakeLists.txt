#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

include(binutils.common)
include(binutils.FreeRTOS)

include(FreeRTOS_${TARGET_SERIES}_GCC_options)

nf_setup_target_build(
    # HAS_NANOBOOTER

    # BOOTER_LINKER_FILE 
    #    kendryte-k210

    CLR_LINKER_FILE 
        kendryte-k210

    # BOOTER_EXTRA_COMPILE_DEFINITIONS
    #    -DCPU_K210
    
    CLR_EXTRA_COMPILE_DEFINITIONS
        -DCPU_K210

    # BOOTER_EXTRA_LINKMAP_PROPERTIES
    #    ",--library-path=${CMAKE_SOURCE_DIR}/targets/Sipeed/common,--defsym=__crt_heap_size__=0x0"

    CLR_EXTRA_LINKMAP_PROPERTIES
        ",--library-path=${CMAKE_SOURCE_DIR}/targets/Sipeed/common,--defsym=__crt_heap_size__=0x0"

    # BOOTER_EXTRA_LIBRARIES
    #    "m;freertos;atomic;bsp;c;stdc++;drivers;posix;hal;"
    CLR_EXTRA_LIBRARIES
        "-Wl,--start-group"
        "m;freertos;atomic;bsp;c;stdc++;drivers;posix;hal;"
        "-Wl,--end-group;"
)

# TODO Enable
# target_compile_definitions(NF_Network PUBLIC -DCPU_K210)
target_compile_definitions(NF_NativeAssemblies PUBLIC -DCPU_K210)
