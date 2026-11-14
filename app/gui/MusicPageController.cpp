#include "MusicPageController.hpp"

#include "PageNavigation.hpp"

#include "GUI.h"

bool MusicPageController::initialize()
{
    return PageNavigation::register_indicator(
               GUI_Panel__Framepad___Music_control__Page_indicator_2, FeaturePage::Macros) &&
           PageNavigation::register_indicator(
               GUI_Panel__Framepad___Music_control__Page_indicator_1, FeaturePage::Slideshow) &&
           PageNavigation::register_indicator(
               GUI_Panel__Framepad___Music_control__Page_indicator, FeaturePage::Music);
}
