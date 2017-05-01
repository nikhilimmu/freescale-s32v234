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

/*Deserlizer Registers*/
#define MAX_ID 0x1E
#define MAX_CTRL_CHANNEL 0x0A
#define MAX_I2C_CONFIG 0x0D
#define MAX_CONFIG 0x07
#define MAX_HIM 0x06
#define MAX_PKT_CRTL 0x9A
#define MAX_REV_GAIN 0x97
#define MAX_SER_CONFIG 0x04
#define MAX_SER_GAIN 0x08

struct max9706 {
    struct i2c_client               *client;
    int powerstate;
    int fsync;
    int video_data_type;
    int pbrs_enable;
};


static struct i2c_client *max_to_i2c(struct max9706 *max)
{
    return max->client;
}


static s32 max_write(const struct i2c_client *client,u8 command, u8 value)
{
    return i2c_smbus_write_byte_data(client, command, value);
}

static s32 max_read(const struct i2c_client *client,u8 reg, u8 *val)
{
    *val=i2c_smbus_write_byte_data(client, reg);
}

unsigned char verify_device(struct max9706 *max)
{
    struct i2c_client *client = max_to_i2c(adv);
    unsigned char id;
    unsigned char retval=1;

    id=max_read(client,MAX_ID, &id);
    if(id != 0x40)
        retval=0;

    return retval;
}

unsigned char disable_high_immunity(struct max9706 *max)
{
    struct i2c_client *client = max_to_i2c(adv);
    unsigned char retval=1;

    retval=max_write(client,MAX_HIM,0x6F);
    if(retval< 0)
    {
        dev_err(&client->dev, "failed to disable dserlizer in him mode\n");
        retval=0;
    }
    return retval;
}
unsigned char enable_auto_ack(struct max9706 *max)
{
    struct i2c_client *client = max_to_i2c(adv);
    unsigned char retval=1;

    retval=max_write(client,MAX_I2C_CONFIG,0xB6);
    if(retval< 0)
    {
        dev_err(&client->dev, "failed to enable auto ack\n");
        retval=0;
    }
    return retval;
}

unsigned char set_revers_gain(struct max9706 *max)
{
    struct i2c_client *client = max_to_i2c(adv);
    unsigned char retval=1;

    retval=max_write(client,MAX_CONFIG,0x02);
    if(retval< 0)
    {
        dev_err(&client->dev, "failed to set reverse gain in deserlizer\n");
        retval=0;
    }
    return retval;
}
unsigned char set_pkt_ctrl(struct max9706 *max)
{
    struct i2c_client *client = max_to_i2c(adv);
    unsigned char retval=1;

    retval=max_write(client,MAX_PKT_CRTL,0x19);
    if(retval< 0)
    {
        dev_err(&client->dev, "failed to set the packet ctrl\n");
        retval=0;
    }
    return retval;
}

unsigned char set_gain(struct max9706 *max)
{
    struct i2c_client *client = max_to_i2c(adv);
    unsigned char retval=1;

    retval=max_write(client,MAX_REV_GAIN,0x62);
    if(retval< 0)
    {
        dev_err(&client->dev, "failed to disable dserlizer in him mode\n");
        retval=0;
    }
    return retval;
}
unsigned char set_revers_gain(struct max9706 *max)
{
    struct i2c_client *client = max_to_i2c(adv);
    unsigned char retval=1;

    retval=max_write(client,MAX_CONFIG,0x02);
    if(retval< 0)
    {
        dev_err(&client->dev, "failed to disable dserlizer in him mode\n");
        retval=0;
    }
    return retval;
}
unsigned char set_double_mode(struct max9706 *max)
{
    struct i2c_client *client = max_to_i2c(adv);
    unsigned char retval=1;

    retval=max_write(client,MAX_CONFIG,0x84);
    if(retval< 0)
    {
        dev_err(&client->dev, "failed to disable dserlizer in him mode\n");
        retval=0;
    }
    return retval;
}

unsigned char set_serlizer_in_config(struct max9706 *max)
{
    struct i2c_client *client = max_to_i2c(adv);
    unsigned char retval=1;

    retval=max_write(client,MAX_SER_CONFIG,0x47);
    if(retval< 0)
    {
        dev_err(&client->dev, "failed to disable dserlizer in him mode\n");
        retval=0;
    }
    return retval;
}
unsigned char set_serlizer_gain(struct max9706 *max)
{
    struct i2c_client *client = max_to_i2c(adv);
    unsigned char retval=1;

    retval=max_write(client,MAX_SER_GAIN,0x01);
    if(retval< 0)
    {
        dev_err(&client->dev, "failed to disable dserlizer in him mode\n");
        retval=0;
    }
    return retval;
}
unsigned char enable_serlizer_config(struct max9706 *max)
{
    struct i2c_client *client = max_to_i2c(adv);
    unsigned char retval=1;

    retval=max9271_write(client,MAX_SER_CONFIG,0x47);
    if(retval< 0)
    {
        dev_err(&client->dev, "failed to disable dserlizer in him mode\n");
        retval=0;
    }
    return retval;
}
unsigned char enable_serlizer_gain(struct max9706 *max)
{
    struct i2c_client *client = max_to_i2c(adv);
    unsigned char retval=1;

    retval=max9271_write(client,MAX_SER_GAIN,0x01);
    if(retval< 0)
    {
        dev_err(&client->dev, "failed to disable dserlizer in him mode\n");
        retval=0;
    }
    return retval;
}
static int init_max(struct max9706 *max)
{
    struct i2c_client *client = max_to_i2c(adv);

    if(!verify_device(max)) //Deserlizer not recognized
    {
        dev_err(&client->dev, "failed to detect deserlizer\n");
        return -EIO;
    }
    if(!disable_high_immunity(max))
        return -EIO;
    if(!enable_auto_ack(max))
        return -EIO;
    if(!enable_serlizer_config(max))
        return -EIO;
    if(!enable_serlizer_gain(max))
        return -EIO;
    if(!set_serlizer_in_config(max))
        return -EIO;
    if(!set_serlizer_gain(max))
        return -EIO;
    if(!set_revers_gain(max))
        return -EIO;
    if(!set_pkt_ctrl(max))
        return -EIO;
    if(!set_gain(max))
        return -EIO;
return 0;
}

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
    { "max96706", 0 },
    {               },
};
MODULE_DEVICE_TABLE(i2c, max_id);

static const struct of_device_id max_dt_ids[] = {
    { .compatible = "fsl,max96706", },
    { /* sentinel */                }
};
MODULE_DEVICE_TABLE(of, max_dt_ids);

static struct i2c_driver max_i2c_driver = {
    .driver = {
        .name = "maxim",
        .owner = THIS_MODULE,
        .of_match_table = max_dt_ids,
    },
    .probe = max_probe,
    .remove = max_remove,
    .id_table = max_id,
};
module_i2c_driver(max_i2c_driver);

MODULE_AUTHOR("Nikhil Varghese <nikhilimmu@gmail.com>");
MODULE_DESCRIPTION("MAX96706/MAX9271 serlizer-deserlizer driver");
MODULE_LICENSE("GPL");
