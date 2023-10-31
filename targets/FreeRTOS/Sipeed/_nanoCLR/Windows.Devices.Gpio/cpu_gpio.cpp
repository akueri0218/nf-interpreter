//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

// Maixduino GPIO
// GPIO00-31 : GPIOHS
// GPIO32-39 : GPIO
//

#include "sys_dev_gpio_native_target.h"
#include "nf_rt_events_native.h"

#include "timers.h"

#include <devices.h>

#define GPIOHS_MAX_PINS  32 
#define GPIO_MAX_PINS    40 // GPIOHS(32) + GPIO(8) 
#define GPIO_BITS_PORT   32 // 32 bits per gpio port
#define TOTAL_GPIO_PORTS ((GPIO_MAX_PINS + (GPIO_BITS_PORT - 1)) / GPIO_BITS_PORT) // 2 ports

// Structure to hold information about input pin
struct gpio_input_state
{
    GPIO_PIN pinNumber;                    // Pin number
    TimerHandle_t debounceTimer;           // debounce timer for this Pin
    GPIO_INTERRUPT_SERVICE_ROUTINE isrPtr; // Ptr to user ISR or null
    uint32_t debounceMs;                   // debounce Millsecs, no debonce=0
    uint8_t mode;                          // Interrupt mode
    void *param;                           // Param to user isr call
    bool expected;                         // Expected state for debounce handler
    bool waitingDebounce;                  // True if waiting for debounce timer to complete
};

// Array of gpio_input_state ptrs for each gpio port
// each port (GPIO1 to GPIO5) has a low & high 16 bit port.
typedef gpio_input_state *statePortArray[GPIO_BITS_PORT];

// For each 16 bit gpio port we have a ptr to an array ptrs to gpio_input_state
// These are initialised only when an input gpio bit is enabled keeping memory use to the minimum
static statePortArray *port_array[TOTAL_GPIO_PORTS];

// Array of bits for saving reserved state
static uint32_t pinReserved[TOTAL_GPIO_PORTS];

// this is an utility define to get a port number from our "encoded" pin number
// pin 0 -> (GPIO_MAX_PINS - 1)
// i.e Port 0 = Pins 0 to 15, port 1 = pins 16 to 31 etc
#define GetIoPort(pinNumber)      (pinNumber / GPIO_BITS_PORT)
#define GetIoBit(pinNumber)       (pinNumber % GPIO_BITS_PORT)
#define IsValidGpioPin(pinNumber) (pinNumber < GPIO_MAX_PINS)

// gpio controller
extern handle_t gpiohs;
extern handle_t gpio;

#define GetGpioHandle(pinNumber) (pinNumber < GPIO_BITS_PORT ? gpiohs : gpio)

void Gpio_IRQHandler(uint32_t pin, void* userdata)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // only gpiohs port supports callback
    if(pin >= GPIOHS_MAX_PINS)
    {
        return;
    }

    statePortArray *states = port_array[0];

    if(states)
    {
        gpio_input_state *pState = (gpio_input_state*)states[GetIoBit(pin)];

        if(pState)
        {
            // Ignore any pin changes during debounce
            if(!pState->waitingDebounce)
            {
                // If debounce timer defined then first wait for it to expire
                if(pState->debounceMs > 0)
                {
                    pState->waitingDebounce = true;

                    // Start Debounce timer
                    xTimerChangePeriodFromISR(
                        pState->debounceTimer,
                        pdMS_TO_TICKS(pState->debounceMs),
                        &xHigherPriorityTaskWoken);
                }
                else if(pState->isrPtr)
                {
                    gpio_pin_value_t pinVal = gpio_get_pin_value(GetGpioHandle(pState->pinNumber), GetIoBit(pState->pinNumber));
                    
                    pState->isrPtr(pState->pinNumber, pinVal ? GpioPinValue_High : GpioPinValue_Low, pState->param);
                }
            }
        }
    }

    if(xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR();
    }
}

void Gpio_DebounceHandler(TimerHandle_t xTimer)
{
    gpio_input_state *pState = (gpio_input_state *)pvTimerGetTimerID(xTimer);

    bool actual = (GpioPinValue)gpio_get_pin_value(GetGpioHandle(pState->pinNumber), GetIoBit(pState->pinNumber));

    if (actual == pState->expected)
    {
        pState->isrPtr(pState->pinNumber, actual, pState->param);

        if (pState->mode == GPIO_INT_EDGE_BOTH)
        {                          // both edges
            pState->expected ^= 1; // update expected state
        }
    }

    pState->waitingDebounce = false;
}

// Get pointer to gpio_input_state for Gpio pin
// return NULL if not found
gpio_input_state *GetInputState(GPIO_PIN pinNumber)
{
    int port = GetIoPort(pinNumber);
    int bit = GetIoBit(pinNumber);

    if (port_array[port] == NULL)
        return NULL;

    statePortArray *inputStates = port_array[port];
    return *inputStates[bit];
}

// Allocate a new gpio_input_state and add to end of list
// if already exist then just return current ptr
gpio_input_state *AllocateGpioInputState(GPIO_PIN pinNumber)
{
    int port = GetIoPort(pinNumber);
    int bit = GetIoBit(pinNumber);

    if (port_array[port] == NULL)
    {
        port_array[port] = (statePortArray *)platform_malloc(sizeof(statePortArray));
        if (port_array[port] == NULL)
            return NULL;

        memset(port_array[port], 0, sizeof(statePortArray));
    }

    statePortArray *inputStates = port_array[port];

    gpio_input_state *pState = (*inputStates)[bit];
    if (pState == NULL)
    {
        pState = (gpio_input_state *)platform_malloc(sizeof(gpio_input_state));
        memset(pState, 0, sizeof(gpio_input_state));
        pState->pinNumber = pinNumber;
        (*inputStates)[bit] = pState;
    }
    return pState;
}

// Delete gpio_input_state from List and tidy up ( Timer & ISR handler )
void DeleteInputState(GPIO_PIN pinNumber)
{
    int port = GetIoPort(pinNumber);
    int bit = GetIoBit(pinNumber);

    statePortArray *inputStates = port_array[port];
    if (inputStates == NULL)
        return;

    gpio_input_state *pState = (*inputStates)[bit];
    if (pState)
    {
        if (pState->debounceTimer != 0)
        {
            xTimerDelete(pState->debounceTimer, 100);
        }

        // interrupt handler availables only gpiohs pins
        if(pinNumber > GPIOHS_MAX_PINS)
        {
            gpio_set_on_changed(gpiohs, pinNumber, (gpio_on_changed_t)NULL, NULL);
        }

        platform_free(pState);
        (*inputStates)[bit] = NULL;
    }
}

bool CPU_GPIO_Initialize()
{
    configASSERT(gpiohs);
    configASSERT(gpio);

    // All port ptrs are null
    memset(port_array, 0, sizeof(port_array));

    // Make sure all pins are not reserved
    memset(pinReserved, 0, sizeof(pinReserved));

    return true;
}

// System.Device.Gpio
bool CPU_GPIO_Uninitialize()
{
    io_close(gpiohs);
    io_close(gpio);

    // First remove any active pin states
    for (int pinNumber = 0; pinNumber < GPIO_MAX_PINS; pinNumber++)
    {
        DeleteInputState(pinNumber);
    }

    // Remove statePortArray if any
    for (int port = 0; port < TOTAL_GPIO_PORTS; port++)
    {
        statePortArray *inputStates = port_array[port];
        if (inputStates != NULL)
        {
            platform_free(port_array[port]); // free up inputStates array
            port_array[port] = NULL;
        }
    }

    return true;
}

// Set/reset reserved state of pin
bool CPU_GPIO_ReservePin(GPIO_PIN pinNumber, bool fReserve)
{
    // Check if valid pin number
    if (!IsValidGpioPin(pinNumber))
        return false;

    int port = GetIoPort(pinNumber);
    int bit = 1 << (GetIoBit(pinNumber));

    GLOBAL_LOCK();

    if (fReserve)
    {
        if (pinReserved[port] & bit)
        {
            GLOBAL_UNLOCK();
            return false; // already reserved
        }

        pinReserved[port] |= bit;
    }
    else
    {
        pinReserved[port] &= ~bit;
    }

    GLOBAL_UNLOCK();
    return true;
}

// System.Device.Gpio
// Return if Pin is reserved
bool CPU_GPIO_PinIsBusy(GPIO_PIN pinNumber)
{
    // Check if valid pin number
    if (!IsValidGpioPin(pinNumber))
        return false;

    int port = GetIoPort(pinNumber);
    int sh = GetIoBit(pinNumber);

    return (pinReserved[port] >> sh) & 1;
}

// System.Device.Gpio
// Return maximum number of pins
int32_t CPU_GPIO_GetPinCount()
{
    return GPIO_MAX_PINS;
}

// Get current state of pin
GpioPinValue CPU_GPIO_GetPinState(GPIO_PIN pinNumber)
{
    return (GpioPinValue)gpio_get_pin_value(GetGpioHandle(pinNumber), GetIoBit(pinNumber));
}

// Set Pin state
void CPU_GPIO_SetPinState(GPIO_PIN pinNumber, GpioPinValue PinState)
{
    gpio_pin_value_t val = PinState ? GPIO_PV_HIGH : GPIO_PV_LOW;

    gpio_set_pin_value(GetGpioHandle(pinNumber), GetIoBit(pinNumber), val);
}

// Toggle pin state
void CPU_GPIO_TogglePinState(GPIO_PIN pinNumber)
{
    gpio_toggle_pin_value(GetGpioHandle(pinNumber), GetIoBit(pinNumber));
}

//
// CPU_GPIO_EnableInputPin
// Enable input pin
//
bool CPU_GPIO_EnableInputPin(
    GPIO_PIN pinNumber,
    uint32_t debounceTimeMilliseconds,
    GPIO_INTERRUPT_SERVICE_ROUTINE pinISR,
    void *isrParam,
    GPIO_INT_EDGE intEdge,
    GpioPinDriveMode driveMode)
{
    gpio_input_state *pState;

    // Check if valid pin number
    if (!IsValidGpioPin(pinNumber))
        return false;

    // Check Input drive mode
    if (driveMode >= (int)GpioPinDriveMode_Output)
        return false;

    if (!CPU_GPIO_SetDriveMode(pinNumber, driveMode))
        return false;

    pState = AllocateGpioInputState(pinNumber);

    if (pinISR != NULL && (pState->isrPtr == NULL))
    {
        // gpio_set_pin_edge, gpio_set_on_changed are supported only for gpiohs
        if(GetIoPort(pinNumber) == 0)
        {
            // TODO: set pin isr
            gpio_set_on_changed(GetIoPort(pinNumber), GetIoBit(pinNumber), (gpio_on_changed_t)(Gpio_IRQHandler), NULL);

            gpio_pin_edge_t edge;

            switch(intEdge)
            {
                case GPIO_INT_NONE:
                    edge = GPIO_PE_NONE;
                    break;
                case GPIO_INT_EDGE_LOW:
                    edge = GPIO_PE_FALLING;
                    break;
                case GPIO_INT_EDGE_HIGH:
                    edge = GPIO_PE_RISING;
                    break;
                case GPIO_INT_EDGE_BOTH:
                    edge = GPIO_PE_BOTH;
                    break;
                default:
                /* NOT supported */
                //case GPIO_INT_LEVEL_HIGH:
                //case GPIO_INT_LEVEL_LOW:
                    intEdge = GPIO_INT_NONE;
                    edge = GPIO_PE_NONE;
                    break;
            }

            gpio_set_pin_edge(GetGpioHandle(pinNumber), GetIoBit(pinNumber), edge);
        }
        else {
            intEdge = GPIO_INT_NONE;
        }

        // store parameters & configs
        pState->isrPtr = pinISR;
        pState->mode = intEdge;
        pState->param = (void *)isrParam;
        pState->debounceMs = debounceTimeMilliseconds;

        // Set up expected new value for debounce
        if (pState->debounceMs > 0)
        {
            if (pState->debounceTimer == 0)
            {
                // Create timer if it doesn't already exist for this pin
                pState->debounceTimer = xTimerCreate("debounce", 100, pdFALSE, (void *)pState, Gpio_DebounceHandler);
            }
            switch (intEdge)
            {
                case GPIO_INT_NONE:
                case GPIO_INT_EDGE_LOW:
                case GPIO_INT_LEVEL_LOW:
                    pState->expected = false;
                    break;

                case GPIO_INT_EDGE_HIGH:
                case GPIO_INT_LEVEL_HIGH:
                    pState->expected = true;
                    break;

                case GPIO_INT_EDGE_BOTH:
                    pState->expected = !CPU_GPIO_GetPinState(pinNumber); // Use NOT current state
                    break;
            }
        }
    }
    else if (pinISR == NULL && (pState->isrPtr != NULL))
    {
        if(GetIoPort(pinNumber) == 0)
        {
            gpio_set_on_changed(GetGpioHandle(pinNumber), GetIoBit(pinNumber), NULL, (void*)NULL);
        }

        // clear parameters & configs
        pState->isrPtr = NULL;
        pState->mode = GPIO_INT_NONE;
        pState->param = NULL;
        pState->debounceMs = 0;
    }

    return true;
}

// Enable an output pin
//
// pinNumber    -   Gpio pin number
// InitialState -   Initial state of pin
// driveMode    -   Drive mode and resistors
// return       -   True if successful, false invalid pin, pin not putput, invalid drive mode for ouptput
//
bool CPU_GPIO_EnableOutputPin(GPIO_PIN pinNumber, GpioPinValue InitialState, GpioPinDriveMode driveMode)
{
    // Check if valid pin number
    if (!IsValidGpioPin(pinNumber))
        return false;

    // check is output drive mode
    if (driveMode < (int)GpioPinDriveMode_Output)
        return false;

    if (CPU_GPIO_SetDriveMode(pinNumber, driveMode) == false)
        return false;

    CPU_GPIO_SetPinState(pinNumber, InitialState);

    return true;
}

void CPU_GPIO_DisablePin(GPIO_PIN pinNumber, GpioPinDriveMode driveMode, uint32_t alternateFunction)
{
    GLOBAL_LOCK();

    CPU_GPIO_SetDriveMode(pinNumber, driveMode);

    DeleteInputState(pinNumber);

    // if (alternateFunction)
    // {
    //     GPIO_PinMux(GPIO_PORT(pinNumber), GPIO_PIN(pinNumber), alternateFunction);
    // }

    GLOBAL_UNLOCK();

    CPU_GPIO_ReservePin(pinNumber, false);
}

// Validate pin and set drive mode
// return true if ok
bool CPU_GPIO_SetDriveMode(GPIO_PIN pinNumber, GpioPinDriveMode driveMode)
{
    // Check if valid pin number
    if (!IsValidGpioPin(pinNumber))
        return false;

    gpio_drive_mode_t mode;

    switch(driveMode)
    {
        case GpioPinDriveMode_Input:
            mode = GPIO_DM_INPUT;
            break;
        case GpioPinDriveMode_InputPullDown:
            mode = GPIO_DM_INPUT_PULL_DOWN;
            break;
        case GpioPinDriveMode_InputPullUp:
            mode = GPIO_DM_INPUT_PULL_UP;
            break;
        case GpioPinDriveMode_Output:
            mode = GPIO_DM_OUTPUT;
            break;
        default:
        /* NOT supported */
        //case GpioPinDriveMode_OutputOpenDrain:
        //case GpioPinDriveMode_OutputOpenDrainPullUp:
        //case GpioPinDriveMode_OutputOpenSource:
        //case GpioPinDriveMode_OutputOpenSourcePullDown:
            return false;
    }

    gpio_set_drive_mode(GetGpioHandle(pinNumber), GetIoBit(pinNumber), mode);

    return true;
}

// System.Device.Gpio
bool CPU_GPIO_DriveModeSupported(GPIO_PIN pinNumber, GpioPinDriveMode driveMode)
{
    // Check if valid pin number
    if (!IsValidGpioPin(pinNumber))
        return false;

    bool driveModeSupported = false;

    // check if the requested drive mode is supported
    if ((driveMode == GpioPinDriveMode_Input) || (driveMode == GpioPinDriveMode_InputPullDown) ||
        (driveMode == GpioPinDriveMode_InputPullUp) || (driveMode == GpioPinDriveMode_Output))
    {
        driveModeSupported = true;
    }

    return driveModeSupported;
}

uint32_t CPU_GPIO_GetPinDebounce(GPIO_PIN pinNumber)
{
    // Check if valid pin number
    if (IsValidGpioPin(pinNumber))
    {
        gpio_input_state *ptr = GetInputState(pinNumber);
        if (ptr)
            return ptr->debounceMs;
    }

    return 0;
}

bool CPU_GPIO_SetPinDebounce(GPIO_PIN pinNumber, uint32_t debounceTimeMilliseconds)
{
    // Check if valid pin number
    if (IsValidGpioPin(pinNumber))
    {
        gpio_input_state *ptr = GetInputState(pinNumber);
        if (ptr)
        {
            ptr->debounceMs = debounceTimeMilliseconds;
            return true;
        }
    }
    return false;
}
