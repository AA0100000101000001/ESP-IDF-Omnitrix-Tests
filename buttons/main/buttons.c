
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BTN 5

#define DELAY_TIME 200
 
volatile bool button_pressed = false;

static void gpio_isr_handler(void* arg) 
{
    button_pressed = true;
}

void button_config()
{
    gpio_install_isr_service(0);
    printf("configuring button\n");
    gpio_reset_pin(BTN);
    gpio_set_direction(BTN, GPIO_MODE_INPUT);
    gpio_pullup_en(BTN);
    gpio_set_intr_type(BTN, GPIO_INTR_POSEDGE);
    gpio_isr_handler_add(BTN, gpio_isr_handler, NULL);
    printf("config complete\n");
}

void app_main()
{   
    //uint8_t led_value = 0;

    button_config();

    while (1) {
        if (button_pressed) {
            printf("*\n");
            button_pressed = false;
			printf("Button Pressed");
        }
        vTaskDelay(DELAY_TIME / portTICK_PERIOD_MS);
    }
}