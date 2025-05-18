#TODO: Modernize since spi_busnum_to_master is deprecated
#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/device.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luis David Garcia");
MODULE_DESCRIPTION("An SPI module for ILI9341 LCD 240x320 Display");

#define MY_BUS_NUM 0
#define DISPLAY_INFO_COMMAND 0x04
#define DISPLAY_INFO_SIZE 24

static struct spi_device *ili9341_dev;

static int __init my_init(void)
{
	struct spi_master *master;

	/* Parameter for SPI device */
	struct spi_board_info spi_device_info = {
		.modalias = "ili9341",
		.max_speed_hz = 6350000,
		.bus_num = MY_BUS_NUM,
		.chip_select = 0,
		.mode = 3, 
	};

	/* Get access to spi bus */
	master = spi_busnum_to_master(MY_BUS_NUM);
	/* Check if we could get the master */
	if (!master) {
		printk("There is no spi bus with Nr . %d\n", MY_BUS_NUM);
		return -1;
	}

	/* Create new SPI device */
	ili9341_dev = spi_new_device(master, &spi_device_info);
	if (!ili9341_dev) {
		printk("Could not create device!\n");
		return -1;
	}

	ili9341_dev->bits_per_word = 0;

	/* Setup the bus for device's parameters */
	if (spi_setup(ili9341_dev) != 0) {
		printk("Could not change bus setup!\n");
		spi_unregister_device(ili9341_dev);
		return -1;

	}

	/* Read Display Information */
	u8 cmd = 0x04;
	if (spi_write(ili9341_dev, &cmd, 1) != 0) {
		printk("Could not write to obtain display information!\n");
		spi_unregister_device(ili9341_dev);
		return -1;
	}	
	u8 rx_buf[4];
	if (spi_read(ili9341_dev, rx_buf, 4) != 0) {
		printk("Could not read display information!\n");
		spi_unregister_device(ili9341_dev);
		return -1;
	}

	printk(KERN_INFO "Display ID: 0x%02X 0x%02X 0x%02X\n",
		rx_buf[1], rx_buf[2], rx_buf[3]);

 	printk("Hello, Kernel!\n");

 	return 0;  
}

static void __exit my_exit(void)
{
	if (ili9341_dev)
		spi_unregister_device(ili9341_dev);
	printk("Goodbye, Kernel!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

