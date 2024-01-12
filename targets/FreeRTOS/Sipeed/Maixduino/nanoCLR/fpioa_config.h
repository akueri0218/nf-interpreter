#ifndef _FPIOA_CONFIG_H_
#define _FPIOA_CONFIG_H_
#include <pin_cfg.h>

#define K210_ISP_RX 4
#define K210_ISP_TX 5

#define LCD_CS          (36)
#define LCD_RST         (37)
#define LCD_DC          (38)
#define LCD_WR          (39)

#define LCD_RST_GPIONUM (30)
#define LCD_DCX_GPIONUM (31)

#define LCD_SLAVE_SELECT (3)

#define FUNC_SPI_SS(x,y) FUNC_SPI##x##_SS##y
#define FUNC_SPI0_SS(x) FUNC_SPI_SS(0, x)

const fpioa_cfg_t g_fpioa_cfg =
{
    .version = PIN_CFG_VERSION,
    .functions_count = 6,
    .functions =
    {
        // fpioa_set_function
        {K210_ISP_RX, FUNC_UARTHS_RX},
        {K210_ISP_TX, FUNC_UARTHS_TX},

        {LCD_CS,  FUNC_SPI0_SS3},
        {LCD_RST, FUNC_GPIOHS0 + LCD_RST_GPIONUM},
        {LCD_DC,  FUNC_GPIOHS0 + LCD_DCX_GPIONUM},
        {LCD_WR,  FUNC_SPI0_SCLK}
    }
};

const pin_cfg_t g_pin_cfg =
{
    .version = PIN_CFG_VERSION,
    .set_spi0_dvp_data = 1
};

const power_bank_cfg_t g_power_bank_cfg =
{
    .version = PIN_CFG_VERSION,
    .power_banks_count = 2,
    .power_banks = 
    {
        {SYSCTL_POWER_BANK0, SYSCTL_POWER_V33}, // IO0-5
        
        {SYSCTL_POWER_BANK6, SYSCTL_POWER_V18}, // IO36-41
    }
};

#endif