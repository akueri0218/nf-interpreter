//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include <nanoHAL_v2.h>
#include <WireProtocol.h>
#include <WireProtocol_Message.h>
#include <WireProtocol_HAL_Interface.h>

#include <stddef.h>
#include <devices.h>

bool WP_Initialise();

static bool WP_Port_Intitialised = false;

bool WP_Initialise()
{
    // uarths has already initialized in _bsp_init
    WP_Port_Intitialised = true;

    if (!WP_Port_Intitialised)
    {
        vTaskSuspend(NULL);
    }

    return WP_Port_Intitialised;
}

void WP_ReceiveBytes(uint8_t **ptr, uint32_t *size)
{
    // TODO: Initialise Port if not already done, Wire Protocol should be calling this directly at startup
    if (!WP_Port_Intitialised)
    {
        WP_Initialise();
    }

    // save for later comparison
    size_t requestedSize = (size_t)*size;

    // check for request with 0 size
    if(*size) {
        size_t read = 0;
        read = uarths_read(*ptr, requestedSize);

        *size -= read;
    }
}

uint8_t WP_TransmitMessage(WP_Message *message)
{
    if (!WP_Port_Intitialised)
    {
        WP_Initialise();
    }

    uarths_write((uint8_t*)&message->m_header, sizeof(message->m_header));

    // if there is anything on the payload send it to the output stream
    if (message->m_header.m_size && message->m_payload)
    {
        uarths_write(message->m_payload, message->m_header.m_size);
    }

    return true;
}
