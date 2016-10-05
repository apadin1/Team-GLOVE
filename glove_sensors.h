/*
 * Filename:  glove_sensors.h
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
 *  This is the definition of the struct used on either end
 *  of the bluetooth connection, and it contains synchronization things.
 *
 */

#ifndef GLOVE_SENSORS_H_
#define GLOVE_SENSORS_H_

#include <FreeRTOS.h>
#include <semphr.h>



class glove_sensors {
public:



private:
    xSemaphoreHandle data_mutex;

}


#endif /* GLOVE_SENSORS_H_ */

