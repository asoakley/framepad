#include "HIDTask.h"
#include "tusb.h"
#include "bsp/board.h"
#include "usb_descriptors.h"
#include "DEV_Config.h"
#include "LEDTask.h"
#include <string.h>

// Delay in milliseconds before sending prompt after Win+R
#define WIN_R_DIALOG_DELAY_MS 500

static hid_state_t hid_state = HID_STATE_IDLE;
static bool last_btn = false;
static size_t prompt_index = 0;
static uint32_t delay_start_ms = 0;
static char current_command[MAX_COMMAND_LENGTH] = "cmd";
static key_press_t *current_key_sequence = NULL;
static size_t current_sequence_length = 0;

// Function to convert string to key presses (simple implementation for alphanumeric)
key_press_t *string_to_key_sequence(const char *str, size_t *length)
{
    static key_press_t sequence[MAX_COMMAND_LENGTH];
    *length = 0;

    for (size_t i = 0; str[i] && *length < MAX_COMMAND_LENGTH; i++)
    {
        char c = str[i];
        uint8_t keycode = 0;

        if (c >= 'a' && c <= 'z')
        {
            keycode = HID_KEY_A + (c - 'a');
        }
        else if (c >= 'A' && c <= 'Z')
        {
            keycode = HID_KEY_A + (c - 'A');
            sequence[*length].modifier = KEYBOARD_MODIFIER_LEFTSHIFT;
        }
        else if (c >= '1' && c <= '9')
        {
            keycode = HID_KEY_1 + (c - '1');
        }
        else if (c == '0')
        {
            keycode = HID_KEY_0;
        }
        else
        {
            // Skip unsupported characters for now
            continue;
        }

        sequence[*length].modifier = (c >= 'A' && c <= 'Z') ? KEYBOARD_MODIFIER_LEFTSHIFT : 0;
        sequence[*length].keycode = keycode;
        (*length)++;
    }

    return sequence;
}

void hid_send_command(const char *command)
{
    if (strlen(command) >= MAX_COMMAND_LENGTH)
        return;

    strcpy(current_command, command);
    current_key_sequence = string_to_key_sequence(command, &current_sequence_length);
    prompt_index = 0;

    // Trigger sending Win+R
    if (hid_state == HID_STATE_IDLE)
    {
        uint8_t keycode[6] = {HID_KEY_GUI_LEFT, HID_KEY_R, 0, 0, 0, 0};
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
        hid_state = HID_STATE_SENT_WIN_R;
    }
}

static void send_hid_report(uint8_t report_id, uint32_t btn)
{
    // skip if hid is not ready yet
    if (!tud_hid_ready())
        return;

    switch (report_id)
    {
    case REPORT_ID_KEYBOARD:
    {
        // use to avoid send multiple consecutive zero report for keyboard
        static bool has_keyboard_key = false;

        if (btn)
        {
            uint8_t keycode[6] = {0};
            // Windows key + R to open Run dialog
            keycode[0] = HID_KEY_GUI_LEFT;
            keycode[1] = HID_KEY_R;

            // Handle sending the rest in the report complete callback.

            tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
            has_keyboard_key = true;
        }
        else
        {
            // send empty key report if previously has key pressed
            if (has_keyboard_key)
                tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
            has_keyboard_key = false;
        }
    }
    break;

    case REPORT_ID_MOUSE:
    {
        int8_t const delta = 0;

        // no button, right + down, no scroll, no pan
        tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, delta, delta, 0, 0);
    }
    break;

    case REPORT_ID_CONSUMER_CONTROL:
    {
        // use to avoid send multiple consecutive zero report
        static bool has_consumer_key = false;

        if (btn)
        {
            // volume down
            uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
            tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
            has_consumer_key = true;
        }
        else
        {
            // send empty key report (release key) if previously has key pressed
            uint16_t empty_key = 0;
            if (has_consumer_key)
                tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
            has_consumer_key = false;
        }
    }
    break;

    case REPORT_ID_GAMEPAD:
    {
        // use to avoid send multiple consecutive zero report for keyboard
        static bool has_gamepad_key = false;

        hid_gamepad_report_t report =
            {
                .x = 0, .y = 0, .z = 0, .rz = 0, .rx = 0, .ry = 0, .hat = 0, .buttons = 0};

        if (btn)
        {
            report.hat = GAMEPAD_HAT_UP;
            report.buttons = GAMEPAD_BUTTON_A;
            tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));

            has_gamepad_key = true;
        }
        else
        {
            report.hat = GAMEPAD_HAT_CENTERED;
            report.buttons = 0;
            if (has_gamepad_key)
                tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
            has_gamepad_key = false;
        }
    }
    break;

    default:
        break;
    }
}

void hid_task(void)
{
    // Poll every 10ms
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    start_ms += interval_ms;

    bool btn_pressed = (DEV_Digital_Read(TP_IRQ_PIN) == false);

    if (btn_pressed && !last_btn && hid_state == HID_STATE_IDLE)
    {
        // Send Win+R
        uint8_t keycode[6] = {HID_KEY_GUI_LEFT, HID_KEY_R, 0, 0, 0, 0};
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
        hid_state = HID_STATE_SENT_WIN_R;
        prompt_index = 0;
    }

    // Check if we should start sending the prompt after the delay
    if (hid_state == HID_STATE_WAITING_FOR_RUN_DIALOG)
    {
        uint32_t elapsed = board_millis() - delay_start_ms;
        if (elapsed >= WIN_R_DIALOG_DELAY_MS && tud_hid_ready())
        {
            // Send first prompt character
            if (prompt_index < current_sequence_length)
            {
                uint8_t keycode[6] = {current_key_sequence[prompt_index].keycode, 0, 0, 0, 0, 0};
                tud_hid_keyboard_report(REPORT_ID_KEYBOARD, current_key_sequence[prompt_index].modifier, keycode);
                hid_state = HID_STATE_SENT_PROMPT_KEY;
            }
            else
            {
                uint8_t keycode[6] = {HID_KEY_ENTER, 0, 0, 0, 0, 0};
                tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
                hid_state = HID_STATE_SENT_ENTER;
            }
        }
    }

    last_btn = btn_pressed;

    // Remote wakeup
    if (tud_suspended() && btn_pressed)
    {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    }
}

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len)
{
    (void)instance;
    (void)len;

    switch (hid_state)
    {
    case HID_STATE_SENT_WIN_R:
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        hid_state = HID_STATE_SENT_EMPTY_WIN_R;
        break;

    case HID_STATE_SENT_EMPTY_WIN_R:
        // Start waiting for the Run dialog to open
        delay_start_ms = board_millis();
        hid_state = HID_STATE_WAITING_FOR_RUN_DIALOG;
        break;

    case HID_STATE_WAITING_FOR_RUN_DIALOG:
        // Wait state - handled by hid_task() timer
        break;

    case HID_STATE_SENT_PROMPT_KEY:
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        hid_state = HID_STATE_SENT_EMPTY_PROMPT;
        break;

    case HID_STATE_SENT_EMPTY_PROMPT:
        prompt_index++;
        if (prompt_index < current_sequence_length)
        {
            uint8_t keycode[6] = {current_key_sequence[prompt_index].keycode, 0, 0, 0, 0, 0};
            tud_hid_keyboard_report(REPORT_ID_KEYBOARD, current_key_sequence[prompt_index].modifier, keycode);
            hid_state = HID_STATE_SENT_PROMPT_KEY;
        }
        else
        {
            uint8_t keycode[6] = {HID_KEY_ENTER, 0, 0, 0, 0, 0};
            tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
            hid_state = HID_STATE_SENT_ENTER;
        }
        break;

    case HID_STATE_SENT_ENTER:
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        hid_state = HID_STATE_SENT_EMPTY_ENTER;
        break;

    case HID_STATE_SENT_EMPTY_ENTER:
        hid_state = HID_STATE_IDLE;
        break;

    default:
        break;
    }
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    (void)instance;

    if (report_type == HID_REPORT_TYPE_OUTPUT)
    {
        // Set keyboard LED e.g Capslock, Numlock etc...
        if (report_id == REPORT_ID_KEYBOARD)
        {
            // bufsize should be (at least) 1
            if (bufsize < 1)
                return;

            uint8_t const kbd_leds = buffer[0];

            if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
            {
                // Capslock On: disable blink, turn led on
                led_set_blink_interval(SOLID);
            }
            else
            {
                led_set_blink_interval(BLINK_MOUNTED);
            }
        }
    }
}