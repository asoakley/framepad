#include "FramepadApp.hpp"
#include "gui/GuiManager.hpp"

extern "C"
{
#include "DEV_Config.h"
#include "LCD_Driver.h"
#include "LCD_Touch.h"
#include "LCD_Bmp.h"
#include "bsp/board.h"
#include "tusb.h"

#include "HIDTask.h"
#include "LEDTask.h"
#include "SleepTask.h"
#include "LCDTask.h"
#include "StreamDeck.h"
}

DeviceState FramepadApp::device_state_{};

DeviceState& FramepadApp::device_state()
{
    return device_state_;
}

void FramepadApp::initialize()
{
    initialize_hardware();
    initialize_gui();
    initialize_usb();
}

void FramepadApp::initialize_hardware()
{
    System_Init();
    board_init();
    SD_Init();

    LCD_SCAN_DIR lcd_scan_dir = SCAN_DIR_DFT;
    LCD_Init(lcd_scan_dir, 800);
    TP_Init(lcd_scan_dir);
    TP_GetAdFac();
}

void FramepadApp::initialize_gui()
{
    stream_deck_load_config();
    GuiManager::initialize();
}

void FramepadApp::initialize_usb()
{
    tusb_init();
}

void FramepadApp::service_usb()
{
    tud_task();
    hid_task();
}

void FramepadApp::process()
{
    service_usb();
    led_blinking_task();
    lcd_task();
    sleep_task();
    GuiManager::process();
}

void FramepadApp::run()
{
    initialize();

    while (true)
    {
        process();
    }
}
