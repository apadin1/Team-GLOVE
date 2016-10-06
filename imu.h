/*
 * Filename:  IMU.h
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
 *  Top-level interface for a task to get data from the IMU
 *
 */

#ifndef IMU_H_
#define IMU_H_

#include "Adafruit_BNO055.h" // will need to port


/*
 * We will want some way to save the calibration for the IMU
 * and then reload it on setup
 */
void saveIMUCalibration();
void loadIMUCalibration();




#endif /* IMU_H_ */

