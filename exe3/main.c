#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED_PIN_R = 4;

volatile int flag_f_r = 0;  
volatile uint32_t start_time = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (gpio == BTN_PIN_R) {
        if (events == 0x4) {  
            start_time = to_ms_since_boot(get_absolute_time());  
        } 
        else if (events == 0x8) {  
            uint32_t end_time = to_ms_since_boot(get_absolute_time());
            uint32_t duration = end_time - start_time;

            if (duration > 500) { 
                flag_f_r = 1;
            }
        }
    }
}

int main() {
    stdio_init_all();
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_put(LED_PIN_R, 0);  

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);
    gpio_set_irq_enabled_with_callback(
        BTN_PIN_R, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);

    while (true) {
        if (flag_f_r) {
            gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R));  
            flag_f_r = false;
        }
    }
}
