/**
 * @file    led_rgb.h
 * @author  Ricardo Jorge Furlan
 * @brief   Initializes the LED GPIOs and provides task functions, with and without the use of semaphores, as well as a handler variable for them, to be used by the RTOS
 * @version 0.1
 * @date    2025-06-13
 */
#ifndef LED_RGB_H
#define LED_RGB_H

extern TaskHandle_t led_rgb_task_handle;

/**
 * @brief Initialize the LED driver and test if applicable
 * 
 * @param test, If the test is true, after initialization each of the LED elements will be turned on individually
 */
void led_rgb_init(bool test);

/**
 * @brief task function, without using semaphores
 * can only change LED color when tash are not suspended
 * @param arg, unused
 */
void led_rgb_task(void * arg );

/**
 * @brief task function, using semaphores
 * can only change LED color when can get the token from the traffic light
 * 
 * @param arg, unused
 */
void led_rgb_task_with_semaphore(void * arg);


#endif // LED_RGB_H