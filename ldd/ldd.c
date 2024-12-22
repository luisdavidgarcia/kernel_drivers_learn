// #include <linux/init.h>
// #include <linux/module.h>

// MODULE_LICENSE("GPL"); //*
// MODULE_AUTHOR("Luis David Garcia"); //!
// MODULE_DESCRIPTION("Our first dynamically loadable kernel module"); //!

// static int lg_module_init(void) {
//     printk("Hello, World! (from LG!!)\n");
//     return 0;
// }

// static void lg_module_exit(void) {
//     printk("Good, Bye! (from LG!!)\n");
// }

// module_init(lg_module_init);
// module_exit(lg_module_exit);

#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#define BUFSIZE 100

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luis David Garcia <luisdavidgarcia@protonmail.com");
MODULE_DESCRIPTION("Basic read loadable kernel module");

static struct proc_dir_entry *custom_proc_node;
static char stored_message[BUFSIZE];
static size_t stored_length = 0;

static ssize_t lg_read(
    struct file* file_pointer, 
    char *user_space_buffer, 
    size_t count, 
    loff_t* offset
) {
    int result;

    printk("lg_read\n");

    if (*offset >= stored_length) 
        return 0;

    if (count > stored_length - *offset) {
        count = stored_length - *offset;
    }

    result = copy_to_user(user_space_buffer, stored_message + *offset, count);
    if (result != 0) {
        return -EFAULT;
    }

    *offset += count;

    return count;
}

static ssize_t lg_write(
    struct file* file_pointer, 
    const char* user_space_buffer, 
    size_t count, 
    loff_t* offset
) {
    printk("lg_write\n");

    // Allow for appending and ensure null terminator can be added
    if (*offset + count > BUFSIZE - 1) {
        return -EFAULT;
    }

    if (copy_from_user(stored_message, user_space_buffer, count)) {
        return -EFAULT;
    }

    // Null terminate the string
    stored_message[count] = '\0';
    stored_length = count;
    *offset = count;

    return count;
}


struct proc_ops driver_proc_ops = {
    .proc_read = lg_read,
    .proc_write = lg_write
};

static int lg_module_init(void) {
    printk("lg_module_init: entry\n");
    
    custom_proc_node = proc_create(
        "lg_driver", 
        0666, 
        NULL, 
        &driver_proc_ops
    );
    
    if (custom_proc_node == NULL) {
        printk("lg_module_init: error\n");
        return -1;
    }

    printk("lg_module_init: exit\n");
    return 0;
}

static void lg_module_exit(void) {
    printk("lg_module_exit: entry\n");
    proc_remove(custom_proc_node);
    printk("lg_module_exit: exit\n");
}


module_init(lg_module_init);
module_exit(lg_module_exit);