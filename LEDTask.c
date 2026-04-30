#include "LEDTask.h"
#include "bsp/board.h"

static blink_interval_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void)
{
    static uint32_t start_ms = 0;
    static bool led_state = false;

    // blink is disabled
    if (blink_interval_ms == SOLID)
    {
        board_led_write(true);
        return;
    }

    // Blink every interval ms
    if (board_millis() - start_ms < blink_interval_ms)
        return; // not enough time
    start_ms += blink_interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}

void led_set_blink_interval(blink_interval_t interval)
{
    blink_interval_ms = interval;
}