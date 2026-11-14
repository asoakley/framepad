#include "PageNavigation.hpp"

#include "GuiManager.hpp"

namespace
{
FeaturePage page_targets[] = {
    FeaturePage::Macros,
    FeaturePage::Slideshow,
    FeaturePage::Music};

void page_indicator_clicked(lv_event_t *event)
{
    FeaturePage *page = static_cast<FeaturePage *>(lv_event_get_user_data(event));
    if (page != nullptr)
        GuiManager::show_page(*page);
}
}

bool PageNavigation::register_indicator(lv_obj_t *indicator, FeaturePage page)
{
    const uint8_t page_index = static_cast<uint8_t>(page);
    if (indicator == nullptr || page_index >= sizeof(page_targets) / sizeof(page_targets[0]))
        return false;

    lv_obj_add_flag(indicator, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_ext_click_area(indicator, 3);
    return lv_obj_add_event_cb(indicator,
                               page_indicator_clicked,
                               LV_EVENT_CLICKED,
                               &page_targets[page_index]) != nullptr;
}
