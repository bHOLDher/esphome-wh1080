import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_DEVICE,
    CONF_DIRECTION,
    CONF_HUMIDITY,
    CONF_ID,
    CONF_SPEED,
    CONF_TEMPERATURE,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_ILLUMINANCE,
    DEVICE_CLASS_PRECIPITATION,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_WIND_SPEED,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL,
    UNIT_CELSIUS,
    UNIT_DEGREES,
    UNIT_EMPTY,
    UNIT_KILOMETER,
    UNIT_KILOMETER_PER_HOUR,
    UNIT_LUX,
    UNIT_PERCENT,
)

from .. import Wh1080Component, wh1080_ns

DEPENDENCIES = ["wh1080"]

CONF_WH1080_ID = "wh1080_id"
CONF_DEVICES = "devices"
CONF_RAIN = "rain"
UNIT_MILLIMETER = "mm"
UNIT_METERS_PER_SECOND = "m/s"
CONF_GUST_SPEED = "gust_speed"

Wh1080Sensor = wh1080_ns.class_("Wh1080Sensor", cg.Component)

DEVICE_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Wh1080Sensor),
        cv.Required(CONF_DEVICE): cv.hex_int_range(max=0x3FFF),
        cv.Optional(CONF_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_METERS_PER_SECOND,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_WIND_SPEED,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_GUST_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_METERS_PER_SECOND,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_WIND_SPEED,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_DIRECTION): sensor.sensor_schema(
            unit_of_measurement=UNIT_DEGREES,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_EMPTY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_HUMIDITY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_RAIN): sensor.sensor_schema(
            unit_of_measurement=UNIT_MILLIMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_PRECIPITATION,
            state_class=STATE_CLASS_TOTAL,
        ),
    }
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_WH1080_ID): cv.use_id(Wh1080Component),
        cv.Required(CONF_DEVICES): cv.ensure_list(DEVICE_SCHEMA),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_WH1080_ID])
    if devices_cfg := config.get(CONF_DEVICES):
        for device_cfg in devices_cfg:
            var = cg.new_Pvariable(device_cfg[CONF_ID])
            await cg.register_component(var, device_cfg)
            cg.add(var.set_id(device_cfg[CONF_DEVICE]))
            if CONF_SPEED in device_cfg:
                sens = await sensor.new_sensor(device_cfg[CONF_SPEED])
                cg.add(var.set_speed_sensor(sens))
            if CONF_GUST_SPEED in device_cfg:
                sens = await sensor.new_sensor(device_cfg[CONF_GUST_SPEED])
                cg.add(var.set_gust_speed_sensor(sens))
            if CONF_DIRECTION in device_cfg:
                sens = await sensor.new_sensor(device_cfg[CONF_DIRECTION])
                cg.add(var.set_direction_sensor(sens))
            if CONF_TEMPERATURE in device_cfg:
                sens = await sensor.new_sensor(device_cfg[CONF_TEMPERATURE])
                cg.add(var.set_temperature_sensor(sens))
            if CONF_HUMIDITY in device_cfg:
                sens = await sensor.new_sensor(device_cfg[CONF_HUMIDITY])
                cg.add(var.set_humidity_sensor(sens))
            if CONF_RAIN in device_cfg:
                sens = await sensor.new_sensor(device_cfg[CONF_RAIN])
                cg.add(var.set_rainfall_sensor(sens))
            cg.add(parent.add_device(var, device_cfg[CONF_DEVICE]))
