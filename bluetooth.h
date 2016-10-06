/*
 * Filename:  bluetooth.h
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
 *  Top-level interface for basic bluetooth send and recieve
 *
 *  Will sit on top of extensive bluetooth stack that
 *  is built into the nifty nRF51 processor
 *
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <inttypes.h>

typedef struct {

} bluetooth_handle_t;

/*
 * The specific initialization details are unknown
 * but building this as a framework to be fleshed out
 */
bluetooth_handle_t* initBluetooth();

/*
 * Serializes a struct requires its length
 *
 * Return -1 on error
 */
int sendSerialized(bluetooth_handle_t*, void* data_struct, uint16_t size);

/*
 * Un-serializes a struct
 *
 * Return -1 on Error
 *
 * Maybe even have a checksum for funzies
 */
int recvSerialized(bluetooth_handle_t*, void* data_struct, uint16_t size);


#endif /* BLUETOOTH_H_ */

