#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/wh1080/wh1080.h"

namespace esphome {
namespace wh1080 {

class Wh1080Sensor : public Component, public Wh1080Device {
 public:
  void update_speed(float value) override;
  void update_gust_speed(float value) override;
  void update_direction(float value) override;
  void update_temperature(float value) override;
  void update_humidity(float value) override;
  void update_rainfall(float value) override;
  void dump_config() override;

 protected:
  float rainfall_last_{0};
  float humidity_last_{1000};
  float temperature_last_{1000};

  SUB_SENSOR(direction)
  SUB_SENSOR(speed)
  SUB_SENSOR(gust_speed)
  SUB_SENSOR(temperature)
  SUB_SENSOR(humidity)
  SUB_SENSOR(rainfall)
};

}  // namespace wh1080
}  // namespace esphome
