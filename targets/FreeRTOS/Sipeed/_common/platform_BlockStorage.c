//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include <nanoPAL_BlockStorage.h>
#include <Target_BlockStorage_w25qxx.h>

// map here the Block Storage Interface to the iMXRT FlexSPI driver
IBlockStorageDevice w25qxx_BlockStorageInterface =
{                          
    &w25qxx_InitializeDevice,
    &w25qxx_UninitializeDevice,
    &w25qxx_GetDeviceInfo,
    &w25qxx_Read,
    &w25qxx_Write,
    NULL,
    &w25qxx_IsBlockErased,
    &w25qxx_EraseBlock,
    NULL,
    NULL
};
