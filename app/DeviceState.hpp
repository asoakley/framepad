#ifndef DEVICE_STATE_HPP
#define DEVICE_STATE_HPP

#include <stdint.h>

enum class ConnectionState : uint8_t
{
    Disconnected,
    Connected,
    Suspended
};

enum class FeaturePage : uint8_t
{
    Macros,
    Slideshow,
    Music
};

class DeviceState final
{
public:
    ConnectionState connection_state() const;
    void set_connection_state(ConnectionState state);

    FeaturePage active_page() const;
    void set_active_page(FeaturePage page);

private:
    ConnectionState connection_state_ = ConnectionState::Disconnected;
    FeaturePage active_page_ = FeaturePage::Macros;
};

#endif
