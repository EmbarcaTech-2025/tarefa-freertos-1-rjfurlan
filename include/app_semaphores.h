/**
 * @file    app_semaphores.h
 * @author  Ricardo Jorge Furlan
 * @brief   Initializes semaphores and makes them available
 * @version 0.1
 * @date    2025-06-13
 */
#ifndef APP_SEMAPHORES_H
#define APP_SEMAPHORES_H

#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t semaphore_handler_led;
extern SemaphoreHandle_t semaphore_handler_buzzer;

/**
 * @brief initializes semaphores
 * 
 */
void app_semaphores_init();


#endif // APP_SEMAPHORES_H