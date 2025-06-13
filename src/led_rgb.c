/**
 * @file    led_rgb.h
 * @author  Ricardo Jorge Furlan
 * @brief   Initializes the LED GPIOs and provides task functions, with and without the use of semaphores, as well as a handler variable for them, to be used by the RTOS
 * @version 0.1
 * @date    2025-06-13
 */
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "../include/app_semaphores.h"
#include "../include/config.h"
#include "../include/led_rgb.h"


TaskHandle_t led_rgb_task_handle;

static void led_init(uint gpio){
    gpio_put(         gpio, false);
    gpio_set_dir(     gpio, GPIO_OUT);
    gpio_set_function(gpio, GPIO_FUNC_SIO);
}

static void led_test(uint gpio){
    gpio_put(         gpio, true);
    sleep_ms(LED_TEST_TIME_MS);
    gpio_put(         gpio, false);
}

/**
 * @brief Initialize the LED driver and test if applicable
 * 
 * @param test, If the test is true, after initialization each of the LED elements will be turned on individually
 */
void led_rgb_init(bool test){
    led_init(LED_R_GPIO);
    led_init(LED_G_GPIO);
    led_init(LED_B_GPIO);

    if(test){
        led_test(LED_R_GPIO);
        led_test(LED_G_GPIO);
        led_test(LED_B_GPIO);
    }
}

/**
 * @brief task function, without using semaphores
 * can only change LED color when tash are not suspended
 * @param arg, unused
 */
void led_rgb_task(void * arg){
    uint8_t led_on = 0;

    while(true){
        gpio_put(LED_R_GPIO, led_on == 0);
        gpio_put(LED_G_GPIO, led_on == 1);
        gpio_put(LED_B_GPIO, led_on == 2);
        vTaskDelay(pdMS_TO_TICKS(TASK_LED_UPDATE_MS));

        if(++led_on >= 3){
            led_on = false;
        }
    }
}

/**
 * @brief task function, using semaphores
 * can only change LED color when can get the token from the traffic light
 * 
 * @param arg, unused
 */
void led_rgb_task_with_semaphore(void * arg){
    uint8_t led_on = 0;

    while(true){
        xSemaphoreTake(semaphore_handler_led, portMAX_DELAY);
        gpio_put(LED_R_GPIO, led_on == 0);
        gpio_put(LED_G_GPIO, led_on == 1);
        gpio_put(LED_B_GPIO, led_on == 2);
        xSemaphoreGive(semaphore_handler_led);

        vTaskDelay(pdMS_TO_TICKS(TASK_LED_UPDATE_MS));
        if(++led_on >= 3){
            led_on = false;
        }
    }
}