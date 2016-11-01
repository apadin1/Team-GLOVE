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

#include "glove_data_comms.h"
#include "crc.h"

GloveDataComms::GloveDataComms() {
    crcInit();
}

int GloveDataComms::sendSerialized(glove_data_raw_t* data) {

}

int GloveDataComms::recvSerialized(glove_data_raw_t* data) {

}

