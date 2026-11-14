#include "GuiManager.hpp"

#include "FramepadApp.hpp"
#include "MacroPageController.hpp"
#include "MusicPageController.hpp"
#include "PageNavigation.hpp"
#include "SlideshowPageController.hpp"

#include "GUI.h"

#include <stdio.h>

extern "C"
{
#include "LCD_Driver.h"
#include "LCD_Touch.h"
#include "bsp/board.h"
#include "pico/stdlib.h"
}

namespace
{
constexpr int32_t display_width = 480;
constexpr int32_t display_height = 320;
constexpr uint32_t draw_buffer_rows = 20;
constexpr uint32_t draw_buffer_pixels = display_width * draw_buffer_rows;

alignas(LV_DRAW_BUF_ALIGN) static uint16_t draw_buffer[2][draw_buffer_pixels];
static lv_display_t *active_display = nullptr;

static_assert(LV_COLOR_DEPTH == 16, "The LCD port requires RGB565 color depth");
static_assert(sizeof(draw_buffer[0]) % LV_DRAW_BUF_ALIGN == 0,
              "Both draw buffers must satisfy LVGL alignment");

void flush_display(lv_display_t *display, const lv_area_t *area, uint8_t *pixel_map)
{
    const int32_t width = area->x2 - area->x1 + 1;
    const int32_t height = area->y2 - area->y1 + 1;
    if (width <= 0 || height <= 0 ||
        area->x1 < 0 || area->y1 < 0 ||
        area->x2 >= display_width || area->y2 >= display_height ||
        static_cast<uint32_t>(width) * static_cast<uint32_t>(height) > draw_buffer_pixels)
    {
        printf("Invalid LVGL display flush area\n");
        lv_display_flush_ready(display);
        return;
    }

    const uint32_t pixel_count = static_cast<uint32_t>(width) * static_cast<uint32_t>(height);
    LCD_SetWindow(static_cast<POINT>(area->x1),
                  static_cast<POINT>(area->y1),
                  static_cast<POINT>(area->x2 + 1),
                  static_cast<POINT>(area->y2 + 1));
    if (!LCD_WritePixelsDMA(reinterpret_cast<const uint16_t *>(pixel_map), pixel_count))
        lv_display_flush_ready(display);
}

void wait_for_flush(lv_display_t *display)
{
    while (!LCD_PixelDMAComplete())
        FramepadApp::service_usb();

    lv_display_flush_ready(display);
}

void read_touch(lv_indev_t *, lv_indev_data_t *data)
{
    // Touch and LCD share SPI; the final flush may outlive lv_timer_handler().
    wait_for_flush(active_display);
    if (TP_Scan(0))
    {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = sTP_Draw.Xpoint;
        data->point.y = sTP_Draw.Ypoint;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}
}

void GuiManager::initialize_lvgl_port()
{
    lv_tick_set_cb(board_millis);

    lv_display_t *display = lv_display_create(display_width, display_height);
    if (display == nullptr)
        panic("LVGL display allocation failed");

    LCD_InitPixelDMA();
    active_display = display;
    lv_display_set_default(display);
    lv_display_set_buffers(display,
                           draw_buffer[0],
                           draw_buffer[1],
                           sizeof(draw_buffer[0]),
                           LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(display, flush_display);
    lv_display_set_flush_wait_cb(display, wait_for_flush);

    lv_indev_t *touch = lv_indev_create();
    if (touch == nullptr)
        panic("LVGL touch input allocation failed");
    lv_indev_set_type(touch, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(touch, read_touch);
}

void GuiManager::initialize()
{
    GUI_load();

    if (!MacroPageController::initialize())
        panic("Macro page initialization failed");
    if (!SlideshowPageController::initialize())
        panic("Slideshow page initialization failed");
    if (!MusicPageController::initialize())
        panic("Music page initialization failed");

    FramepadApp::device_state().set_active_page(FeaturePage::Macros);
}

void GuiManager::process()
{
    if (LCD_PixelDMAComplete())
        lv_display_flush_ready(active_display);
    lv_timer_handler();
}

void GuiManager::show_page(FeaturePage page)
{
    lv_obj_t *screen = nullptr;
    switch (page)
    {
    case FeaturePage::Macros:
        screen = GUI_Screen__Framepad;
        break;
    case FeaturePage::Slideshow:
        screen = GUI_Screen__Framepad___Slideshow;
        break;
    case FeaturePage::Music:
        screen = GUI_Screen__Framepad___Music_control;
        break;
    }

    if (screen == nullptr)
    {
        printf("Requested LVGL page is unavailable\n");
        return;
    }

    lv_screen_load(screen);
    FramepadApp::device_state().set_active_page(page);
}

extern "C" void HAL_init()
{
    GuiManager::initialize_lvgl_port();
}
