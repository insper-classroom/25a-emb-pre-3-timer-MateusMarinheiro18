#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED_PIN_R = 4;

volatile int flag_timer_running = 0;  
volatile int g_timer_0 = 0;      
repeating_timer_t timer_0;                 

bool timer_0_callback(repeating_timer_t *rt) {
    g_timer_0 = 1;
    return true;
}

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) { 
        if (!flag_timer_running) {
            
            add_repeating_timer_us(500000, timer_0_callback, NULL, &timer_0);
            flag_timer_running = 1;
        } else {
            
            cancel_repeating_timer(&timer_0);
            flag_timer_running = 0;
            gpio_put(LED_PIN_R, 0); 
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

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    while (true) {
        if (g_timer_0) {
            gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R)); 
            g_timer_0 = 0;
        }
    }
}
