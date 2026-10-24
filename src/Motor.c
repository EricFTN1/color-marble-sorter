//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "clock.h"
#include "wait.h"
#include "tm4c123gh6pm.h"

// Bitband alias
//Pusher
#define IN1 (*((volatile uint32_t *)(0x42000000 + (0x400063FC-0x40000000)*32 + 4*4))) //PC4
#define IN2 (*((volatile uint32_t *)(0x42000000 + (0x400063FC-0x40000000)*32 + 5*4))) //PC5
#define IN3 (*((volatile uint32_t *)(0x42000000 + (0x400063FC-0x40000000)*32 + 6*4))) //PC6
#define IN4 (*((volatile uint32_t *)(0x42000000 + (0x400063FC-0x40000000)*32 + 7*4))) //PC7

//Port C masks
#define IN1_MASK 16
#define IN2_MASK 32
#define IN3_MASK 64
#define IN4_MASK 128

//Sorter
#define IN5 (*((volatile uint32_t *)(0x42000000 + (0x400073FC-0x40000000)*32 + 0*4))) //PD0
#define IN7 (*((volatile uint32_t *)(0x42000000 + (0x400073FC-0x40000000)*32 + 1*4))) //PD1
#define IN6 (*((volatile uint32_t *)(0x42000000 + (0x400073FC-0x40000000)*32 + 2*4))) //PD2
#define IN8 (*((volatile uint32_t *)(0x42000000 + (0x400073FC-0x40000000)*32 + 3*4))) //PD3

//Port D masks
#define IN5_MASK 1
#define IN7_MASK 2
#define IN6_MASK 4
#define IN8_MASK 8

// LED used for testing only
#define RED_LED     (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 1*4)))
#define BLUE_LED    (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 2*4)))
#define GREEN_LED   (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 3*4)))
#define SW1         (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 4*4)))

#define RED_LED_MASK 2      //PF1
#define BLUE_LED_MASK 4     //PF2
#define GREEN_LED_MASK 8    //PF3
#define SW1_MASK 16         //PF4

//Global Variables
uint32_t time = 3e3;
uint16_t steps = 2047;
bool pusherActive=0;
int currentStepSorter = 0;
int currentStepPusher = 0;
int targetSteps = 0;
int stepCounter = 0;

bool flagCCW = 1;   // Indicates what direction the stepper motor will rotate
uint8_t stepPusher = 2027;

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

// Initialize Hardware
void initHw()
{
    // Initialize system clock to 40 MHz
    initSystemClockTo40Mhz();

    // Enable clocks
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2 | SYSCTL_RCGCGPIO_R3 | SYSCTL_RCGCGPIO_R5;
    _delay_cycles(3);

    // Configure GPIO inputs/outputs
    GPIO_PORTF_DIR_R |= RED_LED_MASK | GREEN_LED_MASK | BLUE_LED_MASK;
    GPIO_PORTF_DIR_R &= ~SW1_MASK;

    // Enable the ports/pins
    GPIO_PORTF_DEN_R |= RED_LED_MASK | GREEN_LED_MASK | BLUE_LED_MASK | SW1_MASK;

    // Enable internal pull-up for SW1
    GPIO_PORTF_PUR_R |= SW1_MASK;

    // Configure Direction Pins
    GPIO_PORTC_DIR_R |= IN1_MASK | IN2_MASK | IN3_MASK | IN4_MASK;
    GPIO_PORTC_DEN_R |= IN1_MASK | IN2_MASK | IN3_MASK | IN4_MASK;

    GPIO_PORTD_DIR_R |= IN5_MASK | IN6_MASK | IN7_MASK | IN8_MASK;
    GPIO_PORTD_DEN_R |= IN5_MASK | IN6_MASK | IN7_MASK | IN8_MASK;
}

void initStepper()
{
    initSystemClockTo40Mhz();

    // Enable clocks
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2;
    _delay_cycles(3);

    // Configure Pins used by Stepper Motor for movement/rotation
    GPIO_PORTC_DIR_R |= IN1_MASK | IN2_MASK | IN3_MASK | IN4_MASK;
    GPIO_PORTC_DEN_R |= IN1_MASK | IN2_MASK | IN3_MASK | IN4_MASK;
}

void movePusher(uint8_t stepNum)
{
    switch (stepNum)
    {
        case (0):
            IN1 = 1;
            IN2 = 0;
            IN3 = 1;
            IN4 = 0;
            break;
        case (1):
            IN1 = 0;
            IN2 = 1;
            IN3 = 1;
            IN4 = 0;
            break;
        case (2):
            IN1 = 0;
            IN2 = 1;
            IN3 = 0;
            IN4 = 1;
            break;
        case (3):
            IN1 = 1;
            IN2 = 0;
            IN3 = 0;
            IN4 = 1;
            break;
        default:
            break;
    }
    waitMicrosecond(time);
}

void moveSorter(uint8_t stepNum)
{
    switch (stepNum)
    {
        case (0):
            IN5 = 1;
            IN6 = 0;
            IN7 = 1;
            IN8 = 0;
            break;
        case (1):
            IN5 = 0;
            IN6 = 1;
            IN7 = 1;
            IN8 = 0;
            break;
        case (2):
            IN5 = 0;
            IN6 = 1;
            IN7 = 0;
            IN8 = 1;
            break;
        case (3):
            IN5 = 1;
            IN6 = 0;
            IN7 = 0;
            IN8 = 1;
            break;
        default:
            break;
    }
    waitMicrosecond(time);
}

void calibrate(void)
{
    while(steps > 0)
    {
        // The following 2 lines are required to rotate the stepper motor clock wise
        moveSorter(steps % 4);
        steps--;
    }
    stepCounter=0;
}

void pushMarble(void)
{
    if (stepPusher > 0) // Check if steps are remaining for a full rotation
    {
        movePusher(stepPusher); // Move pusher motor one step
        steps--;
    }
    else
    {
        pusherActive = 0;  // Reset pusherActive flag when done
    }
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

// Used for 28BYJ-48 Stepper Motor ran at 5V
// Notes: 2048 Steps = 1 Full Rotation = 360 Degree Rotation

int main(void)
{
    initHw();           // Initializes hardware, not important to stepper functionality
    initStepper();      // Initializes GPIO pins used for 28BYJ-48 Stepper Motor

    calibrate();
    while(true)
    {
        if (!SW1 && !RED_LED && !BLUE_LED && !GREEN_LED)
        {
            RED_LED = 1;
            BLUE_LED = 0;
            GREEN_LED = 0;
            waitMicrosecond(1000000);

        }
        else if (!SW1 && RED_LED)
        {
            RED_LED = 0;
            BLUE_LED = 1;
            GREEN_LED = 0;
            waitMicrosecond(1000000);

        }
        else if(!SW1 && BLUE_LED)
        {
            RED_LED = 0;
            BLUE_LED = 0;
            GREEN_LED = 1;
            waitMicrosecond(1000000);

        }
        else if(!SW1 && GREEN_LED)
        {
            RED_LED = 1;
            BLUE_LED = 0;
            GREEN_LED = 0;
            waitMicrosecond(1000000);
        }

        // Set target steps based on LED color
        if (RED_LED)
        {
            targetSteps = 256; // Target steps for RED LED
        }

        else if (BLUE_LED)
        {
            targetSteps = 512; // Target steps for BLUE LED
        }

        else if (GREEN_LED)
        {
            targetSteps = 768; // Target steps for GREEN LED
        }

        if (targetSteps < stepCounter )
        {
            moveSorter(stepCounter % 4); // Move one step forward
            stepCounter--;
        }

        else if (targetSteps > stepCounter )
        {
            moveSorter(stepCounter % 4); // Move one step forward
            stepCounter++;
        }

        else if ( (stepCounter == targetSteps) && !pusherActive && (targetSteps != 0))
        {
            // Activate the pusher when sorter completes
            stepPusher = 2048;   // Set pusher steps for one full rotation
            pusherActive = 1; // Indicate pusher is active
            targetSteps = 0;
        }

        // Move pusher motor if it is active
        if (pusherActive)
        {
            waitMicrosecond(time); // Delay for motor timing
            pushMarble(); // Move pusher motor one step
            waitMicrosecond(time); // Delay for motor timing
        }
    }
}
