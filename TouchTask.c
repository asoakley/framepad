#include "TouchTask.h"
#include "LCD_Touch.h"
#include "StreamDeck.h"
#include "bsp/board.h"

static uint32_t last_touch_time = 0;
#define TOUCH_DEBOUNCE_MS 200

void touchpad_task(void)
{
    // Poll every 50ms
    const uint32_t interval_ms = 50;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms)
        return;
    start_ms += interval_ms;

    // Check for touch
    if (TP_Scan(0))
    { // 0 for relative position
        // Debounce
        if (board_millis() - last_touch_time > TOUCH_DEBOUNCE_MS)
        {
            uint16_t x = sTP_Draw.Xpoint;
            uint16_t y = sTP_Draw.Ypoint;
            stream_deck_handle_touch(x, y);
            last_touch_time = board_millis();
        }
    }
}