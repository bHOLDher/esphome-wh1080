import esphome.codegen as cg
from esphome.components import remote_base, remote_receiver
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@bHOLDher"]
DEPENDENCIES = ["remote_receiver"]

wh1080_ns = cg.esphome_ns.namespace("wh1080")
Wh1080Component = wh1080_ns.class_("Wh1080Component", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Wh1080Component),
        cv.GenerateID(remote_base.CONF_RECEIVER_ID): cv.use_id(
            remote_receiver.RemoteReceiverComponent
        ),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await remote_base.register_listener(var, config)
