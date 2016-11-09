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
 #include <functional>
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
    bool valid,
    bool value,
    char key
    bool operator==(const keyboardData& a) {
        return (digital_value == a.digital_value);
    }
}

struct mouseData {
    bool changed,
    bool valid,
    mousePart part,
    int8_t value
    bool operator==(const mouseData& a){
        return (value == a.value);
    }
}

template <class T>
class AnalogButton {
public:
    //Use this cTor for analog input that may be mapped to analog or digital HID output
    AnalogButton(T* data_, T min_, T max_, float transition_band, bool active_low_=0, hidType hid=NULL, char key_=NULL, mousePart part_=NULL)
        : data(data_), min_abs(min_), max_abs(max_), active_low(active_low_), HID(hid), is_analog(true) {
        update_threshold(transition_band);
        cur_keyboard = keyboardData();
        cur_mouse = mouseData();
        change_hid_profile(hid, true);
        
    }
    //Use this cTor for pure digital buttons
    AnalogButton(T* data_, bool active_low_=0, hidType hid=NULL, char key=NULL, mousePart part=NULL)
        :data(data_), active_low(active_low_), HID(hid), max_abs(0), is_analog(false) {
        cur_keyboard = keyboardData();
        update_value = digital_read();
        change_hid_profile(hid, false);
        }
    //these are functions to check the HID status of the sensor
    bool is_keyboard() {
        return HID == KEYBOARD;
    }
    bool is_mouse() {
        return HID == MOUSE;
    }
    bool is_joystick() {
        return HID == JOYSTICK;
    }
    //call this function to change the hid status of the
    void change_hid_profile(hidType hid, char key_ = NULL, mousePart part_=NULL) {
        HID = hid;
        if (is_analog){
            if (HID == KEYBOARD){
                update_value = analog_to_digital_read;
                cur_keyboard.changed = true;
                cur_keyboard.valid = true;
                cur_keyboard.key = key_;
            }
            else if (HID == MOUSE){
                cur_mouse.changed = true;
                cur_mouse.valid = true;
                cur_mouse.part = part_;
                if (part_ == LBUTTON || part_ == RBUTTON)
                    update_value = analog_to_digital_read;
                else
                    update_value = analog_read;
            }
        }
        else {
            if (HID == KEYBOARD){
                update_value = digital_read;
                cur_keyboard.changed = true;
                cur_keyboard.valid = true;
                cur_keyboard.key = key_;
            }
            else if (HID == MOUSE){
                cur_mouse.changed = true;
                cur_mouse.valid = true;
                cur_mouse.part = part_;
                update_value = digital_read;
            }
        }
        update_value();
    }
    keyboardData get_keyboard_data () {
        keyboardData prev = cur_keyboard;
        prev.valid = false;
        if (HID != KEYBOARD) return prev;
        update_value();
        if (cur_keyboard == prev) cur_keyboard.changed = false;
        else cur_keyboard.changed = true;
        return cur_keyboard;
    }
    
    mouseData get_mouse_data () {
        mouseData prev = cur_mouse;
        prev.valid = false;
        if (HID != MOUSE) return prev;
        update_value();
        if (cur_mouse == prev) cur_mouse.changed = false;
        else cur_mouse.changed = true;
        return cur_mouse;
    }
    
private:
    std::function<void()> update_value;
    bool is_analog;
    bool active_low;
    T* data;
    T min_abs, max_abs;
    T min_thresh, max_thresh;
    T range;
    
    hidType HID;
    keyboardData cur_keyboard;
    mouseData cur_mouse;
    
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
    void analog_to_digital_read() {

        // in the lowest range
        if (*data < min_thresh) {
            if (HID == KEYBOARD) cur_keyboard.value = active_low;
            else if (HID == MOUSE) cur_mouse.value = active_low;
        }
        // in the transition band
        else if (*data < max_thresh) {
            //binary_state = binary_state;
        }
        // in the upper range
        else {
            if (HID == KEYBOARD) cur_keyboard.value = !active_low;
            else if (HID == MOUSE) cur_mouse.value = !active_low;
        }
    }
    
    void analog_read() { 
        uint8_t temp = 255 * (*data / range);
        temp = temp - 128;
        if (temp > 10 || temp < -10)
            cur_mouse.value = temp;
        else 
            cur_mouse.value = 0;
        
    }
    
    void digital_read() {
        if (*data > 0) cur_keyboard.value = 1;
        else cur_keyboard.value = 0;
    }
};
