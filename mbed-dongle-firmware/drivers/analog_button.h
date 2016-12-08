/*
 * Filename:  analog_button.h
 * Project:   EECS 473 - Team GLOVE
 * Date:      Fall 2016 * Authors: *     Nick Bertoldi
 *     Ben Heckathorn
 *     Ryan O’Keefe
 *     Adrian Padin
 *     Tim Schumacher
 *
 * Purpose:
 *
 *  Simple wrapper around an analog sensor value,
 *  to translate to a binary value on threshold.
 *  Also encapsulates some HID Functionality.
 *
 */
#ifndef ANALOG_BUTTON_H_
#define ANALOG_BUTTON_H_
#include <stdint.h>


template <class T>
class AnalogButton {
public:
    AnalogButton() {
        //TODO: this IS the shit
    }
    AnalogButton(T* data_, T min_, T max_, float transition_band, bool active_low_)
        : data(data_), min_abs(min_), max_abs(max_), active_low(active_low_) {

        update_threshold(transition_band);
    }

    void init(T* data_, T min_, T max_, float transition_band, bool active_low_) {
        data = data_;
        min_abs = min_;
        max_abs = max_;
        active_low = active_low_;
        update_threshold(transition_band);
    }

    void update_threshold(float transition_band) {
        if (transition_band < 0.0 || 1.0 < transition_band) {
            // fail
            return;
        }

        range = max_abs - min_abs;
        float valid_band = range * ((1-transition_band) / 2.0);

        min_thresh = min_abs + valid_band;
        max_thresh = max_abs - valid_band;
    }

    void update_bounds(T min_, T max_, float transition_band) {
        min_abs = min_;
        max_abs = max_;
        update_threshold(transition_band);
    }

    /*
     * This is the thing that does the threshold arithmatic
     *
     * Counts on the data changing spurriously
     *
     * Active low for now cuz sure
     */
    bool get_binary_state() {
        DigitalOut l3(LED3);

        // in the lowest range
        if (*data < min_thresh) {
            l3 = 1;
            binary_state = active_low;
        }
        // in the transition band
        else if (*data < max_thresh) {
            binary_state = binary_state;
        }
        // in the upper range
        else {
            binary_state = ! active_low;
            l3 = 0;
        }

        return binary_state;
    }

    uint8_t imu_analog_state() {
        T value = (*data < max_abs) ? *data : max_abs;
        value = (*data > min_abs) ? *data : min_abs;
        int8_t tmp = 0;
        //int mouse_range = 10;
        /*
        double in_min = -50.0;
        double in_max = 50.0;
        double out_min = -50;
        double out_max = 50;
        */
        //tmp = (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        tmp = int(value);

        //tmp = 10 * (value / range); // TODO this scalar
        return tmp;
    }

    T* data;
    T range;
private:
    bool binary_state;
    bool active_low;
    T min_abs, max_abs;
    T min_thresh, max_thresh;
};

enum hidType {
    KEYBOARD,
    MOUSE,
    JOYSTICK,
    DISABLED
};

enum mousePart {
    LBUTTON = 200,
    RBUTTON,
    MIDDLECLICK,
    SCROLLAXIS,
    XAXIS,
    YAXIS,
    NONE
};

struct keyboardData {
    bool valid;
    bool value;
    char key;
    bool operator==(const keyboardData& a) { return (value == a.value); }
};

struct mouseData {
    bool valid;
    mousePart part;
    int8_t value;
    bool operator==(const mouseData& a) { return (value == a.value); }
};

class flexToHID {
public:
    flexToHID () {}
    void init( uint16_t* data_, uint16_t min_, uint16_t max_, float transition_band_, bool active_low_=true) {

        active_low = active_low_;
        sensor_conversion.init(data_, min_, max_, transition_band_, active_low_);
    }

    keyboardData get_keyboard_data() {
        cur_keyboard.value = sensor_conversion.get_binary_state();
        return cur_keyboard;
    }

    mouseData get_mouse_data() {
        cur_mouse.valid = true;
        if (cur_mouse.part == LBUTTON || cur_mouse.part == RBUTTON)
            cur_mouse.value = sensor_conversion.get_binary_state();
        else {
            cur_mouse.valid = false;
        }
        return cur_mouse;
    }

    bool is_keyboard() { return HID == KEYBOARD; }
    bool is_mouse() { return HID == MOUSE; }
    bool is_joystick() { return HID == JOYSTICK; }

    // call this function to change the hid status of the
    void change_hid_profile(hidType hid, char key_=0,
                            mousePart part_ = NONE) {
        HID = hid;
        if (HID == KEYBOARD) {
            cur_keyboard.valid = true;
            cur_keyboard.key = key_;
        } else if (HID == MOUSE) {
            cur_mouse.valid = true;
            cur_mouse.part = part_;
        }
    }

private:
    bool active_low;
    AnalogButton<uint16_t> sensor_conversion;
    hidType HID;
    keyboardData cur_keyboard;
    mouseData cur_mouse;

};

class imuToHID {
public:
    imuToHID() {}
    void init( float* data_, float min_, float max_, float transition_band_, bool active_low_=false) {
        active_low = active_low_;
        sensor_conversion = AnalogButton<float>(data_, min_, max_, transition_band_, active_low_);
    }

    keyboardData get_keyboard_data() {
        cur_keyboard.value = sensor_conversion.get_binary_state();
        return cur_keyboard;
    }

    mouseData get_mouse_data() {
        cur_mouse.valid = true;
        if (cur_mouse.part == LBUTTON || cur_mouse.part == RBUTTON)
            cur_mouse.value = sensor_conversion.get_binary_state();
        else {
            cur_mouse.value = sensor_conversion.imu_analog_state();
        }
        return cur_mouse;
    }

    bool is_keyboard() { return HID == KEYBOARD; }
    bool is_mouse() { return HID == MOUSE; }
    bool is_joystick() { return HID == JOYSTICK; }

    // call this function to change the hid status of the
    void change_hid_profile(hidType hid, char key_=0,
                            mousePart part_ = NONE) {
        HID = hid;
        if (HID == KEYBOARD) {
            cur_keyboard.valid = true;
            cur_keyboard.key = key_;
        } else if (HID == MOUSE) {
            cur_mouse.valid = true;
            cur_mouse.part = part_;
        }
    }

private:
    bool active_low;
    AnalogButton <float> sensor_conversion;
    hidType HID;
    keyboardData cur_keyboard;
    mouseData cur_mouse;
};

class touchToHID {
public:
  touchToHID() {}
  void init(uint8_t* data_) {
      data = data_;
  }

  // these are functions to check the HID status of the sensor
  bool is_keyboard() { return HID == KEYBOARD; }
  bool is_mouse() { return HID == MOUSE; }
  bool is_joystick() { return HID == JOYSTICK; }

  // call this function to change the hid status of the
  void change_hid_profile(hidType hid, char key_=0,
                          mousePart part_ = NONE) {
      HID = hid;
      if (HID == KEYBOARD) {
         cur_keyboard.valid = true;
         cur_keyboard.key = key_;
      } else if (HID == MOUSE) {
         cur_mouse.valid = true;
         cur_mouse.part = part_;
      } else {
         cur_keyboard.valid = false;
         cur_mouse.valid = false;
      }
  }

  keyboardData get_keyboard_data() {
    cur_keyboard.value = *data;
    return cur_keyboard;
  }

  mouseData get_mouse_data() {
    cur_mouse.value = *data;
    return cur_mouse;
  }

private:
  uint8_t* data;

  hidType HID;
  keyboardData cur_keyboard;
  mouseData cur_mouse;
};

#endif /*ANALOG_BUTTON_H_*/
