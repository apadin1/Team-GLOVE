/*
 * Filename:  flex_sensor.c
 * Project:   EECS 473 - Team GLOVE
 * Date:      Fall 2016
 * Authors:
 *     Nick Bertoldi
 *     Ben Heckathorn
 *     Ryan O’Keefe
 *     Adrian Padin
 *     Tim Schumacher
 *
 * Purpose:
 *  Implementation for the FlexSensors class in flex_sensor.h
 *
 */
#include "flex_sensor.h"

/*
 * Constructor initializes the AnalogIn objects
 */
FlexSensors::FlexSensors() {
    sensors[0].ain = new AnalogIn((PinName)FLEX_0);
    sensors[1].ain = new AnalogIn((PinName)FLEX_1);
    sensors[2].ain = new AnalogIn((PinName)FLEX_2);
    sensors[3].ain = new AnalogIn((PinName)FLEX_3);
}

/*
 * Update the deflection for all flex sensors
 */
void FlexSensors::updateSensors() {
    for (uint8_t i = 0; i < FLEX_SENSORS_COUNT; i++) {
        sensors[i].deflection = sensors[i].ain->read_u16();
    }
}

/*
 * Write the flex sensor values to the given array.
 * This assumes no ownership or locking of the given container
 */
void FlexSensors::writeSensors(uint16_t* buf) {
    for (uint8_t i = 0; i < FLEX_SENSORS_COUNT; i++) {
        buf[i] = sensors[i].deflection;
    }
}

/*
 * Alternative interface to both update each pin
 * And write it to the destination buffer
 */
void FlexSensors::updateAndWriteSensors(uint16_t* buf) {
    for (uint8_t i = 0; i < FLEX_SENSORS_COUNT; i++) {
        sensors[i].deflection = sensors[i].ain->read_u16();
        buf[i] = sensors[i].deflection;
    }
}

void FlexSensors::print(Serial& pc) {
    pc.printf("%hu 0x%hx, %hu 0x%hx, %hu 0x%hx, %hu 0x%hx\r\n",
            sensors[0].deflection, sensors[0].deflection,
            sensors[1].deflection, sensors[1].deflection,
            sensors[2].deflection, sensors[2].deflection,
            sensors[3].deflection, sensors[3].deflection);
}

void FlexSensors::printSingle(Serial& pc, uint8_t index) {
    if (index < FLEX_SENSORS_COUNT) {
        pc.printf("Flex %hu: %hu 0x%hx\r\n", index,
                sensors[index].deflection, sensors[index].deflection);
    }
}
