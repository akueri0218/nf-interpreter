/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <devices.h>
#include <stdio.h>
#include "st7789.h"

#define SPI_HIGH_CLOCK_RATE 20000000U // 25000000U
#define SPI_LOW_CLOCK_RATE   3200000U
#define WAIT_CYCLE      0U

#define RST_GPIONUM 30
#define DCX_GPIONUM 31

#define SPI_SLAVE_SELECT 3

enum _instruction_length
{
    INSTRUCTION_LEN_0 = 0,
    INSTRUCTION_LEN_8 = 8,
    INSTRUCTION_LEN_16 = 16,
    INSTRUCTION_LEN_32 = 32,
} ;

enum _address_length
{
    ADDRESS_LEN_0 = 0,
    ADDRESS_LEN_8 = 8,
    ADDRESS_LEN_16 = 16,
    ADDRESS_LEN_32 = 32,
} ;

enum _frame_length
{
    FRAME_LEN_0 = 0,
    FRAME_LEN_8 = 8,
    FRAME_LEN_16 = 16,
    FRAME_LEN_32 = 32,
} ;

extern handle_t gpiohs;
extern handle_t spi0;
handle_t spi_dfs8;
handle_t spi_dfs16;
handle_t spi_dfs32;

void init_dcx()
{
    configASSERT(gpiohs);
    gpio_set_drive_mode(gpiohs, DCX_GPIONUM, GPIO_DM_OUTPUT);
    gpio_set_pin_value(gpiohs, DCX_GPIONUM, GPIO_PV_HIGH);
}

void set_dcx_control()
{
    configASSERT(gpiohs);
    gpio_set_pin_value(gpiohs, DCX_GPIONUM, GPIO_PV_LOW);
}

void set_dcx_data()
{
    configASSERT(gpiohs);
    gpio_set_pin_value(gpiohs, DCX_GPIONUM, GPIO_PV_HIGH);
}

void init_rst()
{
    configASSERT(gpiohs);
    gpio_set_drive_mode(gpiohs, RST_GPIONUM, GPIO_DM_OUTPUT);
    gpio_set_pin_value(gpiohs, RST_GPIONUM, GPIO_PV_HIGH);
}

void set_rst(gpio_pin_value_t val)
{
    gpio_set_pin_value(gpiohs, RST_GPIONUM, val);
}

void spi_control_init()
{
    configASSERT(spi0);
    spi_dfs8 = spi_get_device(spi0, SPI_MODE_0, SPI_FF_OCTAL, 1 << SPI_SLAVE_SELECT, FRAME_LEN_8);
    spi_dev_config_non_standard(spi_dfs8, INSTRUCTION_LEN_8, ADDRESS_LEN_0, WAIT_CYCLE, SPI_AITM_AS_FRAME_FORMAT);
    
    spi_dfs16 = spi_get_device(spi0, SPI_MODE_0, SPI_FF_OCTAL, 1 << SPI_SLAVE_SELECT, FRAME_LEN_16);
    spi_dev_config_non_standard(spi_dfs16, INSTRUCTION_LEN_16, ADDRESS_LEN_0, WAIT_CYCLE, SPI_AITM_AS_FRAME_FORMAT);
    
    spi_dfs32 = spi_get_device(spi0, SPI_MODE_0, SPI_FF_OCTAL, 1 << SPI_SLAVE_SELECT, FRAME_LEN_32);
    spi_dev_config_non_standard(spi_dfs32, INSTRUCTION_LEN_0, ADDRESS_LEN_32, WAIT_CYCLE, SPI_AITM_AS_FRAME_FORMAT);

    spi_dev_set_clock_rate(spi_dfs8, SPI_LOW_CLOCK_RATE);
    spi_dev_set_clock_rate(spi_dfs16, SPI_HIGH_CLOCK_RATE);
    spi_dev_set_clock_rate(spi_dfs32, SPI_HIGH_CLOCK_RATE);
}

void tft_hard_init(void)
{
    init_dcx();
    init_rst();
    set_rst(GPIO_PV_LOW);
    spi_control_init();
    set_rst(GPIO_PV_HIGH);
}

void tft_write_command(uint8_t cmd)
{
    set_dcx_control();
    io_write(spi_dfs8, (const uint8_t *)(&cmd), 1);
}

void tft_write_byte(uint8_t* data_buf, uint32_t length)
{
    set_dcx_data();
    io_write(spi_dfs8, (const uint8_t *)(data_buf), length);
}

void tft_write_half(uint16_t* data_buf, uint32_t length)
{
    set_dcx_data();
    io_write(spi_dfs16, (const uint8_t *)(data_buf), length * 2);
}

void tft_write_word(uint32_t* data_buf, uint32_t length)
{
    set_dcx_data();
    io_write(spi_dfs32, (const uint8_t *)data_buf, length * 4);
}

void tft_fill_data(uint32_t* data_buf, uint32_t length)
{
    set_dcx_data();
    spi_dev_fill(spi_dfs32, 0, *data_buf, *data_buf, length - 1);
}
