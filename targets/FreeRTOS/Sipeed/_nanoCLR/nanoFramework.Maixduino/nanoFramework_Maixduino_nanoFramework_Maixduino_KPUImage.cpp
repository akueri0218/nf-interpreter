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
#include "nanoFramework_Maixduino_nanoFramework_Maixduino_KPUImage.h"

#include "w25qxx.h"

using namespace nanoFramework_Maixduino::nanoFramework_Maixduino;


void KPUImage::LoadKPUImage( unsigned int param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr )
{

    (void)param0;
    (void)param1;
    (void)hr;


    ////////////////////////////////
    // implementation starts here //

    printf("load kpu image\n");

    w25qxx_read_data_safe(param0, param1.GetBuffer(), 320 * 240 * 3);

    // implementation ends here   //
    ////////////////////////////////


}
