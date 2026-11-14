#include "FramepadApp.hpp"

extern "C"
{
#include "LEDTask.h"
#include "tusb.h"

void tud_mount_cb(void)
{
    FramepadApp::device_state().set_connection_state(ConnectionState::Connected);
    led_set_blink_interval(BLINK_MOUNTED);
}

void tud_umount_cb(void)
{
    FramepadApp::device_state().set_connection_state(ConnectionState::Disconnected);
    led_set_blink_interval(BLINK_NOT_MOUNTED);
}

void tud_suspend_cb(bool remote_wakeup_en)
{
    (void)remote_wakeup_en;
    FramepadApp::device_state().set_connection_state(ConnectionState::Suspended);
    led_set_blink_interval(BLINK_SUSPENDED);
}

void tud_resume_cb(void)
{
    FramepadApp::device_state().set_connection_state(ConnectionState::Connected);
    led_set_blink_interval(BLINK_MOUNTED);
}
}
