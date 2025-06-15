#pragma once

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/wh1080/wh1080.h"

namespace esphome {
namespace wh1080 {

class Wh1080BinarySensor : public Component, public Wh1080Device {
 public:
  void update_battery(uint8_t value) override;
  void dump_config() override;

  SUB_BINARY_SENSOR(battery_level)
};

}  // namespace wh1080
}  // namespace esphome
