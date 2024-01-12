//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include <nanoHAL.h>

extern int __ClrHeapTop;
extern int __ClrHeapSize;

void HeapLocation(unsigned char*& baseAddress, unsigned int& sizeInBytes)
{
    NATIVE_PROFILE_PAL_HEAP();

    baseAddress = (unsigned char*)&__ClrHeapTop;  // start of heap
    sizeInBytes =   (unsigned int)&__ClrHeapSize;  // 2MB  
}
