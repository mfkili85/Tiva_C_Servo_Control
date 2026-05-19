/*
 * Servo_task.c
 *
 *  Created on: May 18, 2026
 *      Author: MFKILI
 */
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


#define SERVOTASKSTACKSIZE        512         // Stack size in words

#define SERVO_ITEM_SIZE           sizeof(uint8_t)
#define SERVO_QUEUE_SIZE          5

#define SERVO_TOGGLE_DELAY        250
xQueueHandle g_pSERVOQueue;

extern xSemaphoreHandle g_pUARTSemaphore;


static void SERVOTask(void *pvParameters)
{
    uint8_t i8Message;

    // Define the pulse width constants
    const uint32_t POS_0_DEG  = 781;  // 1ms
    const uint32_t POS_45_DEG  = 1172;
    const uint32_t POS_90_DEG = 1563; // 2ms

    while(1)
    {
        // Wait indefinitely for a message from the queue
        if(xQueueReceive(g_pSERVOQueue, &i8Message, portMAX_DELAY) == pdPASS)
        {
            if(i8Message == LEFT_BUTTON)
            {
                // Move to -90 degrees
                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, POS_0_DEG);
            }
            else if(i8Message == RIGHT_BUTTON)
            {
                // Move to +90 degrees
                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, POS_90_DEG);


            }
        }
    }
}

uint32_t
ServoTaskInit(void)
{
    //
    // Initialize PWM output PD0
    //
    // 1. Set the PWM clock provider (System Clock / 64)
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    // 2. Enable Peripherals: PWM Module 0 and GPIO Port D
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));

    // 3. Configure PD0 as a PWM pin
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinConfigure(GPIO_PD0_M0PWM6);

    // 4. Configure PWM Generator 3 (Controls PWM6 and PWM7)
    // We use Down-count mode for standard PWM
    PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    // Set the period for 50Hz (20ms) based on 50MHz System Clock
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 15625);

    // Set initial position to 45 degrees (1.5ms)
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 1172);

    // 7. Enable the PWM Generator
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);

    // 8. Enable the Output for PD0 (PWM_OUT_6)
    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);

    g_pSERVOQueue = xQueueCreate(SERVO_QUEUE_SIZE, SERVO_ITEM_SIZE);
    if(g_pSERVOQueue == NULL)
    {
        return(1);
    }

    // 2. Attempt to create the task
    // Note: Changed the error message to reflect that it FAILED.
    if(xTaskCreate(SERVOTask, (const char *)"SERVO", SERVOTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_SERVO_TASK, NULL) != pdTRUE)
    {
        // If we reach here, xTaskCreate failed.
        // Try reducing SERVOTASKSTACKSIZE to 128 if this keeps happening.
        UARTprintf("\nServo Task Creation Failed");
        return(1);
    }
    return(0);
}
