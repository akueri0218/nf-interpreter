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
#include "nanoFramework_Maixduino_nanoFramework_Maixduino_KModel.h"

#include <stdio.h>
#include "w25qxx.h"

using namespace nanoFramework_Maixduino::nanoFramework_Maixduino;


void KModel::LoadKModel( unsigned int param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr )
{

    (void)param0; // address
    (void)param1; // model data
    (void)hr;


    ////////////////////////////////
    // implementation starts here //

    printf("kmodel load address: 0x%X\n", param0);

    int size = param1.GetSize();

    w25qxx_read_data_safe(param0, param1.GetBuffer(), size);

    // implementation ends here   //
    ////////////////////////////////


}
