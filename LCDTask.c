#include "LCDTask.h"
#include "bsp/board.h"

void lcd_task(void)
{
    // Poll every 1000ms
    const uint32_t interval_ms = 1000;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms)
        return;
    start_ms += interval_ms;

    // For now, nothing to do - grid is drawn once at init
    // Could be used for animations or updates later
}