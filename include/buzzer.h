/**
 * @file    buzzer.h
 * @author  Ricardo Jorge Furlan
 * @brief   Initializes BUZZER PWM and provides task functions, with and without the use of semaphores, as well as a handler variable for them, to be used by the RTOS.
 * @version 0.1
 * @date    2025-06-13
 */
#ifndef BUZZER_H
#define BUZZER_H

extern bool         buzzer_enable;
extern TaskHandle_t buzzer_task_handle;

/**
 * @brief Initialize the buzzer driver and test if applicable
 * 
 * @param test, if the test is true, after initializing the Buzzer it will make some beeps
 */
void buzzer_init(bool test);

/**
 * @brief task function, without using semaphores
 * The Buzzer rings based on the variable buzzer_enable
 * 
 * @param arg, unused
 */
void buzzer_task(void * arg);

/**
 * @brief task function, using semaphores
 * can only activate the buzzer when can get the token from the traffic light
 * 
 * @param arg, unused
 */
void buzzer_task_with_semaphore(void * arg);

#endif // BUZZER_H