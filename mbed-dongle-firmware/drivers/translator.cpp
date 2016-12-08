/*
 * Filename:  translator.cpp
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
 *  Implementation of translator.h
 */

#include "translator.h"
#include "gpio.h"

#define ACTIVE_LOW true
#define ACTIVE_HIGH false

/* DEBUG */
const PinName TRANSLATOR_DEBUG_PIN = p26;

Translator::Translator(glove_sensors_raw_t& _glove_data,
                       KeyboardMouse& _HIDinput)
    : glove_data(_glove_data), HIDinput(_HIDinput),
    working(TRANSLATOR_DEBUG_PIN) {

        /* Left Glove Setup */

        /* FLEX1 */
        flex_sensors[FLEX1] =
        new flexToHID(glove_data.flex_sensors, 300, 800, 0.10, ACTIVE_LOW);

        /* FLEX2 */
        flex_sensors[FLEX2] =
        new flexToHID(glove_data.flex_sensors+1, 400, 800, 0.25, ACTIVE_LOW);

        /* FLEX3 */
        flex_sensors[FLEX3] =
        new flexToHID(glove_data.flex_sensors+2, 370, 1000, 0.15, ACTIVE_LOW);

        /* FLEX4 */
        flex_sensors[FLEX4] =
        new flexToHID(glove_data.flex_sensors+3, 350, 930, 0.15, ACTIVE_LOW);

        /* TOUCH1 */
        touch_sensors[TOUCH1] =
        new touchToHID(&(glove_data.touch_sensor.a));

        /* TOUCH2 */
        touch_sensors[TOUCH2] =
        new touchToHID(&(glove_data.touch_sensor.b));

        /* TOUCH3 */
        touch_sensors[TOUCH3] =
        new touchToHID(&(glove_data.touch_sensor.c));

        /* TOUCH4 */
        touch_sensors[TOUCH4] =
        new touchToHID(&(glove_data.touch_sensor.d));

        /* PITCHUP */
        imu_axis[PITCHUP] =
        new imuToHID(&(glove_data.imu.orient_pitch), -50, 50, 0.15, ACTIVE_HIGH);

        /* PITCHDOWN */
        imu_axis[PITCHDOWN] =
        new imuToHID(&(glove_data.imu.orient_pitch), -35, 0, 0.15, ACTIVE_LOW);

        /* ROLLLEFT */
        imu_axis[ROLLLEFT] =
        new imuToHID(&(glove_data.imu.orient_roll), -50, 50, 0.15, ACTIVE_HIGH);

        /* ROLLRIGHT */
        imu_axis[ROLLRIGHT] =
        new imuToHID(&(glove_data.imu.orient_roll), -45, 0, 0.15, ACTIVE_LOW);

        /* BUTTON MAPPING */
        flex_sensors[FLEX1]->change_hid_profile(MOUSE, 0, LBUTTON);
        flex_sensors[FLEX2]->change_hid_profile(MOUSE, 0, RBUTTON);
        flex_sensors[FLEX3]->change_hid_profile(KEYBOARD, 'c');
        flex_sensors[FLEX4]->change_hid_profile(KEYBOARD, ' ');
        touch_sensors[TOUCH1]->change_hid_profile(KEYBOARD,'e');
        //touch_sensors[TOUCH2]->change_hid_profile(MOUSE, 0, RBUTTON);
        touch_sensors[TOUCH3]->change_hid_profile(KEYBOARD, 'g');
        touch_sensors[TOUCH4]->change_hid_profile(KEYBOARD, 'h');
        imu_axis[PITCHUP]->change_hid_profile(DISABLED, 0, YAXIS);
        imu_axis[PITCHDOWN]->change_hid_profile(DISABLED, 'j');
        imu_axis[ROLLLEFT]->change_hid_profile(MOUSE, 0, XAXIS);
        imu_axis[ROLLRIGHT]->change_hid_profile(DISABLED, 'l');

        //TO MAKE DEBUG EASIER - choose one line from top or below to be uncommented
        //    flex_sensors[FLEX1]->change_hid_profile(DISABLED);
            //flex_sensors[FLEX2]->change_hid_profile(DISABLED);
        //    flex_sensors[FLEX3]->change_hid_profile(DISABLED);
        //    flex_sensors[FLEX4]->change_hid_profile(DISABLED);
            //touch_sensors[TOUCH1]->change_hid_profile(DISABLED);
            touch_sensors[TOUCH2]->change_hid_profile(DISABLED);
        //    touch_sensors[TOUCH3]->change_hid_profile(DISABLED);
        //    touch_sensors[TOUCH4]->change_hid_profile(DISABLED);
        //   imu_axis[PITCHUP]->change_hid_profile(DISABLED);
        //    imu_axis[PITCHDOWN]->change_hid_profile(DISABLED);
        //    imu_axis[ROLLLEFT]->change_hid_profile(DISABLED);
        //    imu_axis[ROLLRIGHT]->change_hid_profile(DISABLED);


        //update_task_timer = new RtosTimer(this, &Translator::gestureCheck, osTimerPeriodic);
    }

void Translator::updateGestureMap(uint8_t* config) {

    /* Left Glove Configuration */

    /* Flex Sensor Configuration */
    for (int i = 0; i < FLEX_COUNT; ++i) {
        if (config[i] == 0) {
            flex_sensors[i]->change_hid_profile(DISABLED);
        }
        else if (config[i] < 200) {
            flex_sensors[i]->change_hid_profile(KEYBOARD, config[i]);
        }
        else {
            flex_sensors[i]->change_hid_profile(MOUSE, 0, static_cast<mousePart>(config[i]));
        }
    }

    /* Touch Sensor Configuration */
    for (int i = 4; i < TOUCH_COUNT + 4; ++i) {
        if (config[i] == 0) {
            touch_sensors[i-4]->change_hid_profile(DISABLED);
        }
        else if (config[i] < 200) {
            touch_sensors[i-4]->change_hid_profile(KEYBOARD, config[i]);
        }
        else {
            touch_sensors[i-4]->change_hid_profile(MOUSE, 0, static_cast<mousePart>(config[i]));
        }
    }

    /* IMU Configuration */
    for (int i = 8; i < IMU_COUNT + 8; ++i) {
        if (config[i] == 0) {
            imu_axis[i-8]->change_hid_profile(DISABLED);
        }
        else if (config[i] < 200) {
            imu_axis[i-8]->change_hid_profile(KEYBOARD, config[i]);
        }
        else {
            imu_axis[i-8]->change_hid_profile(MOUSE, 0, static_cast<mousePart>(config[i]));
        }
    }
}

float deadzone(float f, float T) {
    if (-T < f && f < T) {
        f = 0;
    }
    else if (f > 0) {
        f -= T;
    }
    else if (f < 0) {
        f += T;
    }
    return f;
}

void Translator::gestureCheck() {

    /* Glove Functionality */
    if (!HIDinput->isConnected()) {
        return;
    }

    led2 = 0;//DEBUG

    // Flex Sensor functionality
    for (int i = 0; i < FLEX_COUNT; ++i) {

        /* Keyboard functionality */
        if (flex_sensors[i]->is_keyboard()) {
            keyboardData keyboard = flex_sensors[i]->get_keyboard_data();
            handleKeyboardInput(keyboard);
        }

        /* Mouse functionality */
        else if (flex_sensors[i]->is_mouse()) {
            mouseData mouse = flex_sensors[i]->get_mouse_data();
            handleMouseInput(mouse);
        }
    }

    // Touch Sensor functionality
    for (int i = 0; i < TOUCH_COUNT; ++i) {

        // Keyboard functionality
        if (touch_sensors[i]->is_keyboard()) {
            keyboardData keyboard = touch_sensors[i]->get_keyboard_data();
            handleKeyboardInput(keyboard);
        }

        // Mouse functionality
        else if (touch_sensors[i]->is_mouse()) {
            mouseData mouse = touch_sensors[i]->get_mouse_data();
            handleMouseInput(mouse);
        }
    }

    // IMU functionality
    for (int i = 0; i < IMU_COUNT; i++) {

        // Keyboard functionality
        if (imu_axis[i]->is_keyboard()) {
            keyboardData keyboard = imu_axis[i]->get_keyboard_data();
            handleKeyboardInput(keyboard);
        }

        // Mouse functionality
        else if (imu_axis[i]->is_mouse()) {
            //mouseData mouse = imu_axis[i]->get_mouse_data();  // Grab mouse data
            //handleMouseInput(mouse);
        }
    }
    // balance the mouse TODO

        float r = -(glove_data->imu.orient_roll+20);
        float p = -glove_data->imu.orient_pitch;
        int T_r = 8;
        int T_p = 5;
        HIDinput->setMouseSpeedX(int8_t(deadzone(r, T_r)));
        HIDinput->setMouseSpeedY(int8_t(deadzone(p, T_p)));

    led2 = 1;
}

void Translator::handleKeyboardInput(keyboardData& keyboard) {
    if (keyboard.valid) {
        if (keyboard.value) {
            HIDinput->keyPress(keyboard.key);
        } else {
            HIDinput->keyRelease(keyboard.key);
        }
    }
}

void Translator::handleMouseInput(mouseData& mouse) {
    if (mouse.valid) {

        // Left click
        if (mouse.part == LBUTTON) {
            if (mouse.value) {
                HIDinput->setMouseButton(LEFT, DOWN);
            } else {
                HIDinput->setMouseButton(LEFT, UP);
            }
        }

        // Right click
        else if (mouse.part == RBUTTON) {
            if (mouse.value) {
                HIDinput->setMouseButton(RIGHT, DOWN);
            } else {
                HIDinput->setMouseButton(RIGHT, UP);
            }
        }

        // Middle click
        else if (mouse.part == MIDDLECLICK) {
            if (mouse.value) {
                HIDinput->setMouseButton(MIDDLE, DOWN);
            } else {
                HIDinput->setMouseButton(MIDDLE, UP);
            }
        }

        // Scroll functionality
        else if (mouse.part == SCROLLAXIS) {
            HIDinput->setMouseScroll(mouse.value);
        }

        // X-axis functionality
        else if (mouse.part == XAXIS) {
            HIDinput->setMouseSpeedX(mouse.value);
        }

        // Y-axis functionality
        else if (mouse.part == YAXIS) {
            HIDinput->setMouseSpeedY(mouse.value);
        }
    }  // if valid
}

void Translator::startUpdateTask(uint32_t ms) { update_task_timer->start(ms); }
void Translator::stopUpdateTask() { update_task_timer->stop(); }
