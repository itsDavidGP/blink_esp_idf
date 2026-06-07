#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

#define LED_PIN 48
#define BUTTON_PIN 0

static const char *TAG = "Blink de 0";

static uint8_t s_led_state = 0;

static led_strip_handle_t led_strip;

static void blink_led(void)
{    if (s_led_state) {
        led_strip_set_pixel(led_strip, 0, 16, 16, 16);
        led_strip_refresh(led_strip);
    } else {
        led_strip_clear(led_strip);
    }
}

static void configure_led(void)
{
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_PIN,
        .max_leds = 1,
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));

    led_strip_clear(led_strip);
}

void app_main(void)
{

    configure_led();

    gpio_config_t boot_button = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&boot_button);

    while(1){

        int button_state = gpio_get_level(BUTTON_PIN);

        blink_led();
        s_led_state = !s_led_state;

        if(button_state){
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        else{
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }
        
    }

}
