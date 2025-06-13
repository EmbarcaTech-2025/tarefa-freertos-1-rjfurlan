/**
 * @file    buttons.c
 * @author  Ricardo Jorge Furlan
 * @brief   Initializes the button GPIOs and provides task functions, with and without the use of semaphores, as well as a handler variable for them, to be used by the RTOS
 * It also updates the OLED display with information whether each of the tasks can be performed or not.
 * @version 0.1
 * @date    2025-06-13
 */
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "../include/app_semaphores.h"
#include "../include/buzzer.h"
#include "../include/buttons.h"
#include "../include/config.h"
#include "../include/led_rgb.h"
#include "../src/oled/display.h"

TaskHandle_t buttons_task_handle;

/**
 * @brief Initializes the button GPIOs
 * 
 */
void buttons_init(){
    gpio_init(   BUTTON_A_GPIO);
    gpio_init(   BUTTON_B_GPIO);
    gpio_pull_up(BUTTON_A_GPIO);
    gpio_pull_up(BUTTON_B_GPIO);
}


/**
 * @brief  task function, without using semaphores
 * Based on button A, the LED task is suspended or not.
 * Based on button B, enable or disable the buzzer. This approach was used because suspending the buzzer task could cause it to ring continuously while the button was pressed.
 * Updates the OLED display with information on whether the LED TASK is suspended or not, as well as whether the buzzer is enabled or not.
 * @param arg, unused
 */
void buttons_task(void * arg){
    bool led_task_suspended = false;
    while(true){
        if(!gpio_get(BUTTON_A_GPIO)){
            if(!led_task_suspended){
                vTaskSuspend(led_rgb_task_handle);
                led_task_suspended = true;
            }
        }else{
            if(led_task_suspended){
                vTaskResume(led_rgb_task_handle);
                led_task_suspended = false;
            }
        }
        if(!gpio_get(BUTTON_B_GPIO)){
            if(buzzer_enable){
                buzzer_enable = false;
            }
        }else{
            if(!buzzer_enable){
                buzzer_enable = true;
            }
        }
        display_fill(false, false);
        display_msg(false, 0, 0, "  NORMAL MODE");
        display_msg(false, 0, 3, led_task_suspended?"LED    SUSPENDED":"LED      RUNNING");
        display_msg(true,  0, 5, buzzer_enable?     "BUZZER    ENABLE":"BUZZER   DISABLE");
        vTaskDelay(pdMS_TO_TICKS(TASK_BUTTONS_POLLING_MS));
    }
}

/**
 * @brief  task function, using semaphores
 * Based on button A, access to or release of the LED is done through a semaphore
 * Based on button B, access to or release of the BUZZER is done through a semaphore
 * Updates the OLED display with information whether each task can access the resource or not.
 * @param arg, unused
 */
void buttons_task_with_semaphore(void * arg){
    bool led_task_blocked = false;
    bool buzzer_blocked   = false;
    while(true){
        if(!gpio_get(BUTTON_A_GPIO)){
            if(!led_task_blocked){
                xSemaphoreTake(semaphore_handler_led, portMAX_DELAY);
                led_task_blocked = true;
            }
        }else{
            if(led_task_blocked){
                xSemaphoreGive(semaphore_handler_led);
                led_task_blocked = false;
            }
        }

        if(!gpio_get(BUTTON_B_GPIO)){
            if(!buzzer_blocked){
                xSemaphoreTake(semaphore_handler_buzzer, portMAX_DELAY);
                buzzer_blocked = true;
            }
        }else{
            if(buzzer_blocked){
                xSemaphoreGive(semaphore_handler_buzzer);
                buzzer_blocked = false;
            }
        }
        display_fill(false, false);
        
        display_msg(false, 0, 0, " SEMAPHORE MODE");
        display_msg(false, 0, 3, led_task_blocked?"LED      BLOCKED":"LED         PLAY");
        display_msg(true,  0, 5, buzzer_blocked?  "BUZZER   BLOCKED":"BUZZER      PLAY");
        vTaskDelay(pdMS_TO_TICKS(TASK_BUTTONS_POLLING_MS));
    }
}