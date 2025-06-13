/**
 * @file    config.h
 * @author  Ricardo Jorge Furlan
 * @brief   Parameters that will be used by the software
 * @version 0.1
 * @date    2025-06-13
 */
#ifndef CONFIG_H
#define CONFIG_H


// #define DEBUG_BITDOG_LAB
#define INIT_TEST_ENABLE            true

#define TASK_LED_UPDATE_MS           500
#define TASK_BUZZER_INTERVAL_MS     1000
#define TASK_BUTTONS_POLLING_MS      100

#define BUZZER_A_BEEP_DURATION_MS    100
#define BUZZER_A_BEEP_DELAY_TEST_MS  500
#define BUZZER_A_GPIO                 21    // PWM 2B
#define BUZZER_A_PWM_CHANNEL           1
#define BUZZER_A_PWM_DIV             125 
#define BUZZER_A_PWM_LEVEL            99 
#define BUZZER_A_PWM_SLICE             2
#define BUZZER_A_PWM_WRAP            999

#define BUTTON_A_GPIO                  5
#define BUTTON_B_GPIO                  6

// I2C_MAIN used by OLED
#define I2C_MAIN_GPIO_SCL             15
#define I2C_MAIN_GPIO_SDA             14
#define I2C_MAIN_BAUDRATE         400000
#define I2C_MAIN_BUS                i2c1

#define LED_R_GPIO                    13
#define LED_G_GPIO                    11
#define LED_B_GPIO                    12
#define LED_TEST_TIME_MS             500


#endif // CONFIG_H