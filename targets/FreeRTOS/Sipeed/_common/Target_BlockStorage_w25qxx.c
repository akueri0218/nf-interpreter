//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

// #include "MIMXRT1062.h"
// #include "flexspi_nor_flash_ops.h"
// #include "fsl_cache.h"
// #include "fsl_flexspi.h"

#include "w25qxx.h"
#include <devices.h>

#include <nanoPAL_BlockStorage.h>
#include <targetHAL.h>
#include <Target_BlockStorage_w25qxx.h>

#include "FreeRTOS.h"

extern handle_t spi3;

bool w25qxx_InitializeDevice(void *context)
{
    (void)context;

    w25qxx_init(spi3);

    return true;
}

bool w25qxx_UninitializeDevice(void *context)
{
    (void)context;

    return true;
}

DeviceBlockInfo *w25qxx_GetDeviceInfo(void *context)
{
    MEMORY_MAPPED_NOR_BLOCK_CONFIG *config = context;

    return config->BlockConfig.BlockDeviceInformation;
}

bool w25qxx_Read(void *context, ByteAddress startAddress, unsigned int numBytes, unsigned char *buffer)
{
    (void)context;

    uint32_t addr = startAddress - SPI3_BASE_ADDR;
    uint8_t *ptr = buffer;
    uint32_t length = 2040;

    while(numBytes)
    {
        if(numBytes < 2040)
            length = numBytes;

        w25qxx_read_data(addr, (uint8_t*)ptr, length);

        addr += length;
        ptr += length;
        numBytes -= length;
    }

    return true;
}

bool w25qxx_Write(
    void *context,
    ByteAddress startAddress,
    unsigned int numBytes,
    unsigned char *buffer,
    bool readModifyWrite)
{
    (void)context;

    // Read-modify-write is used for FAT filesystems only
    if (readModifyWrite)
    {
        return false;
    }
    portENTER_CRITICAL();

    w25qxx_write_data(startAddress - SPI3_BASE_ADDR, (uint8_t*)buffer, numBytes);

    portEXIT_CRITICAL();
    return true;
}

#define FLASH_ERASED_WORD 0xFFFFFFFF

bool w25qxx_IsBlockErased(void *context, ByteAddress blockAddress, unsigned int length)
{
    // TODO: implement

    (void)context;

    w25qxx_enable_xip();

    uint32_t *cursor = (uint32_t *)blockAddress;
    uint32_t *endAddress = (uint32_t *)(blockAddress + length);
    // an erased flash address has to read FLASH_ERASED_WORD
    // OK to check by word (32 bits) because the erase is performed by 'page'
    // whose size is word multiple
    while (cursor < endAddress)
    {
        if (*cursor++ != FLASH_ERASED_WORD)
        {
            w25qxx_reset_xip();

            // found an address with something other than FLASH_ERASED_WORD!!
            return false;
        }
    }

    w25qxx_reset_xip();

    // reached here so the segment must be erased
    return true;
}

bool w25qxx_EraseBlock(void *context, ByteAddress address)
{
    (void)context;
    portENTER_CRITICAL();

    enum w25qxx_status_t status = w25qxx_sector_erase(address - SPI3_BASE_ADDR);

    portEXIT_CRITICAL();
    if (status != W25QXX_OK)
    {
        return false;
    }

    return true;
}
