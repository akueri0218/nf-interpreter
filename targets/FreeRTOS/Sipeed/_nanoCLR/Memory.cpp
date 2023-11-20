//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include <nanoHAL.h>

extern int _heap_start;
extern int _heap_end;

void HeapLocation(unsigned char*& baseAddress, unsigned int& sizeInBytes)
{
    NATIVE_PROFILE_PAL_HEAP();

    baseAddress = (unsigned char*)0x80400000;  // start of heap
    sizeInBytes =   (unsigned int)0x00200000;  // 2MB  
}
