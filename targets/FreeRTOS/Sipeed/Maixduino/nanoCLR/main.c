//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

// #include "board.h"
// #include "peripherals.h"
// #include "pin_mux.h"
// #include "clock_config.h"
// #include "MIMXRT1062.h"
// #include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"

#include <nanoHAL_v2.h>

#include <WireProtocol_ReceiverThread.h>
#include <nanoCLR_Application.h>
#include "Target_BlockStorage_w25qxx.h"
#include "CLR_Startup_Thread.h"

#include "fpioa_config.h"
#include <devices.h>

// debug
// #include <stdio.h>

// configure heap memory
extern uint8_t ucHeap[configTOTAL_HEAP_SIZE];

handle_t gpiohs;
handle_t gpio;

handle_t spi0;
handle_t spi3;

extern void CLRStartupThread(void *argument);

int main(void)
{
    // hardware initialized in _init_bsp()
    
    gpiohs = io_open("/dev/gpio0");
    gpio  = io_open("/dev/gpio1");
    
    spi0 = io_open("/dev/spi0");
    spi3 = io_open("/dev/spi3");
    
    w25qxx_InitializeDevice(NULL);

    uarths_init_extra();

    xTaskCreate(ReceiverThread, "ReceiverThread", 2048, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(CLRStartupThread, "CLRStartupThread", 8192, NULL, configMAX_PRIORITIES - 2, NULL);

    vTaskDelete(NULL); 
}