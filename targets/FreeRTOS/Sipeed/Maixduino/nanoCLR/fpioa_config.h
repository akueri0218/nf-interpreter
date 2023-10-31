#ifndef _FPIOA_CONFIG_H_
#define _FPIOA_CONFIG_H_
#include <pin_cfg.h>

#define K210_ISP_RX 4
#define K210_ISP_TX 5

const fpioa_cfg_t g_fpioa_cfg =
{
    .version = PIN_CFG_VERSION,
    .functions_count = 2,
    .functions =
    {
        // fpioa_set_function
        {K210_ISP_RX, FUNC_UARTHS_RX},
        {K210_ISP_TX, FUNC_UARTHS_TX}
    }
};

// const pin_cfg_t g_pin_cfg =
// {
//     .version = PIN_CFG_VERSION,
//     .set_spi0_dvp_data = 1
// };

const power_bank_cfg_t g_power_bank_cfg =
{
    .version = PIN_CFG_VERSION,
    .power_banks_count = 1,
    .power_banks = 
    {
        {SYSCTL_POWER_BANK0, SYSCTL_POWER_V33}, // IO0-5
    }
};

#endif