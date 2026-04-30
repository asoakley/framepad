#include "SleepTask.h"
#include "bsp/board.h"

void sleep_task(void)
{
    static bool gb_backlightState = true;
    static uint32_t start_ms = 0;

    // Blink every interval ms
    if (board_millis() - start_ms < 1000)
        return; // not enough time
    start_ms += 1000;
}