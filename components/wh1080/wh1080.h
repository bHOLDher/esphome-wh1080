#pragma once

#include "esphome/components/remote_receiver/remote_receiver.h"

namespace esphome {
namespace wh1080 {

class Wh1080Device {
 public:
  virtual void update_battery(uint8_t value) {}
  virtual void update_speed(float value) {}
  virtual void update_gust_speed(float value) {}
  virtual void update_direction(float value) {}
  virtual void update_temperature(float value) {}
  virtual void update_humidity(float value) {}
  virtual void update_rainfall(float value) {}

  void set_id(uint16_t id) { this->id_ = id; }
  uint16_t get_id() { return this->id_; }

 protected:
  uint16_t id_{0};
};

class Wh1080Component : public Component, public remote_base::RemoteReceiverListener {
 public:
  void add_device(Wh1080Device *dev, uint16_t id) { this->devices_.push_back(dev); }
  bool on_receive(remote_base::RemoteReceiveData data) override;

 protected:
  void decode_temperature_(uint8_t *data, uint8_t len);
  bool validate_(uint8_t *data, uint8_t len, int8_t except);
  std::vector<Wh1080Device *> devices_;
};

}  // namespace wh1080
}  // namespace esphome
