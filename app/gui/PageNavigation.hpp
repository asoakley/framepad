#ifndef PAGE_NAVIGATION_HPP
#define PAGE_NAVIGATION_HPP

#include "DeviceState.hpp"
#include "lvgl/lvgl.h"

namespace PageNavigation
{
bool register_indicator(lv_obj_t *indicator, FeaturePage page);
}

#endif
