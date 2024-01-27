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
#include "nanoFramework_Maixduino_nanoFramework_Maixduino_KPU.h"

#include <stdio.h>
#include <devices.h>
#include "region_layer.h"

using namespace nanoFramework_Maixduino::nanoFramework_Maixduino;

handle_t model_context;
static region_layer_t face_detect_rl;
static obj_info_t face_detect_info;
#define ANCHOR_NUM 5 
static float anchor[ANCHOR_NUM * 2] = {1.889,2.5245,  2.9465,3.94056, 3.99987,5.3658, 5.155437,6.92275, 6.718375,9.01025};

unsigned int KPU::FaceDetect_Native( CLR_RT_TypedArray_UINT8 param0, CLR_RT_TypedArray_UINT32 param1, HRESULT &hr )
{

    (void)param0;
    (void)param1;
    (void)hr;
    unsigned int retValue = 0;

    ////////////////////////////////
    // implementation starts here //

    printf("kpu face detect\n");

    face_detect_rl.anchor_number = ANCHOR_NUM;
    face_detect_rl.anchor = anchor;
    face_detect_rl.threshold = 0.7;
    face_detect_rl.nms_value = 0.3;
    region_layer_init(&face_detect_rl, 20, 15, 30, 320, 240);
    
    face_detect_rl.input = (float*)param0.GetBuffer();
    
    region_layer_run(&face_detect_rl, &face_detect_info);

    for(int i = 0; i < face_detect_info.obj_number; i++)
    {
        param1[i * 4 + 0] = face_detect_info.obj[i].x1;
        param1[i * 4 + 1] = face_detect_info.obj[i].y1;
        param1[i * 4 + 2] = face_detect_info.obj[i].x2;
        param1[i * 4 + 3] = face_detect_info.obj[i].y2;
    }

    retValue = face_detect_info.obj_number;

    // implementation ends here   //
    ////////////////////////////////

    return retValue;
}

void KPU::LoadModel_Native( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{

    (void)param0;
    (void)hr;


    ////////////////////////////////
    // implementation starts here //

    printf("kpu load model\n");

    model_context = kpu_model_load_from_buffer(param0.GetBuffer());

    // implementation ends here   //
    ////////////////////////////////


}

void KPU::Run_Native( CLR_RT_TypedArray_UINT8 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr )
{

    (void)param0;
    (void)param1;
    (void)hr;


    ////////////////////////////////
    // implementation starts here //

    printf("kpu run\n");

    if(kpu_run(model_context, param0.GetBuffer()) != 0)
    {
        hr = CLR_E_FAIL;
        return;
    }

    float *output;
    size_t output_size;

    kpu_get_output(model_context, 0, (uint8_t **)&output, &output_size);

    memcpy(param1.GetBuffer(), output, output_size);

    // implementation ends here   //
    ////////////////////////////////


}
