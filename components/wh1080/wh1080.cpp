#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/hal.h"
#include "wh1080.h"

namespace esphome {
namespace wh1080 {

static const char *const TAG = "wh1080";

bool Wh1080Component::validate_(uint8_t *data, uint8_t len, int8_t except) {
  ESP_LOGV(TAG, "Validating data: %s", format_hex(data, len).c_str());

  uint8_t crc = 0;
  uint8_t checklen = 9;
  uint8_t *addr = &data[0];
  // Indicated changes are from reference CRC-8 function in OneWire library
  while (checklen--) {
    uint8_t inbyte = *addr++;
    for (uint8_t i = 8; i; i--) {
      uint8_t mix = (crc ^ inbyte) & 0x80; // changed from & 0x01
      crc <<= 1; // changed from right shift
      if (mix) crc ^= 0x31;// changed from 0x8C;
      inbyte <<= 1; // changed from right shift
    }
  }
  if (data[9] != crc) {
    ESP_LOGV(TAG, "crc received: %s, calculated: %s",format_hex_pretty(data[9]).c_str(),format_hex_pretty(crc).c_str());
    return false;
  }
  return true;
}

void Wh1080Component::decode_temperature_(uint8_t *data, uint8_t len) {
  if (len == 10 && this->validate_(data, len, -1)) {
    uint8_t deviceId = (data[0] << 4) | (data[1] >> 4);
    float temp = (float)((((int32_t)(data[1] & 0x0F) << 8) | (int32_t)data[2]) - 400) / 10;
    float humidity = (float)data[3];
    float windAvg = (float)data[4] * 0.34;
    float windGust = (float)data[5] * 0.34;
    float rain = (float)((int32_t)data[6] << 8 | (int32_t)data[7]) * 0.2794;
    uint8_t batteryFlag = (data[8] >> 4) == 0 ? 100 : 0;
    float windDirection = (float)(data[8] & 0x0F) * 22.5;

    ESP_LOGD(TAG, "Temperature: id %04x, bat %x, temp %.1f, rh %.1f", deviceId, batteryFlag, temp, humidity);
    for (auto *device : this->devices_) {
      if (device->get_id() == deviceId) {
        device->update_temperature(temp);
        device->update_humidity(humidity);
        device->update_rainfall(rain);
        device->update_speed(windAvg);
        device->update_gust_speed(windGust);
        device->update_direction(windDirection);
        device->update_battery(batteryFlag);
      }
    }
  }
}

bool Wh1080Component::on_receive(remote_base::RemoteReceiveData data) {
  uint8_t bytes[10] = {0};
  uint32_t bits = 0;
  uint32_t syncs = 0;

  if (data.size() > 171 && data.size() < 175)
    ESP_LOGV(TAG, "Received raw data with length %" PRIi32, data.size());

  // decode WH1080 OOK data
  data.set_tolerance(200, remote_base::TOLERANCE_MODE_TIME);
  while (data.is_valid(1)) {
    bool is_sync = data.peek_mark(500,1) && data.peek_space(1000);
    bool is_zero = data.peek_mark(1500,1) && data.peek_space(1000);
    bool is_one = data.peek_mark(500,1) && data.peek_space(1000);
    if ((is_one || is_zero) && syncs > 4) {
      if (bits == 0) {
        // Align bits
        if (data.size() == 174)
        {
          data.advance(2);
        }
        //ESP_LOGV(TAG, "Found Sync, %" PRIi32, syncs);
      }
      //
      if (data.peek(1) > 0) {
        // detect bits using on state
        bytes[bits / 8] <<= 1;
        bytes[bits / 8] |= is_one ? 1 : 0;
        bits += 1;

        // try to decode on whole bytes
        if ((bits & 7) == 0) {
          this->decode_temperature_(bytes, bits / 8);
        }

        // reset if buffer is full
        if (bits >= sizeof(bytes) * 8) {
          bits = 0;
          syncs = 0;
        }
      data.advance();
      }
    } else if (is_sync && bits == 0) {
      // count syncs
      syncs++;
      data.advance();
    } else {
      // reset state
      bits = 0;
      syncs = 0;
    }
    data.advance();
  }
  return true;
}

}  // namespace wh1080
}  // namespace esphome
