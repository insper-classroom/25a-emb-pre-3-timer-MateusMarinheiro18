#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile int flag_r = 0;
volatile int flag_g = 0;

volatile int g_timer_0 = 0;
volatile int g_timer_1 = 0;

repeating_timer_t timer_0;
repeating_timer_t timer_1;

bool timer_0_callback(repeating_timer_t *rt) {
    gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R));
    return true;
}

bool timer_1_callback(repeating_timer_t *rt) {
    gpio_put(LED_PIN_G, !gpio_get(LED_PIN_G));
    return true;
}

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {
        if (gpio == BTN_PIN_R)
            if (!flag_r){
                add_repeating_timer_us(500000, timer_0_callback, NULL, &timer_0);
                flag_r = 1;
            }
            else{
                cancel_repeating_timer(&timer_0);
                flag_r = 0;
                gpio_put(LED_PIN_R, 0);
            }
        else if (gpio == BTN_PIN_G){
            if (!flag_g){
                add_repeating_timer_us(250000, timer_1_callback, NULL, &timer_1);
                flag_g = 1;
            }
            else{
                cancel_repeating_timer(&timer_1);
                flag_g = 0;
                gpio_put(LED_PIN_G, 0);
            }
        }
    }
}

int main() {
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_put(LED_PIN_R, 0);

    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);
    gpio_put(LED_PIN_G, 0);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                       &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);


    while (true) {
        if (g_timer_0) {
            gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R));
            g_timer_0 = 0;
        }
        if (g_timer_1) {
            gpio_put(LED_PIN_G, !gpio_get(LED_PIN_G));
            g_timer_1 = 0;
        }
    }
}
