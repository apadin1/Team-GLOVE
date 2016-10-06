/*
 * Filename:  gpio.h
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
 *  These are the functions which will be used to
 *  read and write to GPIO pins
 *
 *  Very vague for now, highly platform dependent
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <inttypes.h>

void initGPIO(uint8_t pin_num, uint8_t in_out);
void readGPIO(uint8_t pin_num);
void writeGPIO(uint8_t pin_num);

#endif /* GPIO_H_*/

