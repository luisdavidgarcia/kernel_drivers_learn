# OS Development using the Linux Kernel

Basically as before we have our own VM on QEMU and KVM. 


## Optional: Installing your own kernel

However, in some cases you may want to put another kernel in another drive.
This requires creating a new partion.

Then o have our own linux kernel we often these directories:

```sh
mkdir -pv ./{bin,sbin,etc,lib,lib64,var,dev,proc,sys,run,tmp}
```

This is useful is creating you own volume for where to put the kernel. Now
sometimes create some dummy nodes

```sh
mknod -m 600 ./dev/console c 5 1
mknod -m 666 ./dev/null c 1 3
```

Now you need to create a boot directory

```sh
mkdir boot
cd boot
```

You can copy your local `/boot` or make your own but to find we just need to do:

```sh
ls -la /boot
```
In my case I will copy these files and grub-install:

```sh
cp /boot/vmlinuz-6.8.0-51-generic
cp /boot/initrd.img-6.8.0-51-generic
grub-install /dev/sdb1 --skip-fs-probe --bot-directory=/mnt/myos/boot
```

You will then create a grub config `grub.cfg`:

```sh
cd /boot/grub/
set default=0
set timeout=0

myenuentry "MyOS 0.0.0.1" {
    linux /boot/vmlinuz-4.19.0-10-amd64 root=/dev/sdb1 ro
    initrd /boot/initrd.img-6.8.0-51-generic
}
```

