#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include "DeviceState.hpp"

class GuiManager final
{
public:
    static void initialize();
    static void initialize_lvgl_port();
    static void process();
    static void show_page(FeaturePage page);

private:
    GuiManager() = delete;
};

#endif
