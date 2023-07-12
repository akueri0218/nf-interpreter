//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#ifndef TARGET_IMXRTFLASH_DRIVER_H
#define TARGET_IMXRTFLASH_DRIVER_H

#include <nanoPAL_BlockStorage.h>

#ifdef __cplusplus
extern "C" {
#endif

bool w25qxxSPIDriver_InitializeDevice(void*);
bool w25qxxSPIDriver_UninitializeDevice(void*);
DeviceBlockInfo* w25qxxSPIDriver_GetDeviceInfo(void*);
bool w25qxxSPIDriver_Read(void*, ByteAddress startAddress, unsigned int numBytes, unsigned char* buffer);
bool w25qxxSPIDriver_Write(void*, ByteAddress startAddress, unsigned int numBytes, unsigned char* buffer, bool readModifyWrite);
bool w25qxxSPIDriver_IsBlockErased(void*, ByteAddress blockAddress, unsigned int length);
bool w25qxxSPIDriver_EraseBlock(void*, ByteAddress address);
void w25qxxSPIDriver_SetPowerState(void*, unsigned int state);

#ifdef __cplusplus
}
#endif

#endif // TARGET_IMXRTFLASH_DRIVER_H
