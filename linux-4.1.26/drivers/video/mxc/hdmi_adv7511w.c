/******************************************************************************
*
ADV7511W chip initilization for interfacing ACE-2D subsystem
*
******************************************************************************/


#include <linux/console.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/fb.h>
#include <linux/fsl_devices.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <video/edid.h>

#define ADV7511_REG_VIDEO_INPUT_CFG1		0x16
#define ADV7511_REG_POWER			0x41
#define ADV7511_REG_AVI_INFOFRAME(x) 		(0x55 + (x))
#define ADV7511_REG_INT(x) 			(0x96 + (x))
#define ADV7511_REG_INPUT_CLK_DIV		0x9d
#define ADV7511_REG_HDMI_POWER			0xa2
#define ADV7511_REG_CEC_I2C_ADDR		0xe0
#define ADV7511_REG_CEC_CTRL			0xe4
#define ADV7511_I2C_FREQ_ID_CFG_RATE_MASK	0xf9
#define ADV7511_REG_INFOFRAME_UPDATE		0x49
#define ADV7511_REG_HDCP_HDMI_CFG		0xaf
#define ADV7511_REG_BCAPS			0xbb
#define ADV7511_REG_TIMING_GEN_SEQ		0xde
#define ADV7511_REG_AN(x) 			(0xb0 + (x))


/**********************************************************
 * FUNCTION: adv_to_i2c
 **********************************************************/
static struct i2c_client *adv_to_i2c(struct adv_data *adv)
{
    return adv->client;
}

/**********************************************************
 * FUNCTION: adv_write
 **********************************************************/
static s32 adv_write(const struct i2c_client *client,u8 command, u8 value)
{
    return i2c_smbus_write_byte_data(client, command, value);
}

static int wakeup_adv_chip(struct adv_data *adv)
{
    struct i2c_client *client = adv_to_i2c(adv);
    int ret;

    ret = adv_write(client, ADV7511_REG_POWER, 0x10);

    return ret;
}

static int adv_fixed_registers(struct adv_data *adv)
{
    struct i2c_client *client = adv_to_i2c(adv);
    int ret;

    ret = adv_write(client, ADV7511_REG_INT(2), 0x03);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_REG_INT(4), 0xe0);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_REG_INT(6), 0x30);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_REG_INPUT_CLK_DIV, 0x61);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_REG_HDMI_POWER, 0xa4);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, 0xa3, 0xa4);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_REG_INT(4), 0xe0);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_REG_CEC_I2C_ADDR, 0xd0);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_I2C_FREQ_ID_CFG_RATE_MASK, 0x00);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_REG_AVI_INFOFRAME(0), 0x02);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    return ret;
}

static int adv_proper_operation_fixed_values(struct adv_data *adv)
{
    struct i2c_client *client = adv_to_i2c(adv);
    int ret;

    ret = adv_write(client, ADV7511_REG_INFOFRAME_UPDATE, 0xA8);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_REG_INT(3), 0x02);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_REG_BCAPS, 0x00);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_REG_TIMING_GEN_SEQ, 0x10);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_REG_CEC_I2C_ADDR, 0xD0);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_REG_CEC_CTRL, 0x60);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_I2C_FREQ_ID_CFG_RATE_MASK, 0x00);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    ret = adv_write(client, ADV7511_REG_INT(4), 0xE0);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }
    return ret;
}

static int adv_initialize(struct adv_data *adv)
{
    struct i2c_client *client = adv_to_i2c(adv);
    int ret;

    /*Configure the Chip to use HDMI Mode*/

    ret = adv_write(client, ADV7511_REG_HDCP_HDMI_CFG, 0x16);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }

    /* RGB ouput, 444, 8 bit */

    ret = adv_write(client, ADV7511_REG_VIDEO_INPUT_CFG1, 0x20);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }
    /* No delay for input video clock try 0xC0 also*/
    ret = adv_write(client, ADV7511_REG_AN(10), 0x60);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }
    /* Add some delay before writing An*/

    ret = adv_write(client, 0xFD, 0xE0);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }
    /* Add some delay before re-starting HDCP */

    ret = adv_write(client, 0xFE, 0X80);
    if(ret<0) {
        DEBUG("error: adv register write failed with error %d\n", ret);
        return -1;
    }
    else
        printk(KERN_INFO "Initializing ADV7511 HDMI Completed\n");

    return ret;
}


/**********************************************************
 * FUNCTION: adv_probe
 **********************************************************/

static int adv_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
    struct i2c_adapter *adap = to_i2c_adapter(client->dev.parent);
    int err;

    if (!i2c_check_functionality(adap, I2C_FUNC_SMBUS_BYTE)) {
        dev_err(&client->dev, "i2c_check_functionality error\n");
        return -ENODEV;
    }

    adv = devm_kzalloc(&client->dev, sizeof(*adv), GFP_KERNEL);
    if (!adv)
        return -ENOMEM;

    adv->client = client;
    i2c_set_clientdata(client, adv);

    err = wakeup_adv_chip(adv);
    if(err < 0) {
        DEBUG("error: Powering up ADV failed %d\n", ret);
        return -1;
    }

    err = adv_fixed_registers(adv);
    if(err < 0) {
        DEBUG("error: Settingup default failed %d\n", ret);
        return -1;
    }

    err = adv_proper_operation_fixed_values(adv);
    if(err < 0) {
        DEBUG("error: register settings for 2DACE failed %d\n", ret);
        return -1;
    }

    err = adv_initialize(adv);
    if(err < 0) {
        DEBUG("error: adv initilize failed %d\n", ret);
        return -1;
    }

    return 0;
}

static int adv_remove(struct i2c_client *client)
{
   struct i2c_client *client = adv_to_i2c(client);
   
   i2c_unregister_device(client);
   kfree(adv);

return 0;

	return 0;
}

/**********************************************************
 * DRIVER configuration
 **********************************************************/
static const struct i2c_device_id adv_id[] = {
    { "adv", 0 },
    {},
};
MODULE_DEVICE_TABLE(i2c, adv_id);

static const struct of_device_id adv_dt_ids[] = {
    { .compatible = "fsl,adv", },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, adv_dt_ids);

static struct i2c_driver adv_i2c_driver = {
    .driver = {
        .name = "adv",
        .owner = THIS_MODULE,
        .of_match_table = adv_dt_ids,
    },
    .probe = adv_probe,
    .remove = adv_remove,
    .id_table = adv_id,
};
module_i2c_driver(adv_i2c_driver);


MODULE_AUTHOR("Nikhil Varghese<nikhilimmu@gmail.com>");
MODULE_DESCRIPTION("ADV7511W driver");
MODULE_LICENSE("GPL");

