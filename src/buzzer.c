/**
 * @file    buzzer.c
 * @author  Ricardo Jorge Furlan
 * @brief   Initializes BUZZER PWM and provides task functions, with and without the use of semaphores, as well as a handler variable for them, to be used by the RTOS.
 * @version 0.1
 * @date    2025-06-13
 */
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "../include/app_semaphores.h"
#include "../include/buzzer.h"
#include "../include/config.h"

bool buzzer_enable = true;
TaskHandle_t buzzer_task_handle;

/**
 * @brief Initialize the buzzer driver and test if applicable
 * 
 * @param test, if the test is true, after initializing the Buzzer it will make some beeps
 */
void buzzer_init(bool test){
    // Inicializa PWM e GPIO do Buzzer
    pwm_set_clkdiv_int_frac(  BUZZER_A_PWM_SLICE, BUZZER_A_PWM_DIV, 0);
    pwm_set_wrap(             BUZZER_A_PWM_SLICE, BUZZER_A_PWM_WRAP);
    pwm_set_chan_level(       BUZZER_A_PWM_SLICE, BUZZER_A_PWM_CHANNEL, 0);
    pwm_set_enabled(          BUZZER_A_PWM_SLICE, true);
    gpio_set_function(BUZZER_A_GPIO, GPIO_FUNC_PWM);

    // se test ativo faz 3 beeps
    if(test){
        for(int i=0;i<3;i++){
            pwm_set_chan_level(BUZZER_A_PWM_SLICE, BUZZER_A_PWM_CHANNEL, BUZZER_A_PWM_LEVEL);
            sleep_ms(BUZZER_A_BEEP_DURATION_MS);
            pwm_set_chan_level(BUZZER_A_PWM_SLICE, BUZZER_A_PWM_CHANNEL, 0);
            sleep_ms(BUZZER_A_BEEP_DELAY_TEST_MS);
        }
    }  
}

/**
 * @brief task function, without using semaphores
 * The Buzzer rings based on the variable buzzer_enable
 * 
 * @param arg, unused
 */
void buzzer_task(void * arg){
    while(true){
        // faz o buzzer tocar se enable
        if(buzzer_enable) pwm_set_chan_level(BUZZER_A_PWM_SLICE, BUZZER_A_PWM_CHANNEL, BUZZER_A_PWM_LEVEL);
        sleep_ms(BUZZER_A_BEEP_DURATION_MS);
        // para o buzzer
        pwm_set_chan_level(BUZZER_A_PWM_SLICE, BUZZER_A_PWM_CHANNEL, 0);
        vTaskDelay(pdMS_TO_TICKS(TASK_BUZZER_INTERVAL_MS - BUZZER_A_BEEP_DURATION_MS));
    }
}    

/**
 * @brief task function, using semaphores
 * can only activate the buzzer when can get the token from the traffic light
 * 
 * @param arg, unused
 */
void buzzer_task_with_semaphore(void * arg){
    while(true){
        // Só faz o buzzer tocar se conseguir pegar o token do semáforo, senão aguarda
        xSemaphoreTake(semaphore_handler_buzzer, portMAX_DELAY);
        pwm_set_chan_level(BUZZER_A_PWM_SLICE, BUZZER_A_PWM_CHANNEL, BUZZER_A_PWM_LEVEL);
        xSemaphoreGive(semaphore_handler_buzzer);

        sleep_ms(BUZZER_A_BEEP_DURATION_MS);
        pwm_set_chan_level(BUZZER_A_PWM_SLICE, BUZZER_A_PWM_CHANNEL, 0);
        vTaskDelay(pdMS_TO_TICKS(TASK_BUZZER_INTERVAL_MS - BUZZER_A_BEEP_DURATION_MS));
    }
}