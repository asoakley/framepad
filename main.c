#include "LCD_Driver.h"
#include "LCD_Touch.h"
#include "LCD_GUI.h"
#include "LCD_Bmp.h"
#include "DEV_Config.h"
#include <stdio.h>
#include "hardware/watchdog.h"
#include "pico/stdlib.h"
#include "comms.h"
#include "tusb.h"
#include "tusb_config.h"
#include <stdlib.h>
#include <string.h>

#include "bsp/board.h"
#include "usb_descriptors.h"

#include "HIDTask.h"
#include "LEDTask.h"
#include "SleepTask.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

int main(void)
{
  System_Init();

  board_init();

  SD_Init();

  LCD_SCAN_DIR lcd_scan_dir = SCAN_DIR_DFT;
  LCD_Init(lcd_scan_dir, 800);
  TP_Init(lcd_scan_dir);

  tusb_init();

  while (1)
  {
    // TP_DrawBoard();

    tud_task(); // tinyusb device task
    led_blinking_task();

    touchpad_task();
    hid_task();

    lcd_task();

    sleep_task();
  }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  led_set_blink_interval(BLINK_MOUNTED);
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  led_set_blink_interval(BLINK_NOT_MOUNTED);
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void)remote_wakeup_en;
  led_set_blink_interval(BLINK_SUSPENDED);
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  led_set_blink_interval(BLINK_MOUNTED);
}
