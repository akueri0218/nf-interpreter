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

#ifndef NANOFRAMEWORK_MAIXDUINO_NANOFRAMEWORK_MAIXDUINO_NANOFRAMEWORK_MAIXDUINO_KPU_H
#define NANOFRAMEWORK_MAIXDUINO_NANOFRAMEWORK_MAIXDUINO_NANOFRAMEWORK_MAIXDUINO_KPU_H

namespace nanoFramework_Maixduino
{
    namespace nanoFramework_Maixduino
    {
        struct KPU
        {
            // Helper Functions to access fields of managed object
            // Declaration of stubs. These functions are implemented by Interop code developers

            static unsigned int FaceDetect_Native( CLR_RT_TypedArray_UINT8 param0, CLR_RT_TypedArray_UINT32 param1, HRESULT &hr );

            static void LoadModel_Native( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr );

            static void Run_Native( CLR_RT_TypedArray_UINT8 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr );

        };
    }
}

#endif // NANOFRAMEWORK_MAIXDUINO_NANOFRAMEWORK_MAIXDUINO_NANOFRAMEWORK_MAIXDUINO_KPU_H
