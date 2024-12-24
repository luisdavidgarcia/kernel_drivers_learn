# Starter Linux Kernel Module Development Guide

## Prerequisites
- Computer Architecture fundamentals
- C programming expertise
- Assembly language knowledge (ARM preferred)

## Development Environment Setup

### Base System Requirements
1. Linux distribution (Fedora used in this guide)
2. Virtual environment using KVM and QEMU
   - Reference: [Fedora Virtualization Guide](https://developer.fedoraproject.org/tools/virtualization/about.html)
3. Ubuntu Server in VM for sandbox testing
   - Download: [Ubuntu Server](https://ubuntu.com/download/server)
   - Configured with SSH access

### Required Packages
```bash
sudo apt update && upgrade -y
sudo apt install -y build-essential linux-headers-$(uname -r) kmod
```

## Core Concepts

### Kernel Fundamentals
The kernel is the layer between userspace and hardware that:
- Manages processes through scheduling
- Controls hardware via kernel drivers
- Exists as files that must be loaded into DRAM during boot process

### Boot Process Components
1. **ROM Boot and Bootloader**: 
   - ROM boot starts the bootloader
   - Bootloader provides kernel and DTB to DRAM

2. **DTB (Device Tree Blob)**:
   - Contains hardware connection information
   - Maps hardware addresses
   - Stores system metadata
   - CPU receives DTB address from bootloader
   - Kernel reads DTB for peripheral interface identification

### Memory Space and Privilege Levels
1. **Kernel Space**
   - Runs in privileged mode (e.g., EL1 in ARM)
   - Direct hardware access
   - Full system permissions

2. **User Space**
   - Runs in unprivileged mode (e.g., EL0 in ARM)
   - Limited direct access
   - Must use system calls for kernel requests

### System Calls and Device Drivers
1. **System Calls**
   - Bridge between user and kernel space
   - Common examples: `read()` and `write()`
   - Returns success/failure

2. **Device Drivers Types**
   - Built-in drivers (compiled into kernel)
   - Loadable kernel modules (loaded at runtime)
   - Use constructors/destructors for initialization/cleanup

### Driver Categories
1. Character Drivers (char)
   - Handle file operations
   - Direct I/O access

2. Block Drivers
   - Filesystem related
   - Storage management

3. Network Drivers
   - Network interface management

## Kernel Module Development

### Required Module Macros
```c
MODULE_LICENSE();   // Required
MODULE_AUTHOR();    // Optional
MODULE_DESCRIPTION(); // Optional
```

### Module Management Commands
```bash
# Loading and Unloading
sudo insmod [filename].ko  # Load module
sudo rmmod [filename].ko   # Remove module

# Monitoring
sudo dmesg               # View kernel messages
modinfo [filename].ko    # Module information
lsmod                    # List loaded modules
```

### Module Loading Process (`insmod`)
1. Calls `init_module`
2. Transfers control to kernel
3. Kernel executes `sys_init_module`
4. Verifies permissions
5. Calls `load_module`:
   - Checks .ko file sanity
   - Creates memory
   - Copies from user to kernel space
   - Resolves symbols
   - Returns kernel reference
6. Adds to loaded modules list
7. Executes `module_init`

### Module Removal Process (`rmmod`)
1. Calls `delete_module`
2. Kernel executes `sys_delete_module`:
   - Checks permissions
   - Verifies dependencies
   - Confirms module is loaded
   - Executes `module_exit`
   - Calls `free_module`:
     - Removes references
     - Cleans up
     - Unloads module
     - Updates state
     - Frees memory

### Proc Filesystem
- Virtual filesystem for kernel-userspace interaction
- No physical files, only logical volumes
- Requires implementation of `proc_ops` struct
- File operations flow:
  - User space → read proc file → calls proc_read() → kernel space → linux driver

### Data Transfer (Kernel to User Space)
- Must return bytes read count
- Increment offset parameter by size read
- Check if offset equals desired bytes
- Return 0 (EOF) to indicate completion
- Applications like `cat` expect EOF to stop reading

## Resources
- [Linux fs.h Source](https://github.com/torvalds/linux/blob/master/include/linux/fs.h)
- [Linux proc_fs.h Source](https://github.com/torvalds/linux/blob/master/include/linux/proc_fs.h)