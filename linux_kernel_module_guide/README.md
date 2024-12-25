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