#ifndef LED_H
#define LED_H

typedef enum
{
    SOLID = 0,
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED = 1000,
    BLINK_SUSPENDED = 2500
} blink_interval_t;

void led_blinking_task(void);

void led_set_blink_interval(blink_interval_t interval);

#endif