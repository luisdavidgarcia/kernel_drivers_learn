/*
 * hello-2.c - Demonstrating the module_init() and module_exit() marcos.
 * This is prefered over using init_module() and cleanup_module().
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

static int __init hello_2_init(void) {
    pr_info("Hello, world 2\n");
    return 0;
}

static void __exit hello_2_exit(void) {
    pr_info("Goodbye, world 2\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);

MODULE_LICENSE("GPL");