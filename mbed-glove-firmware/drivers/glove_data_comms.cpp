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

// Start signal is a 0, flex sensors don't go to 0
const uint8_t GDC_START = 0x00;

// Size of start byte + glove data + CRC
const uint16_t GDC_TXRX_BUF_LEN  = 1 + sizeof(glove_sensors_raw_t) + sizeof(crc_t);


GloveDataComms::GloveDataComms() {
    crcInit();
}

int GloveDataComms::sendSerialized(glove_sensors_raw_t* data) {

    crc_ = crcFast((unsigned char const*)data, sizeof(glove_sensors_raw_t));

    ble_.write((char*)&GDC_START, 1);
    ble_.write((char*)data, sizeof(glove_sensors_raw_t));
    ble_.write((char*)&crc_, sizeof(crc_t));
}

int GloveDataComms::recvSerialized(glove_sensors_raw_t* data) {

}

