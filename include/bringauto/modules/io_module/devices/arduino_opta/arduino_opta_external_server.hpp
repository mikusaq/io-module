#pragma once

#include <device_management.h>
#include <bringauto/io_module_utils/external_server_api_structures.hpp>

namespace bringauto::modules::io_module::devices::arduino_opta {

// These functions are implementing function defined in external_server_api.h specifically for Arduino Opta device

int arduino_opta_forward_status(const buffer device_status, const device_identification device, io_module_utils::context *context);

}
