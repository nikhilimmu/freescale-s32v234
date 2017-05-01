/*
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/hrtimer.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/input/mt.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/module.h>

struct max9271 {
    struct i2c_client               *client;
};


static struct i2c_client *max_to_i2c(struct max9271 *max)
{
    return max->client;
}


static s32 max9271_write(const struct i2c_client *client,u8 command, u8 value)
{
    return i2c_smbus_write_byte_data(client, command, value);
}
EXPORT_SYMBOL(max9271_write);

static s32 max9271_read(const struct i2c_client *client,u8 reg, u8 *val)
{
    *val=i2c_smbus_write_byte_data(client, reg);
}
EXPORT_SYMBOL(max9271_read);

static int max_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
    struct i2c_adapter *adap;
    int err;
    struct max96706 *max;

    adap = to_i2c_adapter(client->dev.parent);

    if (!i2c_check_functionality(adap, I2C_FUNC_SMBUS_BYTE)) {
        dev_err(&client->dev, "i2c_check_functionality error\n");
        return -ENODEV;
    }

    max = devm_kzalloc(&client->dev, sizeof(*max), GFP_KERNEL);
    if (!max)
        return -ENOMEM;

    max->client = client;
    i2c_set_clientdata(client,max);

    err = init_max(max)
    if(err < 0) {
        dev_err(&client->dev, "Failed to initilize deserlizer");
        return -1;
    }
    return 0;
}


static const struct i2c_device_id max_id[] = {
    { "max9271", 0 },
    {               },
};
MODULE_DEVICE_TABLE(i2c, max_id);

static const struct of_device_id max_dt_ids[] = {
    { .compatible = "fsl,max9271", },
    { /* sentinel */                }
};
MODULE_DEVICE_TABLE(of, max_dt_ids);

static struct i2c_driver max_i2c_driver = {
    .driver = {
        .name = "maxim-9271",
        .owner = THIS_MODULE,
        .of_match_table = max_dt_ids,
    },
    .probe = max_probe,
    .remove = max_remove,
    .id_table = max_id,
};
module_i2c_driver(max_i2c_driver);

MODULE_AUTHOR("Nikhil Varghese <nikhilimmu@gmail.com>");
MODULE_DESCRIPTION("MAX9271 serlizer driver");
MODULE_LICENSE("GPL");
