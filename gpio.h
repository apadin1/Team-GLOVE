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
 */

#ifndef GPIO_H_
#define GPIO_H_

void initGPIO(int pin_num, int in_out);
void readGPIO(int pin_num);
void writeGPIO(int pin_num);

#endif /* GPIO_H_*/

