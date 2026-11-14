#ifndef FRAMEPAD_APP_HPP
#define FRAMEPAD_APP_HPP

#include "DeviceState.hpp"

#ifdef __cplusplus
class FramepadApp
{
public:
    static void run();
    static DeviceState& device_state();
    static void service_usb();

private:
    FramepadApp() = delete;

    static DeviceState device_state_;
    static void initialize();
    static void initialize_hardware();
    static void initialize_gui();
    static void initialize_usb();
    static void process();
};
#endif

#endif
