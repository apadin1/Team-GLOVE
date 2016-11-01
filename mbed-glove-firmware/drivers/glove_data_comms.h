/*
 * Filename:  glove_data_comms.h
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
 *  Top-level interface for send & recv the glove sensor data
 *
 *  Will sit on top the BLE UART libraries
 *  Implements CRC & header for the serialized struct
 *
 */

#ifndef GLOVE_DATA_COMMS_H_
#define GLOVE_DATA_COMMS_H_

#include <inttypes.h>

#include "glove_sensors.h"

class GloveDataComms {
public:

    /*
     * Serializes a struct requires its length
     *
     * Return -1 on error
     */
    int sendSerialized(glove_data_raw_t* data);

    /*
     * Un-serializes a struct
     *
     * Return -1 on Error
     *
     * Maybe even have a checksum for funzies
     */
    int recvSerialized(glove_data_raw_t* data);

private:
    BLE ble_;
    crc_t crc_;

};

#endif /* GLOVE_DATA_COMMS_H_ */

