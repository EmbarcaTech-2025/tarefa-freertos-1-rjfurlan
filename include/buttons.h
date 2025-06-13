/**
 * @file    buttons.h
 * @author  Ricardo Jorge Furlan
 * @brief   Initializes the button GPIOs and provides task functions, with and without the use of semaphores, as well as a handler variable for them, to be used by the RTOS
 * It also updates the OLED display with information whether each of the tasks can be performed or not.
 * @version 0.1
 * @date    2025-06-13
 */
#ifndef BUTTONS_H
#define BUTTONS_H

extern TaskHandle_t buttons_task_handle;

/**
 * @brief Initializes the button GPIOs
 * 
 */
void buttons_init();

/**
 * @brief  task function, without using semaphores
 * Based on button A, the LED task is suspended or not.
 * Based on button B, enable or disable the buzzer. This approach was used because suspending the buzzer task could cause it to ring continuously while the button was pressed.
 * Updates the OLED display with information on whether the LED TASK is suspended or not, as well as whether the buzzer is enabled or not.
 * @param arg, unused
 */
void buttons_task(void * arg);

/**
 * @brief  task function, using semaphores
 * Based on button A, access to or release of the LED is done through a semaphore
 * Based on button B, access to or release of the BUZZER is done through a semaphore
 * Updates the OLED display with information whether each task can access the resource or not.
 * @param arg, unused
 */
void buttons_task_with_semaphore(void * arg);

#endif // BUTTONS_H