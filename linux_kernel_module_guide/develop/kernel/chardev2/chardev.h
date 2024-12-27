/* 
 * chardev.h - the header file with the loctl definitions.
 *
 * The declarations here have to be in a header file, because they need
 * to be known both to the kernel module (in chardev2.c) and the process
 * calling ioctl() (in userspace_ioctl.c).
 */

#ifndef CHARDEV_H
#define CHARDEV_H

#include <linux/ioctl.h>

/* The major device number. We can not rely on dynamci registration 
 * any more, becasue ioctls need to know it.
 */
#define MAJOR_NUM 100

/* Set the message of the device driver */
#define IOCTL_SET_MSG _IOW(MAJOR_NUM, 0, char *)
/* _IOW means that we are creating an ioctl command number for passing 
 * information from a user procres ot the kernel module.
 * 
 * The first arguments, MAJOR_NUM, is the major device number we are using.
 * 
 * The first arguments, MAJOR_NUM, is the major device number we are using.
 * 
 * The second argument is the number of the command (ther could be several
 * with different meanings).
 * 
 * The third arugment is the type we want to get from the process ot the
 * kernel.
 */

/* Get the message of the device driver */
#define IOCTL_GET_MSG _IOR(MAJOR_NUM, 1, char *)
/* This ioctl is used for ouptut, to get the message of the device driver.
 * However, we still need the buffer to place the emssage in to be input, 
 * as it is allocated by the procress.
 */\

 /* Get the n'th byte of the message */
 #define IOCTL_GET_NTH_BYTE _IOWR(MAJOR_NUM, 2, int)
 /* The IOCTL is used fro both input and output. it recieved from the user
  * a number, n, and returns message[n].
  */

 /* The name of the device file */
 #define DEVICE_FILE_NAME "char_dev"
 #define DEVICE_PATH "/dev/char_dev"

 #endif