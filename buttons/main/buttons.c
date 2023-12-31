
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

#define ALIEN_NUMBER 11 //0-12
#define DELAY_TIME 200
 
volatile bool button_pressed = false;
volatile bool right_button_pressed = false;
volatile bool left_button_pressed = false;
volatile bool select_button_pressed = false;
volatile unsigned char cnt = 0;

//variables to keep track of the timing of recent interrupts
int64_t  button_time = 0;  
int64_t  last_button_time = 0; 

char mode = 1;
int alienNo = 0;

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
			
			switch (mode) {
				//Omnitrix is in start mode
				case 1:
					//Go to select alien mode
					mode = 2;
					break;
				//Omnitrix is in select alien mode
				case 2: 
					//Go to start mode
					mode = 1;
					break;
			}
			ESP_LOGI(TAG, "mode %d", mode);
        }
		else if (right_button_pressed) {
			right_button_pressed = false;
			ESP_LOGI(TAG, "Right Button Pressed\n");
			
			switch (mode) {
				//Omnitrix is in select alien mode
				case 2:
					if ((alienNo  + 1)> ALIEN_NUMBER ) {
						alienNo = 0;
					}
					else {
						alienNo += 1;
					}
				ESP_LOGI(TAG, "alien: %d", alienNo);
				break;
			}
        }
		else if (left_button_pressed) {
			left_button_pressed = false;
			ESP_LOGI(TAG, "Left Button Pressed\n");
			
			switch (mode) {
				//Omnitrix is in select alien mode
				case 2:
					if ((alienNo - 1) < 0 ) {
					alienNo = ALIEN_NUMBER;
				}
				else {
					alienNo -= 1;
				}
				ESP_LOGI(TAG, "alien: %d", alienNo);
				break;
			}
        }
		else if (select_button_pressed) {
			select_button_pressed = false;
			ESP_LOGI(TAG, "Select Button Pressed\n");

			switch (mode) {
				//Omnitrix is in select alien mode
				case 2:
					//Go to transformation mode
					mode = 3;
					break;
				//Omnitrix is in transformation mode
				case 3:
					//Go to start mode
					mode = 1;
					break;
			}
			ESP_LOGI(TAG, "mode %d", mode);
        }
        vTaskDelay(DELAY_TIME / portTICK_PERIOD_MS);
    }
}