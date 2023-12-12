
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"

static const char *TAG = "MAIN";

#define BTN 5 			//Start button
#define BTN_RIGHT 1 	//Encoder right
#define BTN_LEFT 4 		//Encoder left
#define BTN_SELECT 3 	//Encoder button

#define DELAY_TIME 200
 
volatile bool button_pressed = false;
volatile bool right_button_pressed = false;
volatile bool left_button_pressed = false;
volatile bool select_button_pressed = false;
volatile unsigned char cnt = 0;

//variables to keep track of the timing of recent interrupts
int64_t  button_time = 0;  
int64_t  last_button_time = 0; 

static void btn_isr_handler(void* arg) 
{
	button_time = esp_timer_get_time();
	if (button_time - last_button_time > DELAY_TIME)
	{
		button_pressed = true;
		last_button_time = button_time;
	}
}

static void right_btn_isr_handler(void* arg) 
{
	button_time = esp_timer_get_time();
	if (button_time - last_button_time > DELAY_TIME)
	{
		right_button_pressed = true;
		last_button_time = button_time;
	}
}

static void left_btn_isr_handler(void* arg) 
{
	button_time = esp_timer_get_time();
	if (button_time - last_button_time > DELAY_TIME)
	{
		left_button_pressed = true;
		last_button_time = button_time;
	}
}

static void select_btn_isr_handler(void* arg) 
{
	button_time = esp_timer_get_time();
	if (button_time - last_button_time > DELAY_TIME)
	{
		select_button_pressed = true;
		last_button_time = button_time;
	}
}

void button_config()
{
    gpio_install_isr_service(0);
    printf("configuring buttons\n");
    gpio_reset_pin(BTN);
    gpio_set_direction(BTN, GPIO_MODE_INPUT);
    gpio_pullup_en(BTN);
    gpio_set_intr_type(BTN, GPIO_INTR_POSEDGE);
    gpio_isr_handler_add(BTN, btn_isr_handler, NULL);
	
    gpio_reset_pin(BTN_RIGHT);
    gpio_set_direction(BTN_RIGHT, GPIO_MODE_INPUT);
    gpio_pullup_en(BTN_RIGHT);
    gpio_set_intr_type(BTN_RIGHT, GPIO_INTR_POSEDGE);
    gpio_isr_handler_add(BTN_RIGHT, right_btn_isr_handler, NULL);
	
    gpio_reset_pin(BTN_LEFT);
    gpio_set_direction(BTN_LEFT, GPIO_MODE_INPUT);
    gpio_pullup_en(BTN_LEFT);
    gpio_set_intr_type(BTN_LEFT, GPIO_INTR_POSEDGE);
    gpio_isr_handler_add(BTN_LEFT, left_btn_isr_handler, NULL);
	
    gpio_reset_pin(BTN_SELECT);
    gpio_set_direction(BTN_SELECT, GPIO_MODE_INPUT);
    gpio_pullup_en(BTN_SELECT);
    gpio_set_intr_type(BTN_SELECT, GPIO_INTR_POSEDGE);
    gpio_isr_handler_add(BTN_SELECT, select_btn_isr_handler, NULL);
    
	printf("config complete\n");
}

void app_main()
{   

    button_config();

    while (1) {
        if (button_pressed) {
			button_pressed = false;
			ESP_LOGI(TAG, "Button Pressed\n");
        }
		else if (right_button_pressed) {
			right_button_pressed = false;
			ESP_LOGI(TAG, "Right Button Pressed\n");
        }
		else if (left_button_pressed) {
			left_button_pressed = false;
			ESP_LOGI(TAG, "Left Button Pressed\n");
        }
		else if (select_button_pressed) {
			select_button_pressed = false;
			ESP_LOGI(TAG, "Select Button Pressed\n");
        }
        vTaskDelay(DELAY_TIME / portTICK_PERIOD_MS);
    }
}