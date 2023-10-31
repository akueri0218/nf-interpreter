//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include <nanoHAL_v2.h>
//#include "MIMXRT1062.h"
#include <sysctl.h>

inline void CPU_Reset()
{
    sysctl_reset(SYSCTL_RESET_SOC);
};

// CPU sleep is not currently implemented in this target
inline void CPU_Sleep(SLEEP_LEVEL_type level, uint64_t wakeEvents)
{
    (void)level;
    (void)wakeEvents;
};

inline bool CPU_IsSoftRebootSupported()
{
    return true;
};

void CPU_SetPowerMode(PowerLevel_type powerLevel)
{
    //TODO power modes support
    (void)powerLevel;
}
