//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------

#include "nanoFramework_Maixduino.h"
#include "nanoFramework_Maixduino_nanoFramework_Maixduino_LCD.h"

#include "lcd.h"
#include <stdio.h>

using namespace nanoFramework_Maixduino::nanoFramework_Maixduino;

void LCD::Inittialize(  HRESULT &hr )
{

    (void)hr;


    ////////////////////////////////
    // implementation starts here //

    printf("lcd_init\n");

    lcd_init();

    // implementation ends here   //
    ////////////////////////////////


}

void LCD::Clear( uint16_t param0, HRESULT &hr )
{

    (void)param0; // color
    (void)hr;


    ////////////////////////////////
    // implementation starts here //

    printf("lcd_clear color: 0x%04X\n", param0);

    lcd_clear(param0);

    // implementation ends here   //
    ////////////////////////////////


}

void LCD::DrawRect( uint16_t param0, uint16_t param1, uint16_t param2, uint16_t param3, uint16_t param4, HRESULT &hr )
{

    (void)param0; // x1
    (void)param1; // y1
    (void)param2; // x2
    (void)param3; // y2
    (void)param4; // color
    (void)hr;


    ////////////////////////////////
    // implementation starts here //

    printf("lcd_draw_rectangle x1: %d, y1: %d, x2: %d, y2: %d, color: 0x%04X\n", param0, param1, param2, param3, param4);

    if(param2 >= LCD_Y_MAX)
    {
        param2 = LCD_Y_MAX - 1;
    }
    if(param3 >= LCD_X_MAX)
    {
        param3 = LCD_X_MAX - 1;
    }

    lcd_draw_rectangle(param0, param1, param2, param3, 2, param4);

    // implementation ends here   //
    ////////////////////////////////


}

void LCD::DrawText_Native( uint16_t param0, uint16_t param1, CLR_RT_TypedArray_UINT8 param2, uint16_t param3, HRESULT &hr )
{

    (void)param0; // x
    (void)param1; // y
    (void)param2; // text
    (void)param3; // color
    (void)hr;


    ////////////////////////////////
    // implementation starts here //

    uint32_t size = param2.GetSize() + 1;

    uint8_t* text = (uint8_t*)platform_malloc(size);

    memcpy(text, param2.GetBuffer(), size);

    text[size-1] = 0;

    printf("lcd_draw_string x: %d, y: %d, text: \"%s\", color: 0x%04X\n", param0, param1, text, param3);

    lcd_draw_string(param0, param1, (char*)text, param3);

    // implementation ends here   //
    ////////////////////////////////


}

void LCD::DrawPicture( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{

    (void)param0;
    (void)hr;


    ////////////////////////////////
    // implementation starts here //

    printf("draw picture\n");

    size_t size = 320 * 240 * 2;

    uint32_t* buf = (uint32_t*)platform_malloc(size);

    memcpy(buf, param0.GetBuffer(), size);

    lcd_draw_picture_safe(0, 0, 320, 240, buf);

    // implementation ends here   //
    ////////////////////////////////


}
