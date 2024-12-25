# Linux Kernel Module Programming Guide 

I will be following the programming course for Linux Kernel programming:
[Linux Kernel Programming](https://sysprog21.github.io/lkmpg/#introduction)

I will also be using an Ubuntu Server VM I made with QEMU and KVM

## Prerequisites

Ensure you have a C foundation and get comfortable with `modprobe`, `insmod`,
and `depmod`, which you can download with:

```sh
sudo apt-get install build-essential kmod
```

To view all modules currently loaded you can use to options:

1. `lsmod`
2. `cat /proc/modules`

Now get the valuable linux header files for the kernel with these commands:

```sh
sudo apt-get update
uname -r
```

The `uname -r` will provide information of which kernel headers you should get.

Then you can install the headers and kmod if you haven't already with:

```sh
sudo apt-get install kmod linux-headers-`uname -r`
```

Key Files to Learn/Read:

1. [`include/linux/printk.h`](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/tree/include/linux/printk.h): Gives info the priority macros
2. [`Documentation/kbuild/modules.rst`](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/tree/Documentation/kbuild/modules.rst): Infor for compile kernel modules
3. [`Documentation/kbuild/makefiles.rst`](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/tree/Documentation/kbuild/makefiles.rst): Info for kernel Makefiles
    - Real world Makefile example: [link](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/tree/drivers/char/Makefile)
4. [`include/linux/moduleparam.h`](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/tree/include/linux/moduleparam.h): Gives insights into how module can have command line arguments

### Precomplied vs Compiling your Kernel

Many distributions come already with a precompiled kernel with flags already 
set, but recompiling it from source will give you the option to set helpful
debugging flags.

Note this is `Version Magic` string in every kernel and module where if it
doesn't match the kernel's versions it will reject insertion. We can check that 
`version magic` with `modinfo module.ko`

Typically like in my case module development occurs in a VM or sandboxed 
environments because you typically never want to recompile the kernel in a 
production system.

### Externel Functions (printf??)

No, flat out you can't use externel functions in kernel programming only the 
functions and macros provided by the kernel are the ones you can use. If you're
curious which symbols are exported at your kernel look at `/proc/kallsyms`

### Namespace Pollution

Basically it's like it sounds you want to contain your code in the kernel
because even the smallest module will be linked again the entire kernel. That is
why you see so much static variables and functions and well-defined prefix for
symbols as it follows convention and makes it simple enough to have a 
consistent scheme for all to follow.

Remember the `/proc/kallsyms` ahs all the symbols that the kernel knows about
and that are accesible to our modules since they share the kernel's codespace.

### Memory Managment

Note: Most of our discussions will always be in terms of monolithich kernel's
instead of microkernels.

Recall for operating systems memory management is quite tough, though you don't
have to be an expert, the important thing to remember is lots of the memory
is all logical (virtual) addresses. It would be far to unsafe for pointers
in C/C++ to point to real addresses, so they really just point to logical ones.
Remember processes can't diretly interact with one another's virtual memory
instead this use `mmap` and create shared memory spaces.

The kernel also has it's own memory space, so don't try to step on it since it
can be quite serious.


### The `/dev` Folder

Here is where all device files are typically found on Unix when you do an
`ls -la /dev` you will seem some instances like this:

```sh
crw-rw---- 1 root dialout   4,  79 Dec 24 21:59 ttyS15
crw-rw---- 1 root dialout   4,  80 Dec 24 21:59 ttyS16
crw-rw---- 1 root dialout   4,  81 Dec 24 21:59 ttyS17
```

The `4` so the leading number before the comma is called a major number and it
denotes that those all devices use the same driver. The numbers to the right
of the of the comma are known as minor numbers and they just help the driver
distinguish what each part of the hardware the device file controls.

There are two main types of device:

1. character devices: typically associated with file operations, and can accept
                      as many bytes as they want
2. block devices: typically associated with storage, so they have a buffer for 
                  requests and accept input and output only in blocks

Most devices in real world are character devices, which is denoted by `c` in
the output from the `ls -la /dev`, and a block device would have a `b` as the 
first letter

To see the determined assignment of major numbers refer here:
[Documentation/admin-guide/devices.txt](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/tree/Documentation/admin-guide/devices.txt)

Funny enough `/dev` is just the location of preference and convention now, so 
stick with putting you devices there. Also you can create devices such as a 
char device names coffee like this:

```sh
mknod /dev/coffee c 12 2
```

Last word of detail is the major number is user by kernel to index the driver,
and the minor number is for the driver to index

## Character Device Drivers

Interacting with the hardware and writing the module involves the
`file_operations` strucutre defined in [`include/linux/fs.h`](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/tree/include/linux/fs.h). A modern implmentaiton is the `proc_ops` in that same
file

However, you don't need to implment all the functions in that structure only
the ones relevant to your device.

Fun note, but the `read`, `write`, and `seek` operations are guaranteed 
thread-safe if using an `f_pos` lock.

Each device is a `file` structure and it not a `file` on disk, that is an inode.

To register a device you may use `register_chrdev`, but you should not manually
assign a major number instead let the let kernel assign it by passing a major 
number of `0` to `register_chrdev`. 

To reduce the range of minor addresses you can then use the cdev interface and 
give a range of device numbers iwth `register_chrdev_region` or 
`alloc_chrdev_region`. Then initalize the `struct cdev` and associte it with the
device numbers, but common practice is to embedded this structu in your own
device specific struct.

## 