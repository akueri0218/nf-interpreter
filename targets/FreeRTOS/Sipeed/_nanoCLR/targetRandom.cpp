//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "Core.h"

void CLR_RT_Random::Initialize()
{
    // TODO implement
}

void CLR_RT_Random::Initialize( int seed )
{
    (void)seed;
    // TODO implement
}

uint32_t CLR_RT_Random::Next()
{
    uint32_t data = 0;
    // TODO implement
    return data;
}

double CLR_RT_Random::NextDouble()
{
    uint32_t data = 0;
    // TODO implement
    return ((double)data) / ((double)0xFFFFFFFF);
}

void CLR_RT_Random::NextBytes(unsigned char* buffer, unsigned int count)
{
    // TODO implement 
}
