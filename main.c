/**
 * @file    main.c
 * @author  Ricardo Jorge Furlan
 * @brief   Initializes devices and launches tashs on the RTOS
 * @version 0.1
 * @date    2025-06-13
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "include/app_semaphores.h"
#include "include/buttons.h"
#include "include/buzzer.h"
#include "include/config.h"
#include "include/led_rgb.h"
#include "src/oled/display.h"

/**
 * @brief Initializes devices and launches tasks on the RTOS
 * Note: you can use the buttons to choose between tasks that use semaphores or not.
 * 
 * @return int 
 */
int main()
{
    stdio_init_all();
    printf("embarcatech-freertos-tarefa-1\n");

#ifdef DEBUG_BITDOG_LAB
    // Configuração utilizada para configurar a UART da BidDogLab para Debug
    gpio_set_function( 0, GPIO_FUNC_SIO);
    gpio_set_function( 1, GPIO_FUNC_SIO);
    gpio_set_function(17, GPIO_FUNC_UART);
    gpio_set_function(16, GPIO_FUNC_UART);
#endif

    // Adotou-se inicializar os dispositivos fora do RTOS
    printf("Init all devices\n");
    display_init();
    display_msg(true, 3, 2, "RTOS TASK");
    buttons_init();
    buzzer_init( INIT_TEST_ENABLE);
    led_rgb_init(INIT_TEST_ENABLE);
    
    // Opção, fora do RTOS para escolher se a aplicação vai utilizar semáforo ou não
    bool semaphore_mode;

    display_fill(false, false);
    display_msg(false, 0, 0, "  MODE SELECT");
    display_msg(false, 0, 3, "BUTTON A: NORMAL");
    display_msg(false, 0, 6, "BUZZER B:");
    display_msg(true,  0, 7, "      SEMAPHORES");

    bool fim = false;
    while(!fim){
        if(!gpio_get(BUTTON_A_GPIO)){
            semaphore_mode = false;
            fim            = true;
            printf("Selected Normal mode\n");
        } else if(!gpio_get(BUTTON_B_GPIO)){
            semaphore_mode = true;
            fim            = true;
            printf("Selected Semaphore mode\n");
        } else sleep_ms(50)        ;
    }

    // Utilizou-se tasks distintas para com o sem o uso de semáforos
    if(semaphore_mode){
        printf("Create all tasks, Semaphores Mode\n");
        app_semaphores_init();
        xTaskCreate(led_rgb_task_with_semaphore, "LedRgbChangeWithSemaphore", 256, NULL, 1, &led_rgb_task_handle);
        xTaskCreate(buzzer_task_with_semaphore,  "BuzzerPlayWithSemaphore",   256, NULL, 1, &buzzer_task_handle);
        xTaskCreate(buttons_task_with_semaphore, "ButtonCmdWithSemaphore",    256, NULL, 1, &buttons_task_handle);
    }else{
        printf("Create all tasks, Standard Mode\n");
        xTaskCreate(led_rgb_task, "LedRgbChange", 256, NULL, 1, &led_rgb_task_handle);
        xTaskCreate(buzzer_task,  "BuzzerPlay",   256, NULL, 1, &buzzer_task_handle);
        xTaskCreate(buttons_task, "ButtonCmd",    256, NULL, 1, &buttons_task_handle);
    }

    printf("Start Scheduler\n");
    vTaskStartScheduler();

    printf("After Start Scheduler, never arrives this line\n");
    while (true) { }

    return 0;
}