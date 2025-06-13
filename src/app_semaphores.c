/**
 * @file    app_semaphores.c
 * @author  Ricardo Jorge Furlan
 * @brief   Initializes semaphores and makes them available
 * @version 0.1
 * @date    2025-06-13
 */
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "../include/app_semaphores.h"


SemaphoreHandle_t semaphore_handler_led;
SemaphoreHandle_t semaphore_handler_buzzer;

/**
 * @brief initializes semaphores
 * 
 */
void app_semaphores_init(){
    // Criação dos semáforos binários
    semaphore_handler_led    = xSemaphoreCreateBinary();
    semaphore_handler_buzzer = xSemaphoreCreateBinary();
    xSemaphoreGive(semaphore_handler_led);
    xSemaphoreGive(semaphore_handler_buzzer);
}