#include "esphome/core/log.h"
#include "wh1080_binary_sensor.h"

namespace esphome {
namespace wh1080 {

static const char *const TAG = "wh1080";

void Wh1080BinarySensor::update_battery(uint8_t value) {
  if (this->battery_level_binary_sensor_) {
    this->battery_level_binary_sensor_->publish_state(value == 0);
  }
}

void Wh1080BinarySensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Wh1080 Binary Sensor: 0x%04x", this->id_);
  LOG_BINARY_SENSOR("  ", "Battery", this->battery_level_binary_sensor_);
}

}  // namespace wh1080
}  // namespace esphome
