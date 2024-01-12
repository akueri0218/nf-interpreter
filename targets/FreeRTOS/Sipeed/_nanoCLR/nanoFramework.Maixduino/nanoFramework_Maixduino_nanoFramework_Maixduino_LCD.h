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

#ifndef NANOFRAMEWORK_MAIXDUINO_NANOFRAMEWORK_MAIXDUINO_NANOFRAMEWORK_MAIXDUINO_LCD_H
#define NANOFRAMEWORK_MAIXDUINO_NANOFRAMEWORK_MAIXDUINO_NANOFRAMEWORK_MAIXDUINO_LCD_H

namespace nanoFramework_Maixduino
{
    namespace nanoFramework_Maixduino
    {
        struct LCD
        {
            // Helper Functions to access fields of managed object
            // Declaration of stubs. These functions are implemented by Interop code developers

            static void Inittialize(  HRESULT &hr );

            static void Clear( uint16_t param0, HRESULT &hr );

            static void DrawRect( uint16_t param0, uint16_t param1, uint16_t param2, uint16_t param3, uint16_t param4, HRESULT &hr );

            static void DrawText_Native( uint16_t param0, uint16_t param1, CLR_RT_TypedArray_UINT8 param2, uint16_t param3, HRESULT &hr );

        };
    }
}

#endif // NANOFRAMEWORK_MAIXDUINO_NANOFRAMEWORK_MAIXDUINO_NANOFRAMEWORK_MAIXDUINO_LCD_H
