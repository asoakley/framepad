#include "DeviceState.hpp"

ConnectionState DeviceState::connection_state() const
{
    return connection_state_;
}

void DeviceState::set_connection_state(ConnectionState state)
{
    connection_state_ = state;
}

FeaturePage DeviceState::active_page() const
{
    return active_page_;
}

void DeviceState::set_active_page(FeaturePage page)
{
    active_page_ = page;
}
