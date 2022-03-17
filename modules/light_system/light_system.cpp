//=====[Libraries]=============================================================

#include "arm_book_lib.h"

#include "light_system.h"
#include "bright_control.h"
#include "light_level_control.h"
#include "ldr_sensor.h"
#include "pc_serial_com.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration and initialization of private global objects]===============

//=====[Declaration of external public global variables]=======================

static float lightSystemLoopGain = 0.01;

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static float dutyCycle = 0.5f;
static float brightnessRGBLedRedFactor = 0.5f;
static float brightnessRGBLedGreenFactor = 0.5f;
static float brightnessRGBLedBlueFactor = 0.5f;

//=====[Implementations of public functions]===================================

void lightSystemInit()
{
    brightControlInit();
}

void lightSystemUpdate()
{
    static int i = 0;
    char str[10];
    
    dutyCycle = dutyCycle + lightSystemLoopGain
                            * (lightLevelControlRead() - ldrSensorRead());

    setDutyCycle( RGB_LED_RED, brightnessRGBLedRedFactor*dutyCycle );
    setDutyCycle( RGB_LED_GREEN, brightnessRGBLedGreenFactor*dutyCycle );
    setDutyCycle( RGB_LED_BLUE, brightnessRGBLedBlueFactor*dutyCycle );

    if (i > 100) {
        i=0;

        pcSerialComStringWrite("SP: ");
        sprintf( str, "%0.4f", lightLevelControlRead() );
        pcSerialComStringWrite( str );
        pcSerialComStringWrite(" | ");
        pcSerialComStringWrite("LDR: ");
        sprintf( str, "%0.4f", ldrSensorRead() );
        pcSerialComStringWrite( str );
        pcSerialComStringWrite(" | ");
        pcSerialComStringWrite("Duty: ");
        sprintf( str, "%0.4f", dutyCycle );
        pcSerialComStringWrite( str ); 
        pcSerialComStringWrite(" | ");
        pcSerialComStringWrite("Added: ");
        sprintf( str, "%0.4f", lightSystemLoopGain 
                                * (lightLevelControlRead() - ldrSensorRead()) );
        pcSerialComStringWrite( str );
        pcSerialComStringWrite("\r\n");
    }
    i++;
}

void lightSystemBrightnessChangeRGBFactor( lightSystem_t light, bool state )
{
    switch( light ) {
        case RGB_LED_RED:
            if ( state ) brightnessRGBLedRedFactor+=0.1;
                else brightnessRGBLedRedFactor-=0.1;
            if ( brightnessRGBLedRedFactor > 1) brightnessRGBLedRedFactor=1.0;
            if ( brightnessRGBLedRedFactor < 0) brightnessRGBLedRedFactor=0.0; 
        break;
        case RGB_LED_GREEN:
            if ( state ) brightnessRGBLedGreenFactor+=0.1;
                else brightnessRGBLedGreenFactor-=0.1;  
            if ( brightnessRGBLedGreenFactor > 1) brightnessRGBLedGreenFactor=1.0;
            if ( brightnessRGBLedGreenFactor < 0) brightnessRGBLedGreenFactor=0.0;      
        break;
        case RGB_LED_BLUE:
            if ( state ) brightnessRGBLedBlueFactor+=0.1;
                else brightnessRGBLedBlueFactor-=0.1;   
            if ( brightnessRGBLedBlueFactor > 1) brightnessRGBLedBlueFactor=1.0;
            if ( brightnessRGBLedBlueFactor < 0) brightnessRGBLedBlueFactor=0.0;
        break;
        default:
        break;
    }
}

//=====[Implementations of private functions]==================================
