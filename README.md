# Prereqs

- Know Computer Architecture
- Know C
- Know Assembly (ARM prefered)

# Getting Started

I already had Fedora installed, so Linux completed.

However, I just found out about KVM and QEMU.Best way to learn about it is here directly: [Virtulization](https://developer.fedoraproject.org/tools/virtualization/about.html)

I then use virtual-manager to load an ubuntu server ISO, which I got ssh working 
with just because it might be useful to know both: [Ubuntu Server](https://ubuntu.com/download/server)

Now just ssh'ing and using as my sandbox

Yet it is important to perform an update and get the linux headers/libraries,
the uname basically here will gives us the version

```sh
sudo apt update && upgrade -y
sudo apt install -y build-essential linux-headers-$(uname -r) kmod
```

## What is a kernel, System, and Bootup
It is the layer between userspace and the hardware basically a governor. The
kernle managers processes (with a scheduler) and manage hardware (wuith kernel
drivers). These drivers think of like one for a cmaera the reason our computer 
can interact with teh camer is because of that driver. 

A kernel is a byunch of files basically whole infrastrutre for all these devices.
However, to interact witht CPU is must be moved to DRAM. The DRAM has this copy
occur mainly if the ROM boots and starts the process. The ROM boot is really
just mean to start the *bootloader* that intermediate coode that provide the 
kernel and *dtb* to the DRAM.

Here the kernel will continously be refered to a single entitiy the Linux Kernel 
in our case. Along with the kernel thre is a *dtb* and *bootloader*.

*dtb* (Data Tree Blob): A strcutre that contains information on which hardware
is connected to what, which addresses are teh ardware availble, and other
metadatat like that.

The *dtb* is passed over the bootloader, which also passes the kernel. It really
is that intermediate step to let all the hardware know of the mappings in the 
system. 

The CPU will acquire the address of *dtb* from teh bootloader and store it in 
one of its registers. The *dtb* is read by the kernel, which then permit it to
identify how to intercase with new peripheals.

If drivers are missing then the kernel will report and they will manually need
to be loaded, which is whole point of what we are learnign lol.

# Why User Space, Kernel Space, System Calls, and Device Drivers

The Linux kernel runs in a privileged mode so can really interact quite easily 
with the hardware and has more permissions than most of us. In order to 
stop it from oversharing with it scheduler since near all other applications
run in unprivileged mode and are at the will at the kernel.

The user applicaiton must talk to the kernel to make these requets for what 
hardware or data is produced it needed. Some CPUs like ARM have exception levels.

For instance EL1 is for kernel and it acquires status from status registers.
The status is form the user applications. The user applications run in EL0.

To get these status or to interact from user space to the kernel we must use 
system calls to tell the kernel what we want it to do. The kernel will give 1
or a 0 depending on the request provided in the system call(s). Two of such
vital and widely used system calls are *read* and *write*.

Notice that the kernel will need to ahve these implmeentaions of system calls 
to be able to process and read them. The device drivers implment system calls
so that our applcications cna interact with the kernel for requets.

In the kernel ther eare *built in drivers* to the ones that autonmatically are
always needed like the default ones. If the linux kernel doesn't have your drivers
you can load and compile it into the live system, so that you can check if 
they interact correctly. These are *loadable kernel modules/drivers*. 

*Constructor* and *Destructor* are vital here sinxe our loaded drivers will
make teh kernel calls these to initate them.

# What Files and File Operations, System Calls and Drivers

The drivers typically implment two types of system calls:

1. file operations (e.g. read/write)
2.  hardare management/access (e.g. ioctl)

Recall Linux treats everything like file, so we can just perform operations on
all these files, so interacting with the hardware is really just interacting
with a file basically.

Linux does map hardware to a file, essentially exposing it as a file. The 
kernel drivers are them meant to tell what is supposed to happen the file is
opened, read, written, etc.

For drivers there are mainly three types:

1. char (character driver related to file operations)
2. block (related to fileststem, storage)
3. network (related to ethernet and so forth)

For example, if we are using UART and our user code atemps to read/write to the 
file /dev/ttyACMO, the read() system call will be used, which will then
leverage the read() implmented by the UART> This triggers that device
driver from the UART to read information and then pass it back to the user.

user_code -> read() -> /dev/ttyACM0 -> read() -> UART

For example of these system calls serach and find `struct file_operations` here
at this `fs.h` file from the linux: [fs.h](https://github.com/torvalds/linux/blob/master/include/linux/fs.h)


## Kernel Files

These files are located in `.ko` files and note you use C macros such as
`module_init` and `module_exit`

Here is the required Macro in the kernel file:

```cpp
MODULE_LICENSE();
```

The optional ones are 

```cpp
MODULE_AUTHOR();
MODULE_DESCRIPTION();
```

To load, observer output, and remove kernel modules we can run these commands:

1. Load: `sudo insmod [filename].ko`
2. Observe: `sudo dmesg`
3. Remove:  `sudo rmmod [filename].ko`

Then to view information of the kernel driver:

1. `modinfo [filename].ko`

Another method to confirm if loaded/unloaded our kernel module or just to see
which dependencies are other drivers are depending on since some drivers require
others:

1. `lsmod` (lists out the modules in use right now)

This is the a great way to confirm if you modules were safelyu removed or 
inserted inot the kernel.

## What are these kernel functions doing

1. `insmod`:
    - calls `init_module` to hint the kernel that a module insertion is 
      attempted
    - Transfers control to the the kernel
    - Kernel execute `sys_init_module`
    - Verifies permissions
    - `load_module` function is called
        1. Check the sanityu of the .ko
        1. creates the memory.
        1. copies from user space to kernel space.
        1. resolves sysmbols.
        1. Returns a reference to the Kernel
    - Adds the reference to a linked list that has all the loaded modules
    - `module_init` listed function

2. `rmmod`:
    - calls `delete_module()` which hint sthe kernel that a module is to bre
      removed
        1. Control tranfed to the kernel.
    - kernel exectures `sys_delete_module()`
        1. Checks the permissions of the one requesting.
        1. Checks if any other loaded modules needs teh current module.
        1. Checks if the moudle is acutally loaded!
        1. Execute the funciton provided in `module_exit`
        1. `free_module()`
            1. removes references and kernel object reference
            1. performs any other cleanup.
            1. unloads the module
            1. Changes the sate in list
            1. removes it from the list and free the memory

3. `modinfo`:
    - Basically provides the metadata for the loadable module