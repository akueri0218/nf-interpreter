//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------

#ifndef NANOFRAMEWORK_MAIXDUINO_H
#define NANOFRAMEWORK_MAIXDUINO_H

#include <nanoCLR_Interop.h>
#include <nanoCLR_Runtime.h>
#include <nanoPackStruct.h>

struct Library_nanoFramework_Maixduino_nanoFramework_Maixduino_Color
{
    static const int FIELD_STATIC__BLACK = 0;
    static const int FIELD_STATIC__NAVY = 1;
    static const int FIELD_STATIC__DARKGREEN = 2;
    static const int FIELD_STATIC__DARKCYAN = 3;
    static const int FIELD_STATIC__MAROON = 4;
    static const int FIELD_STATIC__PURPLE = 5;
    static const int FIELD_STATIC__OLIVE = 6;
    static const int FIELD_STATIC__LIGHTGREY = 7;
    static const int FIELD_STATIC__DARKGREY = 8;
    static const int FIELD_STATIC__BLUE = 9;
    static const int FIELD_STATIC__GREEN = 10;
    static const int FIELD_STATIC__CYAN = 11;
    static const int FIELD_STATIC__RED = 12;
    static const int FIELD_STATIC__MAGENTA = 13;
    static const int FIELD_STATIC__YELLOW = 14;
    static const int FIELD_STATIC__WHITE = 15;
    static const int FIELD_STATIC__ORANGE = 16;
    static const int FIELD_STATIC__GREENYELLOW = 17;
    static const int FIELD_STATIC__PINK = 18;

    //--//
};

struct Library_nanoFramework_Maixduino_nanoFramework_Maixduino_KModel
{
    static const int FIELD___modelData = 1;
    static const int FIELD___outputSize = 2;

    NANOCLR_NATIVE_DECLARE(LoadKModel___STATIC__VOID__U4__SZARRAY_U1);

    //--//
};

struct Library_nanoFramework_Maixduino_nanoFramework_Maixduino_KPUImage
{
    static const int FIELD__data = 1;
    static const int FIELD__previewData = 2;

    NANOCLR_NATIVE_DECLARE(LoadKPUImage___STATIC__VOID__U4__SZARRAY_U1);

    //--//
};

struct Library_nanoFramework_Maixduino_nanoFramework_Maixduino_KPU
{
    static const int FIELD_STATIC__FaceDetectModel = 19;
    static const int FIELD_STATIC___model = 20;

    NANOCLR_NATIVE_DECLARE(FaceDetect_Native___STATIC__U4__SZARRAY_U1__SZARRAY_U4);
    NANOCLR_NATIVE_DECLARE(LoadModel_Native___STATIC__VOID__SZARRAY_U1);
    NANOCLR_NATIVE_DECLARE(Run_Native___STATIC__VOID__SZARRAY_U1__SZARRAY_U1);

    //--//
};

struct Library_nanoFramework_Maixduino_nanoFramework_Maixduino_KPU__FaceDetectInfo
{
    static const int FIELD__x1 = 1;
    static const int FIELD__y1 = 2;
    static const int FIELD__x2 = 3;
    static const int FIELD__y2 = 4;

    //--//
};

struct Library_nanoFramework_Maixduino_nanoFramework_Maixduino_LCD
{
    NANOCLR_NATIVE_DECLARE(Inittialize___STATIC__VOID);
    NANOCLR_NATIVE_DECLARE(Clear___STATIC__VOID__U2);
    NANOCLR_NATIVE_DECLARE(DrawRect___STATIC__VOID__U2__U2__U2__U2__U2);
    NANOCLR_NATIVE_DECLARE(DrawText_Native___STATIC__VOID__U2__U2__SZARRAY_U1__U2);
    NANOCLR_NATIVE_DECLARE(DrawPicture___STATIC__VOID__SZARRAY_U1);

    //--//
};

extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_Maixduino;

#endif // NANOFRAMEWORK_MAIXDUINO_H
