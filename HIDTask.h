#ifndef HID_H
#define HID_H

#include <stdint.h>
#include "tusb.h"

typedef enum
{
    HID_STATE_IDLE,
    HID_STATE_SENT_WIN_R,
    HID_STATE_SENT_EMPTY_WIN_R,
    HID_STATE_WAITING_FOR_RUN_DIALOG,
    HID_STATE_SENDING_PROMPT,
    HID_STATE_SENT_PROMPT_KEY,
    HID_STATE_SENT_EMPTY_PROMPT,
    HID_STATE_SENT_ENTER,
    HID_STATE_SENT_EMPTY_ENTER
} hid_state_t;

typedef struct
{
    uint8_t modifier;
    uint8_t keycode;
} key_press_t;

void hid_task(void);

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len);

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen);

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize);

#endif