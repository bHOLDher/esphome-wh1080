#include "esphome/core/log.h"
#include "wh1080_sensor.h"

namespace esphome {
namespace wh1080 {

static const char *const TAG = "wh1080";

void Wh1080Sensor::update_direction(float value) {
  if (this->direction_sensor_) {
    // do not confirm wind values as they can change rapidly
    if (value >= 0.0f && value <= 360.0f) {
      this->direction_sensor_->publish_state(value);
    }
  }
}

void Wh1080Sensor::update_speed(float value) {
  if (this->speed_sensor_) {
    // do not confirm wind values as they can change rapidly
    if (value >= 0.0f && value <= 257.0f) {
      this->speed_sensor_->publish_state(value);
    }
  }
}
void Wh1080Sensor::update_gust_speed(float value) {
  if (this->gust_speed_sensor_) {
    // do not confirm wind values as they can change rapidly
    if (value >= 0.0f && value <= 257.0f) {
      this->gust_speed_sensor_->publish_state(value);
    }
  }
}

void Wh1080Sensor::update_temperature(float value) {
  if (this->temperature_sensor_) {
    // filter out crc false positives by confirming any large changes in value
    if (fabsf(value - this->temperature_last_) < 1.0) {
      this->temperature_sensor_->publish_state(value);
    }
    this->temperature_last_ = value;
  }
}

void Wh1080Sensor::update_humidity(float value) {
  if (this->humidity_sensor_) {
    // filter out crc false positives by confirming any large changes in value
    if (fabsf(value - this->humidity_last_) < 1.0) {
      this->humidity_sensor_->publish_state(value);
    }
    this->humidity_last_ = value;
  }
}

void Wh1080Sensor::update_rainfall(float value) {
  if (this->rainfall_sensor_) {
    // filter out crc false positives by confirming any change in value
    if (value == this->rainfall_last_) {
      this->rainfall_sensor_->publish_state(value);
    }
    this->rainfall_last_ = value;
  }
}

void Wh1080Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Wh1080 Sensor: 0x%04x", this->id_);
  LOG_SENSOR("  ", "Speed", this->speed_sensor_);
  LOG_SENSOR("  ", "Gust Speed", this->gust_speed_sensor_);
  LOG_SENSOR("  ", "Direction", this->direction_sensor_);
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("  ", "Humidity", this->humidity_sensor_);
  LOG_SENSOR("  ", "Rainfall", this->rainfall_sensor_);
}

}  // namespace wh1080
}  // namespace esphome
