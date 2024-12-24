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
