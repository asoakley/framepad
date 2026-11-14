#include "SlideshowPageController.hpp"

#include "PageNavigation.hpp"

#include "GUI.h"

bool SlideshowPageController::initialize()
{
    return PageNavigation::register_indicator(
               GUI_Panel__Framepad___Slideshow__Page_indicator_5, FeaturePage::Macros) &&
           PageNavigation::register_indicator(
               GUI_Panel__Framepad___Slideshow__Page_indicator_4, FeaturePage::Slideshow) &&
           PageNavigation::register_indicator(
               GUI_Panel__Framepad___Slideshow__Page_indicator_3, FeaturePage::Music);
}
