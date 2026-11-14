#include "MacroPageController.hpp"

#include "PageNavigation.hpp"

#include "GUI.h"

#include <stdio.h>

extern "C"
{
#include "StreamDeck.h"
}

namespace
{
uint8_t macro_indices[MAX_ICONS] = {0, 1, 2, 3, 4, 5};

void macro_button_clicked(lv_event_t *event)
{
    uint8_t *icon_index = static_cast<uint8_t *>(lv_event_get_user_data(event));
    if (icon_index == nullptr || !stream_deck_activate_icon(*icon_index))
        printf("Macro button has no active SD command\n");
}
}

bool MacroPageController::initialize()
{
    lv_obj_t *const macro_buttons[MAX_ICONS] = {
        GUI_Button__Framepad__Macro_button_5,
        GUI_Button__Framepad__Macro_button_4,
        GUI_Button__Framepad__Macro_button_3,
        GUI_Button__Framepad__Macro_button_2,
        GUI_Button__Framepad__Macro_button_1,
        GUI_Button__Framepad__Macro_button};

    lv_obj_t *const macro_app_labels[MAX_ICONS] = {
        GUI_Label__Framepad__App_5,
        GUI_Label__Framepad__App_4,
        GUI_Label__Framepad__App_3,
        GUI_Label__Framepad__App_2,
        GUI_Label__Framepad__App_1,
        GUI_Label__Framepad__App};

    lv_obj_t *const macro_action_labels[MAX_ICONS] = {
        GUI_Label__Framepad__Action_5,
        GUI_Label__Framepad__Action_4,
        GUI_Label__Framepad__Action_3,
        GUI_Label__Framepad__Action_2,
        GUI_Label__Framepad__Action_1,
        GUI_Label__Framepad__Action};

    for (uint8_t i = 0; i < MAX_ICONS; i++)
    {
        if (i < stream_deck_config.num_icons && stream_deck_config.icons[i].active)
        {
            lv_label_set_text(macro_app_labels[i], "Macro");
            lv_label_set_text(macro_action_labels[i], stream_deck_config.icons[i].command);
            if (lv_obj_add_event_cb(macro_buttons[i],
                                    macro_button_clicked,
                                    LV_EVENT_CLICKED,
                                    &macro_indices[i]) == nullptr)
            {
                return false;
            }
        }
        else
        {
            lv_label_set_text(macro_app_labels[i], "Unavailable");
            lv_label_set_text(macro_action_labels[i], "Configure SD card");
            lv_obj_add_state(macro_buttons[i], LV_STATE_DISABLED);
        }
    }

    lv_obj_remove_flag(GUI_Container__Framepad__First_Page, LV_OBJ_FLAG_HIDDEN);
    return PageNavigation::register_indicator(
               GUI_Panel__Framepad__Page_1_Indicator_1, FeaturePage::Macros) &&
           PageNavigation::register_indicator(
               GUI_Panel__Framepad__Page_2_Indicator_1, FeaturePage::Slideshow) &&
           PageNavigation::register_indicator(
               GUI_Panel__Framepad__Page_3_Indicator_1, FeaturePage::Music);
}
