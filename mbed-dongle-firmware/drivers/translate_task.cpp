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
 *  Implementation of translatorTask
 */
#include "translate_task.h"
    
TranslateTask::TranslateTask(Translator* left_, Translator* right_,
                        KeyboardMouse* input_):
                        left(left_), right(right_), input(input_){
                            update_task_timer =
                                new RtosTimer(this, &TranslateTask::doTheThing, osTimerPeriodic);
                        }
                        
void  TranslateTask::doTheThing() {
    left->gestureCheck();
    //right->gestureCheck();
    input->sendMouse();
    input->sendKeyboard();
}
void TranslateTask::startUpdateTask(uint32_t ms) {
    update_task_timer->start(ms);
}
void TranslateTask::stopUpdateTask() {
    update_task_timer->stop();
}
    