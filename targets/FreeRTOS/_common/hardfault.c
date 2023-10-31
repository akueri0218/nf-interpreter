//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#if defined(CPU_MIMXRT1062CVL5A)
#include "MIMXRT1062.h"
#elif defined(CPU_K210)
#include <sysctl.h>
#endif

__attribute__((used)) void prvGetRegistersFromStack(unsigned int *pulFaultStackAddress)
{
    /* These are volatile to try and prevent the compiler/linker optimising them
    away as the variables never actually get used.  If the debugger won't show the
    values of the variables, make them global my moving their declaration outside
    of this function. */
    volatile unsigned int r0;
    volatile unsigned int r1;
    volatile unsigned int r2;
    volatile unsigned int r3;
    volatile unsigned int r12;
    volatile unsigned int lr;  /* Link register. */
    volatile unsigned int pc;  /* Program counter. */
    volatile unsigned int psr; /* Program status register. */

    r0 = pulFaultStackAddress[0];
    r1 = pulFaultStackAddress[1];
    r2 = pulFaultStackAddress[2];
    r3 = pulFaultStackAddress[3];

    r12 = pulFaultStackAddress[4];
    lr = pulFaultStackAddress[5];
    pc = pulFaultStackAddress[6];
    psr = pulFaultStackAddress[7];

    (void)r0;
    (void)r1;
    (void)r2;
    (void)r3;
    (void)r12;
    (void)lr;
    (void)pc;
    (void)psr;

    // forces a breakpoint causing the debugger to stop
    // if no debugger is attached this is ignored
#if defined(__arm__)
    __asm volatile("BKPT #0\n");

    // If no debugger connected, just reset the board
    NVIC_SystemReset();
#elif defined(__riscv64)
    __asm volatile("ebreak\n");

    sysctl_reset(SYSCTL_RESET_SOC);
#endif

    for (;;)
        ;
}

__attribute__((naked, aligned(4))) void HardFault_Handler(void)
{
#if defined(__arm__)
    __asm volatile(
                   " tst lr, #4                                                \n"
                   " ite eq                                                    \n"
                   " mrseq r0, msp                                             \n"
                   " mrsne r0, psp                                             \n"
                   " ldr r1, [r0, #24]                                         \n"
                   " ldr r2, handler2_address_const                            \n"
                   " bx r2                                                     \n"
                   " handler2_address_const: .word prvGetRegistersFromStack    \n"                  ""
    );
#elif defined(__riscv64)
    __asm volatile(
        "" // # TODO: 実装
    );
#endif
}
