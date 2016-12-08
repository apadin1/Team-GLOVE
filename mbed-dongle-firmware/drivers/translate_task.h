#ifndef TRANSLATE_TASK_H_
#define TRANSLATE_TASK_H_

#include "translator.h"
#include "gpio.h"
#include "keyboard_mouse.h"
#include "mbed.h"

class TranslateTask {
public:
    TranslateTask(Translator* left_, Translator* right_, KeyboardMouse* input_);
    void doTheThing();
    void startUpdateTask(uint32_t ms);
    void stopUpdateTask();

private:
    RtosTimer* update_task_timer;
    Translator* left;
    Translator* right;
    KeyboardMouse* input;
};


#endif
