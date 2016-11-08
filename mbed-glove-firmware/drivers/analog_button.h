/*
 * Filename:  analog_button.h
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
 *
 *  Simple wrapper around an analog sensor value,
 *  to translate to a binary value on threshold.
 *  Also encapsulates some HID Functionality.
 *
 */
 
enum class hidType {
    KEYBOARD,
    MOUSE,
    JOYSTICK
}

enum class mousePart {
    LBUTTON,
    RBUTTON,
    SCROLL,
    XAXIS,
    YAXIS
}

struct keyboardData {
    bool changed,
    char key,
    bool digital_value,
}

struct mouseData {
    bool changed,
    mousePart part,
    bool digital_value,
    int8_t speed
}

template <class T>
class AnalogButton {
public:

    AnalogButton(T* data_, T min_, T max_, float transition_band, bool active_low_=1, hidType hid, char key)
        : data(data_), min_abs(min_), max_abs(max_), active_low(active_low_), HID(hid), key_input(key), click(NONE) {

        update_threshold(transition_band);
        calc_analog_state();
        calc_binary_state();
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
    void calc_binary_state() {

        // in the lowest range
        if (*data < min_thresh) {
            binary_state = active_low;
        }
        // in the transition band
        else if (*data < max_thresh) {
            //binary_state = binary_state;
        }
        // in the upper range
        else {
            binary_state = ! active_low;
        }
    }
    
    void calc_analog_state() { 
        uint8_t temp = 255 * (*data / range);
        analog_state = temp - 128
    }
    
    bool is_keyboard() {
        return HID == KEYBOARD;
    }
    bool is_mouse() {
        return HID == MOUSE;
    }
    bool is_joystick() {
        return HID == JOYSTICK
    }
    
private:
    bool binary_state;
    bool active_low;
    T* data;
    T min_abs, max_abs;
    T min_thresh, max_thresh;
    T range;
    hidType HID;
    char key_input;
    clickType click;
    int8_t analog_state;
};
